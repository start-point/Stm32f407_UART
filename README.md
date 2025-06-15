# STM32F407 UART ������Ӧ��ʾ��

## 1. ��Ŀ���

����Ŀ�ǻ��� STM32F407ZGT6 ΢��������һ������ͨ��ʾ������Ҫʵ����ͨ�� UART4 ���������շ����������� `printf` �ض����ܣ�������е����������Ŀ�ṹ��������ΪӦ�ó����߼����ײ㴮�������Ͳ���ģ�飬����������չ��

## 2. ��Ҫ����

*   **UART4 ����ͨ��**:
    *   ʹ�� STM32 HAL ��� DMA (Direct Memory Access) ʵ�ָ�Ч�Ĵ������ݽ��պͷ��͡�
    *   ֧�ֿ����ж� (Idle Line Detection) ���գ��ܹ��Զ�������ݰ��Ľ�����
    *   ���ջ�������СΪ 256 �ֽڣ����ͻ�������СΪ 256 �ֽڡ�
*   **`printf` �ض���**:
    *   ����׼��� `printf` �����ض��� UART4������ֱ��ͨ���������������Ϣ��
    *   ���� DMA ���� `printf` �������ߵ���Ч�ʡ�
*   **�����н���**:
    *   ͨ�����ڽ����û��������������н����ʹ���
    *   ֧�ֵ����������
        *   `help`: ��ʾ���������б�
        *   `info`: ��ʾϵͳ��Ϣ����оƬ�ͺš�ϵͳʱ��Ƶ�ʺͱ���ʱ�䡣
        *   `echo [text]`: �����û�������ı���
*   **ϵͳ����ʱ����ʾ**:
    *   �����Ե�ͨ���������ϵͳ��ǰ������ʱ�䡣

## 3. ģ��˵��

### 3.1 `Code/app.c` & `Code/app.h`

*   **`app.c`**: ����Ӧ�ó������ڵ����Ҫ�߼���
    *   `APP_Init()`: ��ʼ��ϵͳ�����衣
    *   `APP_Process()`: �����������������������״̬��ʾ��
    *   `APP_Test()`: ����ִ���ض����Թ��ܡ�
*   **`app.h`**: ���� `app.c` �ж���ĺ������ⲿ������

### 3.2 `Code/usb_uart.c` & `Code/usb_uart.h`

*   **`usb_uart.c`**: ʵ���� UART4 �ĵײ������� `printf` �ض����ܡ�
    *   `USB_UART_Init()`: ��ʼ�� UART4������ DMA ���պͿ����жϡ�
    *   `USB_UART_ReceiveToIdle_DMA()`: ���� DMA ���գ�ֱ����⵽���߿��С�
    *   `USB_UART_Transmit(uint8_t *data, uint16_t size)`: ʹ�� DMA �������ݡ�
    *   `USB_UART_GetRxLen()`: ��ȡ��ǰ���յ������ݳ��ȡ�
    *   `USB_UART_ClearRxBuffer()`: ��ս��ջ�������
    *   `__io_putchar` / `fputc`: `printf` �ض����ʵ�֣����ַ�ͨ�� DMA ���͵� UART4��
    *   `HAL_UART_TxCpltCallback()`: UART ������ɻص����������ڹ�����æ��־��
    *   `HAL_UART_ErrorCallback()`: UART ����ص����������ڴ�����������������ա�
    *   `HAL_UARTEx_RxEventCallback()`: UART �����¼��ص������������жϣ���������յ������ݡ�
*   **`usb_uart.h`**: ���� `usb_uart.c` �ж���ĺ������궨����ⲿ�������� `DEBUG_UART`��`DEBUG_UART_RX_BUFFER_SIZE`��`uart_rx_buffer` �ȡ�

### 3.3 `Code/Test/test_usb_uart.c` & `Code/Test/test_usb_uart.h`

*   **`test_usb_uart.c`**: �����˴��ڵ��Թ��ܵĲ���ʵ�֡�
    *   `Test_ProcessCommand()`: ��������ʹ���ͨ�����ڽ��յ�������� `help`��`info`��`echo`��
    *   `Test_ShowSystemInfo()`: ��ʾϵͳ�����Ϣ��
    *   `Test_EchoCommand(char *text)`: ʵ�ּ򵥵��ı����Թ��ܡ�
    *   `Test_ShowRunTime()`: �����Ե���ʾϵͳ����ʱ�䡣
    *   `USB_UART_Test()`: ����һ��������ѭ�������������������ʾ����ʱ�䡣
*   **`test_usb_uart.h`**: ���� `test_usb_uart.c` �ж���Ĳ�����غ�����

## 4. ʹ�÷���

1.  **Ӳ������**: ȷ�� STM32F407 ������� UART4 (PA0/PA1) ���ӵ� USB ת����ģ����������
2.  **��������¼**: ʹ�� Keil MDK ���������ݵ� IDE ������Ŀ�������̼���¼�� STM32F407 ΢��������
3.  **���ڵ���**:
    *   �򿪴��ڵ������֣����ò����ʣ����� `main.c` �� STM32CubeMX ���ã���ͨ��Ϊ 115200��
    *   ͨ���������ַ�������� `help`��`info`��`echo your_text`������Ŀ��ͨ�� `APP_Process()` �е� `Test_ProcessCommand()` ���д������ؽ����
    *   ͨ�� `printf` ��������ĵ�����Ϣ��ϵͳ����ʱ�佫�ڴ�����������ʾ��

## 5. ע������

*   ����Ŀ�� UART ���ã��粨���ʡ�����λ��ֹͣλ��У��λ��Ӧ�� `main.c` �� STM32CubeMX ���ɵ����ñ���һ�¡�
*   ��ʵ��Ӧ���У�Ӧ���ݾ���������� `DEBUG_UART_RX_BUFFER_SIZE` �� `DEBUG_UART_TX_BUFFER_SIZE` �Ĵ�С��

## 6. ���Խ��
![alt text](image.png)