#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include <stdarg.h>
#include <stdio.h>

/**
 * @brief  USART1 初始化函数
 * @param  None
 * @retval None
 */
void USART1_Init(void)
{
    /*开启GPIOA和USART1的时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    /*配置USART1的TX引脚为复用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /*配置USART1的RX引脚为输入浮空*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;	//波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Tx;	//发送模式
    USART_InitStructure.USART_Parity = USART_Parity_No;	//无奇偶校验位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	//1位停止位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//8位数据位
    USART_Init(USART1, &USART_InitStructure);
    
    USART_Cmd(USART1, ENABLE);	//使能USART1
}

/**
 * @brief  USART1 printf函数
 * @param  format: 格式化字符串
 * @param  ...: 可变参数
 * @retval None
 */
void U1_printf(const char* format, ...)
{
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf((char *)buffer, sizeof(buffer), format, args);
    va_end(args);
    
    for (char* p = buffer; *p != '\0'; p++)
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待发送缓冲区空
        USART_SendData(USART1, *p);	//发送一个字符
    }
	while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));  // 等待发送完成
}
