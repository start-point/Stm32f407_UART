#include "test_usb_uart.h"
#include <string.h>

// �����������
void Test_ProcessCommand(void)
{
    if(uart_rx_done_flag)
    {
        /* ȷ���ַ�����0��β */
        uart_rx_buffer[uart_rx_len] = 0;
        
        /* ��ӡ���յ������� */
        printf("���յ�����: %s\r\n", uart_rx_buffer);
        printf("���ݳ���: %d\r\n", uart_rx_len);
        
        /* ������յ�"help"���� */
        if(strstr((char*)uart_rx_buffer, "help"))
        {
            printf("���������б�:\r\n");
            printf("help - ��ʾ������Ϣ\r\n");
            printf("info - ��ʾϵͳ��Ϣ\r\n");
            printf("echo [�ı�] - �����ı�\r\n");
        }
        /* ������յ�"info"���� */
        else if(strstr((char*)uart_rx_buffer, "info"))
        {
            Test_ShowSystemInfo();
        }
        /* ������յ�"echo"���� */
        else if(strstr((char*)uart_rx_buffer, "echo"))
        {
            char *text = strstr((char*)uart_rx_buffer, "echo") + 5; // ����"echo "
            Test_EchoCommand(text);
        }
        
        /* ��ս��ջ����� */
        USB_UART_ClearRxBuffer();
        
        /* ������������ */
        USB_UART_ReceiveToIdle_DMA();
    }
}

// ����ϵͳ��Ϣ��ʾ
void Test_ShowSystemInfo(void)
{
    printf("ϵͳ��Ϣ:\r\n");
    printf("оƬ�ͺ�: STM32F4xx\r\n");
    printf("ϵͳʱ��: %d MHz\r\n", HAL_RCC_GetSysClockFreq() / 1000000);
    printf("����ʱ��: %s %s\r\n", __DATE__, __TIME__);
}

// ���Ի��Թ���
void Test_EchoCommand(char *text)
{
    printf("����: %s\r\n", text);
}

// ��������ʱ����ʾ
void Test_ShowRunTime(void)
{
    static uint32_t last_time = 0;
    if(HAL_GetTick() - last_time >= 1000)
    {
        last_time = HAL_GetTick();
        printf("ϵͳ����ʱ��: %d ms\r\n", HAL_GetTick());
    }
}

// �����Ժ���
void USB_UART_Test(void)
{
    // ��ʼ��USB UART
    USB_UART_Init();
    
    // ��ӡ��ӭ��Ϣ
    printf("��ӭʹ�ô��ڵ��Թ���\r\n");
    printf("ϵͳʱ��Ƶ��: %d Hz\r\n", HAL_RCC_GetSysClockFreq());
    
    // ������ѭ��
    while(1)
    {
        // ��������
        Test_ProcessCommand();
        
        // ��ʾ����ʱ��
        Test_ShowRunTime();
    }
}