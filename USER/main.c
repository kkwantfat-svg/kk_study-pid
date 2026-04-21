#include "stm32f10x.h"                  // Device header
#include "timer.h"
#include "led.h"
#include "Key.h"
#include "oled.h"
#include "usart.h"
#include "RP.h"
#include "Moter.h"
#include "encoder.h"

uint8_t KeyNum;
uint16_t speed = 0;
int16_t encoderCount = 0;
int16_t location = 0;

int main(void)
{
    Timer_Init();
    nvic_config();
    LED_Init();
    Key_Init();
    OLED_Init();
    USART1_Init();
    RP_Init();
    Moter_Init();
    Encoder_Init();
    
    while (1)
    {
        OLED_Clear();
        KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			speed += 10;
		}
		if (KeyNum == 2)
		{
			speed -= 10;
		}
		if (KeyNum == 3)
		{
			speed = 0;
            location = 0;
		}
        Moter_SetSpeed(speed);
        OLED_Printf(0, 0, OLED_8X16, "encoder: %+d", encoderCount);
        OLED_Printf(0, 16, OLED_8X16, "location: %+d", location);
        OLED_Printf(0, 32, OLED_8X16, "speed: %+d", speed);
        // OLED_Printf(0, 48, OLED_8X16, "%d", RP_GetValue(4));
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
        if (counter >= 100) // 每100个更新周期读取一次编码器值
        {
            counter = 0;
            encoderCount = Encoder_GetCount();
            location += encoderCount; // 累加位置
        }
    }
}
