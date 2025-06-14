/**
 * @file    usb_uart.c
 * @brief   串口调试功能实现
 */

#include "usb_uart.h"

/* 私有变量定义 */
uint8_t uart_rx_buffer[DEBUG_UART_RX_BUFFER_SIZE];
uint8_t uart_tx_buffer[DEBUG_UART_TX_BUFFER_SIZE];
volatile uint8_t uart_rx_done_flag = 0;
volatile uint16_t uart_rx_len = 0;
static uint8_t printf_buffer[DEBUG_UART_TX_BUFFER_SIZE];
static volatile uint8_t printf_busy = 0;

/**
 * @brief  初始化串口调试功能
 * @param  无
 * @retval 无
 */
void USB_UART_Init(void)
{
    /* 串口已在MX_UART4_Init中初始化 */
    
    /* 启动DMA接收 */
    USB_UART_ReceiveToIdle_DMA();
    
    /* 使能串口空闲中断 */
    __HAL_UART_ENABLE_IT(&DEBUG_UART, UART_IT_IDLE);
}

/**
 * @brief  反初始化串口调试功能
 * @param  无
 * @retval 无
 */
void USB_UART_DeInit(void)
{
    /* 停止DMA接收 */
    HAL_UART_DMAStop(&DEBUG_UART);
    
    /* 禁用串口空闲中断 */
    __HAL_UART_DISABLE_IT(&DEBUG_UART, UART_IT_IDLE);
}

/**
 * @brief  使用DMA接收数据直到空闲
 * @param  无
 * @retval 无
 */
void USB_UART_ReceiveToIdle_DMA(void)
{
    /* 清空接收缓冲区 */
    memset(uart_rx_buffer, 0, DEBUG_UART_RX_BUFFER_SIZE);
    
    /* 启动DMA接收 */
    HAL_UARTEx_ReceiveToIdle_DMA(&DEBUG_UART, uart_rx_buffer, DEBUG_UART_RX_BUFFER_SIZE);
    
    /* 禁用半传输中断 */
    __HAL_DMA_DISABLE_IT(DEBUG_UART.hdmarx, DMA_IT_HT);
    
    /* 确保初始状态下接收标志为0 */
    uart_rx_done_flag = 0;
}

/**
 * @brief  获取接收数据长度
 * @param  无
 * @retval 接收数据长度
 */
uint16_t USB_UART_GetRxLen(void)
{
    return uart_rx_len;
}

/**
 * @brief  清空接收缓冲区
 * @param  无
 * @retval 无
 */
void USB_UART_ClearRxBuffer(void)
{
    memset(uart_rx_buffer, 0, DEBUG_UART_RX_BUFFER_SIZE);
    uart_rx_len = 0;
    uart_rx_done_flag = 0;
}

/**
 * @brief  使用DMA发送数据
 * @param  data: 要发送的数据
 * @param  size: 数据大小
 * @retval 无
 */
void USB_UART_Transmit(uint8_t *data, uint16_t size)
{
    HAL_UART_Transmit_DMA(&DEBUG_UART, data, size);
}

/**
 * @brief  发送字符串
 * @param  str: 要发送的字符串
 * @retval 无
 */
void USB_UART_TransmitStr(char *str)
{
    USB_UART_Transmit((uint8_t *)str, strlen(str));
}

/**
 * @brief  格式化打印函数（DMA加速）
 * @param  format: 格式字符串
 * @param  ...: 可变参数
 * @retval 无
 */
void USB_UART_Printf(const char *format, ...)
{
    va_list args;
    uint32_t length;
    
    /* 等待上一次DMA传输完成 */
    while (printf_busy) {
        /* 等待DMA传输完成 */
    }
    
    va_start(args, format);
    length = vsnprintf((char *)uart_tx_buffer, DEBUG_UART_TX_BUFFER_SIZE, format, args);
    va_end(args);
    
    /* 标记正在发送 */
    printf_busy = 1;
    
    /* 使用DMA发送数据 */
    HAL_UART_Transmit_DMA(&DEBUG_UART, uart_tx_buffer, length);
}

/**
 * @brief  接收完成处理函数(内部使用)
 * @param  无
 * @retval 无
 */
void USB_UART_RxCpltCallback(void)
{
    uart_rx_done_flag = 1;
    
    /* 打印调试信息 */
    printf("USB_UART_RxCpltCallback: 接收完成\r\n");
}

/**
 * @brief  发送完成处理函数(内部使用)
 * @param  无
 * @retval 无
 */
void USB_UART_TxCpltCallback(void)
{
    /* 发送完成后的处理 */
}

/**
 * @brief  错误处理函数(内部使用)
 * @param  无
 * @retval 无
 */
void USB_UART_ErrorCallback(void)
{
    /* 错误处理 */
    USB_UART_ReceiveToIdle_DMA();
}

/**
 * @brief  空闲回调函数
 * @param  无
 * @retval 无
 */
void USB_UART_IdleCallback(void)
{
    /* 计算接收到的数据长度 */
    uart_rx_len = DEBUG_UART_RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(DEBUG_UART.hdmarx);
    
    /* 只有当接收到数据时才设置标志 */
    if(uart_rx_len > 0)
    {
        /* 设置接收完成标志 */
        uart_rx_done_flag = 1;
        
        /* 停止DMA接收 */
        HAL_UART_DMAStop(&DEBUG_UART);
        
        /* 确保字符串以0结尾 */
        uart_rx_buffer[uart_rx_len] = 0;
    }
    else
    {
        /* 如果没有接收到数据，重新启动接收 */
        USB_UART_ReceiveToIdle_DMA();
    }
}

/* printf重定向相关函数 */

/* 重定向printf函数 - 使用DMA加速 */
#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif
{
    static uint16_t index = 0;
    
    /* 将字符加入缓冲区 */
    printf_buffer[index++] = (uint8_t)ch;
    
    /* 如果是换行符或缓冲区已满或是回车符，则发送数据 */
    if (ch == '\n' || index >= DEBUG_UART_TX_BUFFER_SIZE - 1 || ch == '\r')
    {
        /* 等待上一次DMA传输完成 */
        while (printf_busy) {
            /* 等待DMA传输完成 */
        }
        
        /* 标记正在发送 */
        printf_busy = 1;
        
        /* 使用DMA发送数据 */
        HAL_UART_Transmit_DMA(&DEBUG_UART, printf_buffer, index);
        
        /* 重置索引 */
        index = 0;
    }
    
    return ch;
}

/* 重定向scanf函数 */
#ifdef __GNUC__
int __io_getchar(void)
#else
int fgetc(FILE *f)
#endif
{
    uint8_t ch = 0;
    /* 接收单个字符 */
    HAL_UART_Receive(&DEBUG_UART, &ch, 1, 0xFFFF);
    return ch;
}

/**
 * @brief  串口接收完成回调函数(HAL库回调)
 * @param  huart: 串口句柄
 * @retval 无
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == DEBUG_UART_HANDLE)
  {
    uart_rx_done_flag = 1;
    
    /* 打印调试信息 */
    printf("HAL_UART_RxCpltCallback: 接收完成\r\n");
    
    /* 重新启动DMA接收 */
    USB_UART_ReceiveToIdle_DMA();
  }
}

/**
 * @brief  串口发送完成回调函数(HAL库回调)
 * @param  huart: 串口句柄
 * @retval 无
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == DEBUG_UART_HANDLE)
  {
    /* 标记发送完成 */
    printf_busy = 0;
  }
}

/**
 * @brief  串口错误回调函数(HAL库回调)
 * @param  huart: 串口句柄
 * @retval 无
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == DEBUG_UART_HANDLE)
  {
    /* 错误处理 */
    USB_UART_ReceiveToIdle_DMA();
  }
}

/**
  * @brief  串口接收到空闲中断回调函数(HAL库回调)
  * @param  huart: 串口句柄
  * @param  Size: 接收到的数据大小
  * @retval 无
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if(huart->Instance == DEBUG_UART_HANDLE)
  {
    /* 只有当接收到数据时才设置标志 */
    if(Size > 0)
    {
      uart_rx_len = Size;
      uart_rx_done_flag = 1;
      
      /* 确保字符串以0结尾 */
      uart_rx_buffer[Size] = 0;
      
      /* 打印调试信息 */
      printf("HAL_UARTEx_RxEventCallback: 接收到 %d 字节数据\r\n", Size);
    }
    else
    {
      /* 如果没有接收到数据，重新启动接收 */
      USB_UART_ReceiveToIdle_DMA();
    }
  }
} 