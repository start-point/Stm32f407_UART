# 串口调试功能

本目录包含了使用串口4（UART4）实现的串口调试功能，使用DMA方式加快读写，并重写了printf函数以方便调用。

## 文件说明

- `usb_uart.h`: 串口调试功能头文件，包含函数声明和宏定义
- `usb_uart.c`: 串口调试功能实现文件，包含所有功能实现

## 功能特点

1. 使用UART4作为调试串口
2. 使用DMA方式收发数据，显著提高效率
3. 重写printf函数使用DMA加速，大幅提升调试速度
4. 支持串口命令解析功能
5. 支持空闲中断检测，自动接收不定长数据
6. 模块化设计，所有串口相关功能（包括HAL回调）集中在usb_uart.c/h文件中
7. 通过缓冲区管理优化printf性能，减少阻塞时间

## 如何添加到项目中

### 对于Keil MDK用户

1. 在Keil MDK中打开项目
2. 右键点击项目，选择"Add New Group"，创建一个名为"Code"的组
3. 右键点击"Code"组，选择"Add Existing Files to Group 'Code'"
4. 选择`usb_uart.c`和`usb_uart.h`文件
5. 确保在编译选项中包含了Code目录的路径

### 对于STM32CubeIDE用户

1. 在STM32CubeIDE中打开项目
2. 右键点击项目，选择"New > Folder"，创建一个名为"Code"的文件夹
3. 将`usb_uart.c`和`usb_uart.h`文件复制到该文件夹中
4. 右键点击项目，选择"Properties > C/C++ Build > Settings > Tool Settings > MCU GCC Compiler > Include paths"
5. 添加"../Code"到包含路径中

## 使用方法

1. 在需要使用串口调试的文件中包含头文件：
```c
#include "usb_uart.h"
```

2. 在main函数初始化部分调用初始化函数：
```c
USB_UART_Init();
```

3. 使用标准printf函数或自定义函数发送数据：
```c
printf("Hello World!\r\n");
USB_UART_Printf("System clock: %d Hz\r\n", HAL_RCC_GetSysClockFreq());
```

4. 接收数据处理：
```c
if(uart_rx_done_flag)
{
    // 处理接收到的数据
    printf("Received: %s\r\n", uart_rx_buffer);
    
    // 清空接收缓冲区
    USB_UART_ClearRxBuffer();
}
```

## 注意事项

1. 波特率默认设置为115200
2. 确保在使用前已正确初始化UART4
3. 接收缓冲区大小为256字节，发送缓冲区大小为256字节
4. 如需修改缓冲区大小，请修改usb_uart.h中的宏定义 