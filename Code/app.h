#ifndef __APP_H
#define __APP_H

#include "usb_uart.h"

// 应用程序初始化
void APP_Init(void);

// 应用程序主循环
void APP_Process(void);

// 应用程序测试函数
void APP_Test(void);

#endif /* __APP_H */ 