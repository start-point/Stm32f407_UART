/**
 * @file    usb_uart.h
 * @brief   串口调试功能头文件
 */

#ifndef __USB_UART_H
#define __USB_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* 包含头文件 */
#include "main.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/* 宏定义 */
#define DEBUG_UART                  huart4
#define DEBUG_UART_HANDLE           UART4
#define DEBUG_UART_RX_BUFFER_SIZE   256
#define DEBUG_UART_TX_BUFFER_SIZE   256

/* 外部变量声明 */
extern UART_HandleTypeDef DEBUG_UART;
extern uint8_t uart_rx_buffer[DEBUG_UART_RX_BUFFER_SIZE];
extern uint8_t uart_tx_buffer[DEBUG_UART_TX_BUFFER_SIZE];
extern volatile uint8_t uart_rx_done_flag;
extern volatile uint16_t uart_rx_len;

/* 函数声明 */
void USB_UART_Init(void);
void USB_UART_DeInit(void);

/* 内部使用的函数 */
void USB_UART_RxCpltCallback(void);
void USB_UART_TxCpltCallback(void);
void USB_UART_ErrorCallback(void);
void USB_UART_IdleCallback(void);

/* 发送函数 */
void USB_UART_Transmit(uint8_t *data, uint16_t size);
void USB_UART_TransmitStr(char *str);
void USB_UART_Printf(const char *format, ...);

/* 接收函数 */
void USB_UART_ReceiveToIdle_DMA(void);
uint16_t USB_UART_GetRxLen(void);
void USB_UART_ClearRxBuffer(void);

/* printf重定向相关函数 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __USB_UART_H */ 