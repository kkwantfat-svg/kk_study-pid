#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

/* PWM 初始化 */
void PWM_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // 设置自动重装载值 ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1; // 设置预分频器
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // 初始占空比为0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);


    TIM_CtrlPWMOutputs(TIM2, ENABLE); // 使能主输出
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief  设置PWM占空比
 * @param pulse 占空比值 (0-100)
 * @retval 无
 */
void PWM_SetDutyCycle(uint8_t pulse)
{
    if (pulse > 100) pulse = 100; // 限制占空比在0-100范围内
    TIM_SetCompare1(TIM2, pulse);
}
