#include "stm32f10x.h"                  // Device header
#include "timer.h"
#include "led.h"
#include "Key.h"
#include "oled.h"

uint8_t KeyNum;
uint16_t target = 0;

int main(void)
{
    Timer_Init();
    nvic_config();
    LED_Init();
    Key_Init();
    OLED_Init();


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
        if (counter >= 1000) // Toggle LED every 1 second
        {

            LED_Toggle();
            counter = 0;
        }
    }
}
