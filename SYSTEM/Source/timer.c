#include "stm32f10x_tim.h"

/**
 * @brief  Initializes TIM1 to generate update interrupts every 1ms
 * @param  None
 * @retval None
 */
void Timer_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    // Enable clock for TIM1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    // Configure TIM1 for internal clock
    TIM_InternalClockConfig(TIM1);

    TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 1ms
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 72MHz / 72 = 1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    // Clear update flag and start TIM1
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);

    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    
    TIM_Cmd(TIM1, ENABLE);
}

/**
 * @brief  Configures the NVIC for TIM1 update interrupt
 * @param  None
 * @retval None
 */
void nvic_config(void)
{
    // Set priority grouping
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // Configure NVIC for TIM1 update interrupt
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);
}

/** 
 * @brief  TIM1 update interrupt handler
 * @param  None
 * @retval None
 */
/*
void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        // Clear the update interrupt flag
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        
        // User code to handle timer update event
    }
}
*/
