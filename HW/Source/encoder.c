#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

/*
 * 函数名：Encoder_Init
 * 描述 ：编码器初始化
 */
void Encoder_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  // 使用PA6和PA7作为编码器输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period = 0xFFFE; // 设置自动重装载值 ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0; // 不分频
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);

    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 捕获上升沿
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 直接连接到TI
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 不分频
    TIM_ICInitStructure.TIM_ICFilter = 0xF; // 滤波
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // 配置TI1
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 0xF; // 滤波
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; // 配置TI2
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);  // 配置编码器接口，使用TI1和TI2作为输入
    
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * 函数名：Encoder_GetCount
 * 描述 ：获取编码器计数值(在中断中获取)，每次读取后清零
 */
int16_t Encoder_GetCount(void)
{
    int16_t count = (int16_t)TIM_GetCounter(TIM3); // 获取当前计数值
    TIM_SetCounter(TIM3, 0); // 读取后清零
    return count;
}
