#ifndef __TEST_USB_UART_H
#define __TEST_USB_UART_H

#include "../usb_uart.h"
#include "main.h"

// �����������
void Test_ProcessCommand(void);

// ����ϵͳ��Ϣ��ʾ
void Test_ShowSystemInfo(void);

// ���Ի��Թ���
void Test_EchoCommand(char *text);

// ��������ʱ����ʾ
void Test_ShowRunTime(void);

// �����Ժ���
void USB_UART_Test(void);

#endif /* __TEST_USB_UART_H */