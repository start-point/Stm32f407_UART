#include "app.h"
#include "Test/test_usb_uart.h"

void APP_Init(void)
{
    // 初始化USB UART
    USB_UART_Init();

}

void APP_Process(void)
{
    // 处理USB UART命令
    Test_ProcessCommand();
    
    // 显示运行时间
    Test_ShowRunTime();
}

void APP_Test(void)
{
    // 测试USB UART功能
    USB_UART_Test();
}