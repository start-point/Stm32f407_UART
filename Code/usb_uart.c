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




/* 重定向printf函数 - 使用DMA加速 */
#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif
{
    static uint16_t index = 0;
    
    // 将字符加入缓冲区
    // 在写入之前检查是否会溢出
    if (index >= DEBUG_UART_TX_BUFFER_SIZE - 1) {
        // 缓冲区已满，先发送当前缓冲区内容
        // 等待上一次DMA传输完成
        while (printf_busy) {
            // 等待DMA传输完成
        }
        
        // 标记正在发送
        printf_busy = 1;
        
        // 使用DMA发送数据
        HAL_UART_Transmit_DMA(&DEBUG_UART, printf_buffer, index);
        
        // 重置索引
        index = 0;
    }
    
    printf_buffer[index++] = (uint8_t)ch;
    
    // 如果是换行符或缓冲区已满或是回车符，则发送数据
    if (ch == '\n' || index >= DEBUG_UART_TX_BUFFER_SIZE - 1 || ch == '\r')
    {
        // 等待上一次DMA传输完成
        while (printf_busy) {
            // 等待DMA传输完成
        }
        
        // 标记正在发送
        printf_busy = 1;
        
        // 使用DMA发送数据
        HAL_UART_Transmit_DMA(&DEBUG_UART, printf_buffer, index);
        
        // 重置索引
        index = 0;
    }
    
    return ch;
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
    // 当接收到数据时
    if(Size > 0)
    {
      uart_rx_len = Size;
      uart_rx_done_flag = 1; // 设置接收完成标志
      
      // 确保字符串以0结尾，避免缓冲区溢出
      // 如果Size等于缓冲区大小，则不能在Size处写入，因为会超出数组边界
      if (Size < DEBUG_UART_RX_BUFFER_SIZE) {
          uart_rx_buffer[Size] = 0;
      } else {
          // 如果接收到的数据刚好填满缓冲区，则在最后一个有效字符后添加空字符
          uart_rx_buffer[DEBUG_UART_RX_BUFFER_SIZE - 1] = 0;
      }
      
      // 在接收到数据后，DMA接收会自动停止。
      // 重新启动DMA接收的任务应该交给上层应用（如Test_ProcessCommand）在处理完数据后进行，
      // 避免在中断中重复启动DMA导致状态混乱。
    }
    else // 如果没有接收到数据（空闲中断但无数据），则重新启动接收
    {
      USB_UART_ReceiveToIdle_DMA();
    }
  }
}
