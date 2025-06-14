/**
 * @file    usb_uart.h
 * @brief   ���ڵ��Թ���ͷ�ļ�
 */

#ifndef __USB_UART_H
#define __USB_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* ����ͷ�ļ� */
#include "main.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/* �궨�� */
#define DEBUG_UART                  huart4
#define DEBUG_UART_HANDLE           UART4
#define DEBUG_UART_RX_BUFFER_SIZE   256
#define DEBUG_UART_TX_BUFFER_SIZE   256

/* �ⲿ�������� */
extern UART_HandleTypeDef DEBUG_UART;
extern uint8_t uart_rx_buffer[DEBUG_UART_RX_BUFFER_SIZE];
extern uint8_t uart_tx_buffer[DEBUG_UART_TX_BUFFER_SIZE];
extern volatile uint8_t uart_rx_done_flag;
extern volatile uint16_t uart_rx_len;

/* �������� */
void USB_UART_Init(void);
void USB_UART_DeInit(void);

/* �ڲ�ʹ�õĺ��� */
void USB_UART_RxCpltCallback(void);
void USB_UART_TxCpltCallback(void);
void USB_UART_ErrorCallback(void);
void USB_UART_IdleCallback(void);

/* ���ͺ��� */
void USB_UART_Transmit(uint8_t *data, uint16_t size);
void USB_UART_TransmitStr(char *str);
void USB_UART_Printf(const char *format, ...);

/* ���պ��� */
void USB_UART_ReceiveToIdle_DMA(void);
uint16_t USB_UART_GetRxLen(void);
void USB_UART_ClearRxBuffer(void);

/* printf�ض�����غ��� */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __USB_UART_H */ 