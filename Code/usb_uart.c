/**
 * @file    usb_uart.c
 * @brief   ���ڵ��Թ���ʵ��
 */

#include "usb_uart.h"

/* ˽�б������� */
uint8_t uart_rx_buffer[DEBUG_UART_RX_BUFFER_SIZE];
uint8_t uart_tx_buffer[DEBUG_UART_TX_BUFFER_SIZE];
volatile uint8_t uart_rx_done_flag = 0;
volatile uint16_t uart_rx_len = 0;
static uint8_t printf_buffer[DEBUG_UART_TX_BUFFER_SIZE];
static volatile uint8_t printf_busy = 0;

/**
 * @brief  ��ʼ�����ڵ��Թ���
 * @param  ��
 * @retval ��
 */
void USB_UART_Init(void)
{
    /* ��������MX_UART4_Init�г�ʼ�� */
    
    /* ����DMA���� */
    USB_UART_ReceiveToIdle_DMA();
    
    /* ʹ�ܴ��ڿ����ж� */
    __HAL_UART_ENABLE_IT(&DEBUG_UART, UART_IT_IDLE);
}

/**
 * @brief  ����ʼ�����ڵ��Թ���
 * @param  ��
 * @retval ��
 */
void USB_UART_DeInit(void)
{
    /* ֹͣDMA���� */
    HAL_UART_DMAStop(&DEBUG_UART);
    
    /* ���ô��ڿ����ж� */
    __HAL_UART_DISABLE_IT(&DEBUG_UART, UART_IT_IDLE);
}

/**
 * @brief  ʹ��DMA��������ֱ������
 * @param  ��
 * @retval ��
 */
void USB_UART_ReceiveToIdle_DMA(void)
{
    /* ��ս��ջ����� */
    memset(uart_rx_buffer, 0, DEBUG_UART_RX_BUFFER_SIZE);
    
    /* ����DMA���� */
    HAL_UARTEx_ReceiveToIdle_DMA(&DEBUG_UART, uart_rx_buffer, DEBUG_UART_RX_BUFFER_SIZE);
    
    /* ���ð봫���ж� */
    __HAL_DMA_DISABLE_IT(DEBUG_UART.hdmarx, DMA_IT_HT);
    
    /* ȷ����ʼ״̬�½��ձ�־Ϊ0 */
    uart_rx_done_flag = 0;
}

/**
 * @brief  ��ȡ�������ݳ���
 * @param  ��
 * @retval �������ݳ���
 */
uint16_t USB_UART_GetRxLen(void)
{
    return uart_rx_len;
}

/**
 * @brief  ��ս��ջ�����
 * @param  ��
 * @retval ��
 */
void USB_UART_ClearRxBuffer(void)
{
    memset(uart_rx_buffer, 0, DEBUG_UART_RX_BUFFER_SIZE);
    uart_rx_len = 0;
    uart_rx_done_flag = 0;
}

/**
 * @brief  ʹ��DMA��������
 * @param  data: Ҫ���͵�����
 * @param  size: ���ݴ�С
 * @retval ��
 */
void USB_UART_Transmit(uint8_t *data, uint16_t size)
{
    HAL_UART_Transmit_DMA(&DEBUG_UART, data, size);
}

/**
 * @brief  �����ַ���
 * @param  str: Ҫ���͵��ַ���
 * @retval ��
 */
void USB_UART_TransmitStr(char *str)
{
    USB_UART_Transmit((uint8_t *)str, strlen(str));
}

/**
 * @brief  ��ʽ����ӡ������DMA���٣�
 * @param  format: ��ʽ�ַ���
 * @param  ...: �ɱ����
 * @retval ��
 */
void USB_UART_Printf(const char *format, ...)
{
    va_list args;
    uint32_t length;
    
    /* �ȴ���һ��DMA������� */
    while (printf_busy) {
        /* �ȴ�DMA������� */
    }
    
    va_start(args, format);
    length = vsnprintf((char *)uart_tx_buffer, DEBUG_UART_TX_BUFFER_SIZE, format, args);
    va_end(args);
    
    /* ������ڷ��� */
    printf_busy = 1;
    
    /* ʹ��DMA�������� */
    HAL_UART_Transmit_DMA(&DEBUG_UART, uart_tx_buffer, length);
}

/**
 * @brief  ������ɴ�����(�ڲ�ʹ��)
 * @param  ��
 * @retval ��
 */
void USB_UART_RxCpltCallback(void)
{
    uart_rx_done_flag = 1;
    
    /* ��ӡ������Ϣ */
    printf("USB_UART_RxCpltCallback: �������\r\n");
}

/**
 * @brief  ������ɴ�����(�ڲ�ʹ��)
 * @param  ��
 * @retval ��
 */
void USB_UART_TxCpltCallback(void)
{
    /* ������ɺ�Ĵ��� */
}

/**
 * @brief  ��������(�ڲ�ʹ��)
 * @param  ��
 * @retval ��
 */
void USB_UART_ErrorCallback(void)
{
    /* ������ */
    USB_UART_ReceiveToIdle_DMA();
}

/**
 * @brief  ���лص�����
 * @param  ��
 * @retval ��
 */
void USB_UART_IdleCallback(void)
{
    /* ������յ������ݳ��� */
    uart_rx_len = DEBUG_UART_RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(DEBUG_UART.hdmarx);
    
    /* ֻ�е����յ�����ʱ�����ñ�־ */
    if(uart_rx_len > 0)
    {
        /* ���ý�����ɱ�־ */
        uart_rx_done_flag = 1;
        
        /* ֹͣDMA���� */
        HAL_UART_DMAStop(&DEBUG_UART);
        
        /* ȷ���ַ�����0��β */
        uart_rx_buffer[uart_rx_len] = 0;
    }
    else
    {
        /* ���û�н��յ����ݣ������������� */
        USB_UART_ReceiveToIdle_DMA();
    }
}

/* printf�ض�����غ��� */

/* �ض���printf���� - ʹ��DMA���� */
#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif
{
    static uint16_t index = 0;
    
    /* ���ַ����뻺���� */
    printf_buffer[index++] = (uint8_t)ch;
    
    /* ����ǻ��з��򻺳����������ǻس������������� */
    if (ch == '\n' || index >= DEBUG_UART_TX_BUFFER_SIZE - 1 || ch == '\r')
    {
        /* �ȴ���һ��DMA������� */
        while (printf_busy) {
            /* �ȴ�DMA������� */
        }
        
        /* ������ڷ��� */
        printf_busy = 1;
        
        /* ʹ��DMA�������� */
        HAL_UART_Transmit_DMA(&DEBUG_UART, printf_buffer, index);
        
        /* �������� */
        index = 0;
    }
    
    return ch;
}

/* �ض���scanf���� */
#ifdef __GNUC__
int __io_getchar(void)
#else
int fgetc(FILE *f)
#endif
{
    uint8_t ch = 0;
    /* ���յ����ַ� */
    HAL_UART_Receive(&DEBUG_UART, &ch, 1, 0xFFFF);
    return ch;
}

/**
 * @brief  ���ڽ�����ɻص�����(HAL��ص�)
 * @param  huart: ���ھ��
 * @retval ��
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == DEBUG_UART_HANDLE)
  {
    uart_rx_done_flag = 1;
    
    /* ��ӡ������Ϣ */
    printf("HAL_UART_RxCpltCallback: �������\r\n");
    
    /* ��������DMA���� */
    USB_UART_ReceiveToIdle_DMA();
  }
}

/**
 * @brief  ���ڷ�����ɻص�����(HAL��ص�)
 * @param  huart: ���ھ��
 * @retval ��
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == DEBUG_UART_HANDLE)
  {
    /* ��Ƿ������ */
    printf_busy = 0;
  }
}

/**
 * @brief  ���ڴ���ص�����(HAL��ص�)
 * @param  huart: ���ھ��
 * @retval ��
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == DEBUG_UART_HANDLE)
  {
    /* ������ */
    USB_UART_ReceiveToIdle_DMA();
  }
}

/**
  * @brief  ���ڽ��յ������жϻص�����(HAL��ص�)
  * @param  huart: ���ھ��
  * @param  Size: ���յ������ݴ�С
  * @retval ��
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if(huart->Instance == DEBUG_UART_HANDLE)
  {
    /* ֻ�е����յ�����ʱ�����ñ�־ */
    if(Size > 0)
    {
      uart_rx_len = Size;
      uart_rx_done_flag = 1;
      
      /* ȷ���ַ�����0��β */
      uart_rx_buffer[Size] = 0;
      
      /* ��ӡ������Ϣ */
      printf("HAL_UARTEx_RxEventCallback: ���յ� %d �ֽ�����\r\n", Size);
    }
    else
    {
      /* ���û�н��յ����ݣ������������� */
      USB_UART_ReceiveToIdle_DMA();
    }
  }
} 