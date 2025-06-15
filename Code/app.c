#include "app.h"
#include "Test/test_usb_uart.h"

void APP_Init(void)
{
    // ��ʼ��USB UART
    USB_UART_Init();

}

void APP_Process(void)
{
    // ����USB UART����
    Test_ProcessCommand();
    
    // ��ʾ����ʱ��
    Test_ShowRunTime();
}

void APP_Test(void)
{
    // ����USB UART����
    USB_UART_Test();
}