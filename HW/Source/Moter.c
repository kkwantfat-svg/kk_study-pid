#include "stm32f10x_gpio.h"
#include "pwm.h"

/**
 * 函数名: Moter_Init
 * 描述: 电机初始化函数
 */
void Moter_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;  // 使用PB12和PB13控制电机
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    PWM_Init(); // 初始化PWM
}

/*
 * 函数名: Moter_SetSpeed_Location
 * 描述: 设置电机速度与位置函数
 * 参数: speed - 电机速度，范围为-100到100，正数表示正转，负数表示反转，0表示停止
 */
void Moter_SetSpeed_Location(int8_t speed)
{
    if(speed >= 0)  // 正转
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_12); // PB12低电平
        GPIO_SetBits(GPIOB, GPIO_Pin_13); // PB13高电平
		PWM_SetDutyCycle(speed); // 设置占空比
    }
    else  // 反转
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_12);  // PB12高电平
        GPIO_ResetBits(GPIOB, GPIO_Pin_13); // PB13低电平
        PWM_SetDutyCycle(-speed); // 设置占空比
    }
}
