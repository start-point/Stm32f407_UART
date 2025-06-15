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

/* 外部变量 */
extern UART_HandleTypeDef DEBUG_UART;
extern uint8_t uart_rx_buffer[DEBUG_UART_RX_BUFFER_SIZE];
extern uint8_t uart_tx_buffer[DEBUG_UART_TX_BUFFER_SIZE];
extern volatile uint8_t uart_rx_done_flag;
extern volatile uint16_t uart_rx_len;

/* 函数声明 */
void USB_UART_Init(void);

/* 发送函数 */
void USB_UART_Transmit(uint8_t *data, uint16_t size);

/* 接收函数 */
void USB_UART_ReceiveToIdle_DMA(void);
uint16_t USB_UART_GetRxLen(void);
void USB_UART_ClearRxBuffer(void);

#ifdef __cplusplus
}
#endif

#endif /* __USB_UART_H */ 