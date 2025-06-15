#ifndef __TEST_USB_UART_H
#define __TEST_USB_UART_H

#include "../usb_uart.h"
#include "main.h"

// 测试命令处理函数
void Test_ProcessCommand(void);

// 测试系统信息显示
void Test_ShowSystemInfo(void);

// 测试回显功能
void Test_EchoCommand(char *text);

// 测试运行时间显示
void Test_ShowRunTime(void);

// 主测试函数
void USB_UART_Test(void);

#endif /* __TEST_USB_UART_H */