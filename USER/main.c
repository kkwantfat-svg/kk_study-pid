#include "stm32f10x.h"                  // Device header
#include "timer.h"
#include "led.h"

int main(void)
{
    Timer_Init();
    nvic_config();
    LED_Init();

    while (1)
    {

	}	
}

void TIM1_UP_IRQHandler(void)
{
    static uint16_t led_state = 0;
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        // Clear the update interrupt flag
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        
        // User code to handle timer update event
        led_state = !led_state;
        if (led_state)
        {
            LED_ON();
        }
        else
        {
            LED_OFF();
        }
    }
}
