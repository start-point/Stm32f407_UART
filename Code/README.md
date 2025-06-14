# ���ڵ��Թ���

��Ŀ¼������ʹ�ô���4��UART4��ʵ�ֵĴ��ڵ��Թ��ܣ�ʹ��DMA��ʽ�ӿ��д������д��printf�����Է�����á�

## �ļ�˵��

- `usb_uart.h`: ���ڵ��Թ���ͷ�ļ����������������ͺ궨��
- `usb_uart.c`: ���ڵ��Թ���ʵ���ļ����������й���ʵ��

## �����ص�

1. ʹ��UART4��Ϊ���Դ���
2. ʹ��DMA��ʽ�շ����ݣ��������Ч��
3. ��дprintf����ʹ��DMA���٣�������������ٶ�
4. ֧�ִ��������������
5. ֧�ֿ����жϼ�⣬�Զ����ղ���������
6. ģ�黯��ƣ����д�����ع��ܣ�����HAL�ص���������usb_uart.c/h�ļ���
7. ͨ�������������Ż�printf���ܣ���������ʱ��

## �����ӵ���Ŀ��

### ����Keil MDK�û�

1. ��Keil MDK�д���Ŀ
2. �Ҽ������Ŀ��ѡ��"Add New Group"������һ����Ϊ"Code"����
3. �Ҽ����"Code"�飬ѡ��"Add Existing Files to Group 'Code'"
4. ѡ��`usb_uart.c`��`usb_uart.h`�ļ�
5. ȷ���ڱ���ѡ���а�����CodeĿ¼��·��

### ����STM32CubeIDE�û�

1. ��STM32CubeIDE�д���Ŀ
2. �Ҽ������Ŀ��ѡ��"New > Folder"������һ����Ϊ"Code"���ļ���
3. ��`usb_uart.c`��`usb_uart.h`�ļ����Ƶ����ļ�����
4. �Ҽ������Ŀ��ѡ��"Properties > C/C++ Build > Settings > Tool Settings > MCU GCC Compiler > Include paths"
5. ���"../Code"������·����

## ʹ�÷���

1. ����Ҫʹ�ô��ڵ��Ե��ļ��а���ͷ�ļ���
```c
#include "usb_uart.h"
```

2. ��main������ʼ�����ֵ��ó�ʼ��������
```c
USB_UART_Init();
```

3. ʹ�ñ�׼printf�������Զ��庯���������ݣ�
```c
printf("Hello World!\r\n");
USB_UART_Printf("System clock: %d Hz\r\n", HAL_RCC_GetSysClockFreq());
```

4. �������ݴ���
```c
if(uart_rx_done_flag)
{
    // ������յ�������
    printf("Received: %s\r\n", uart_rx_buffer);
    
    // ��ս��ջ�����
    USB_UART_ClearRxBuffer();
}
```

## ע������

1. ������Ĭ������Ϊ115200
2. ȷ����ʹ��ǰ����ȷ��ʼ��UART4
3. ���ջ�������СΪ256�ֽڣ����ͻ�������СΪ256�ֽ�
4. �����޸Ļ�������С�����޸�usb_uart.h�еĺ궨�� 