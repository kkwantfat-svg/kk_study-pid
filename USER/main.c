#include "stm32f10x.h"                  // Device header
#include "timer.h"
#include "led.h"
#include "Key.h"
#include "oled.h"
#include "usart.h"

uint8_t KeyNum;
uint16_t target = 0;

int main(void)
{
    Timer_Init();
    nvic_config();
    LED_Init();
    Key_Init();
    OLED_Init();
    USART1_Init();

    U1_printf("USART1 initialized\r\n");
    
    
    while (1)
    {
        KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			target += 10;
		}
		if (KeyNum == 2)
		{
			target -= 10;
		}
		if (KeyNum == 3)
		{
			target = 0;
		}
        OLED_Clear();
        OLED_Printf(0, 0, OLED_6X8, "Key: %d", target);
        OLED_Update();
	}	
}

void TIM1_UP_IRQHandler(void)
{
    static uint16_t counter = 0;
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        // Clear the update interrupt flag
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        
        // User code to handle timer update event
        Key_Tick();
        counter++;
    }
}
