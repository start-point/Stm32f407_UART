#include "test_usb_uart.h"
#include <string.h>

// 测试命令处理函数
void Test_ProcessCommand(void)
{
    if(uart_rx_done_flag)
    {
        /* 确保字符串以0结尾 */
        uart_rx_buffer[uart_rx_len] = 0;
        
        /* Print received data */
        printf("Received data: %s\r\n", uart_rx_buffer);
        printf("Data length: %d\r\n", uart_rx_len);
        
        /* 如果接收到"help"命令 */
        if(strstr((char*)uart_rx_buffer, "help"))
        {
            printf("Available commands:\r\n");
            printf("help - Display help information\r\n");
            printf("info - Display system information\r\n");
            printf("echo [text] - Echo text\r\n");
        }
        /* 如果接收到"info"命令 */
        else if(strstr((char*)uart_rx_buffer, "info"))
        {
            Test_ShowSystemInfo();
        }
        /* 如果接收到"echo"命令 */
        else if(strstr((char*)uart_rx_buffer, "echo"))
        {
            char *text = strstr((char*)uart_rx_buffer, "echo") + 5; // 跳过"echo "
            Test_EchoCommand(text);
        }
        
        /* 清空接收缓冲区 */
        USB_UART_ClearRxBuffer();
        
        /* 重新启动接收 */
        USB_UART_ReceiveToIdle_DMA();
    }
}

// 测试系统信息显示
void Test_ShowSystemInfo(void)
{
    printf("System Info:\r\n");
    printf("Chip Model: STM32F4xx\r\n");
    printf("System Clock: %d MHz\r\n", HAL_RCC_GetSysClockFreq() / 1000000);
    printf("Compile Time: %s %s\r\n", __DATE__, __TIME__);
}

// 测试回显功能
void Test_EchoCommand(char *text)
{
    printf("Echo: %s\r\n", text);
}

// 测试运行时间显示
void Test_ShowRunTime(void)
{
    static uint32_t last_time = 0;
    if(HAL_GetTick() - last_time >= 1000)
    {
        last_time = HAL_GetTick();
        printf("\t\tSystem Run Time: %d ms\r\n", HAL_GetTick());
    }
}

// 主测试函数
void USB_UART_Test(void)
{
    // 初始化USB UART
    USB_UART_Init();
    
    // 打印欢迎信息
    printf("Welcome to serial port debugging\r\n");
    printf("System Clock Frequency: %d Hz\r\n", HAL_RCC_GetSysClockFreq());
        
    // 主测试循环
    while(1)
    {
        // 处理命令
        Test_ProcessCommand();
        
        // 显示运行时间
        Test_ShowRunTime();
    }
}
