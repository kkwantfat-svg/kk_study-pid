#include "stm32f10x_gpio.h"

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // Enable clock for GPIOC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    // Configure PC13 as push-pull output
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // Set PC13 high (LED off)
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void LED_ON(void)
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_13); // Set PC13 low to turn on LED
}

void LED_OFF(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_13); // Set PC13 high to turn off LED
}

void LED_Toggle(void)
{
    GPIOC->ODR ^= GPIO_Pin_13; // Toggle PC13
}
