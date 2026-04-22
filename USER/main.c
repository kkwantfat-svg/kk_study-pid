#include "stm32f10x.h"                  // Device header
#include "timer.h"
#include "led.h"
#include "Key.h"
#include "oled.h"
#include "usart.h"
#include "RP.h"
#include "Moter.h"
#include "encoder.h"
#include "pid.h"    

uint8_t KeyNum;
uint16_t speed = 0;

#define    STOP_SPEED   1.0f

// PID_Controller pidspeedcontroller = {
//     .Kp = 0.8f,
//     .Ki = 0.34f,
//     .Kd = 0.0f,
//     .target = 0.0f,
//     .output = 0.0f,
//     .actual = 0.0f,
//     .integral = 0.0f,
//     .prev_error = 0.0f,
//     .error = 0.0f,
//     .output_limit_max = 100.0f,
//     .output_limit_min = -100.0f,
//     .integral_limit_max = 200.0f,
//     .integral_limit_min = -200.0f

// };

PID_Controller pidlocationcontroller = {
    .Kp = 0.42f,
    .Ki = 0.0f,
    .Kd = 0.13f,
    .target = 0.0f,
    .output = 0.0f,
    .actual = 0.0f,
    .integral = 0.0f,
    .prev_error = 0.0f,
    .error = 0.0f,
    .output_limit_max = 100.0f,
    .output_limit_min = -100.0f,
    .integral_limit_max = 200.0f,
    .integral_limit_min = -200.0f

};

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

    OLED_Printf(0, 0, OLED_8X16, "Location Control");
	OLED_Update();
    
    while (1)
    {
        //OLED_Clear();
        // KeyNum = Key_GetNum();
		// if (KeyNum == 1)
		// {
		// 	speed += 10;
		// }
		// if (KeyNum == 2)
		// {
		// 	speed -= 10;
		// }
		// if (KeyNum == 3)
		// {
		// 	speed = 0;
		// }

        //pidspeedcontroller.target = (float)RP_GetValue(4) / 4095.0f * 200.0f - 100.0f; // 将RP传感器的ADC值转换为目标速度，范围为-100到100
        // pidspeedcontroller.Kp = RP_GetValue(1) / 4095.0f * 2.0f; // 将RP传感器的ADC值转换为Kp，范围为0到2
        // pidspeedcontroller.Ki = RP_GetValue(2) / 4095.0f * 2.0f; // 将RP传感器的ADC值转换为Ki，范围为0到2
        // pidspeedcontroller.Kd = RP_GetValue(3) / 4095.0f * 2.0f; // 将RP传感器的ADC值转换为Kd，范围为0到2

        pidlocationcontroller.target = (float)RP_GetValue(4) / 4095.0f * 816 - 408; // 将RP传感器的ADC值转换为目标位置
        pidlocationcontroller.Kp = RP_GetValue(1) / 4095.0f * 2.0f; // 将RP传感器的ADC值转换为Kp，范围为0到2
        pidlocationcontroller.Ki = RP_GetValue(2) / 4095.0f * 2.0f; // 将RP传感器的ADC值转换为Ki，范围为0到2
        pidlocationcontroller.Kd = RP_GetValue(3) / 4095.0f * 2.0f; // 将RP传感器的ADC值转换为Kd，范围为0到2

		OLED_Printf(0, 16, OLED_8X16, "Kp:%4.2f", pidlocationcontroller.Kp);
		OLED_Printf(0, 32, OLED_8X16, "Ki:%4.2f", pidlocationcontroller.Ki);
		OLED_Printf(0, 48, OLED_8X16, "Kd:%4.2f", pidlocationcontroller.Kd);
		
		OLED_Printf(64, 16, OLED_8X16, "Tar:%+04.0f", pidlocationcontroller.target);
		OLED_Printf(64, 32, OLED_8X16, "Act:%+04.0f", pidlocationcontroller.actual);
		OLED_Printf(64, 48, OLED_8X16, "Out:%+04.0f", pidlocationcontroller.output);

        OLED_Update();
        U1_printf("%f,%f,%f\r\n", pidlocationcontroller.target, pidlocationcontroller.actual, pidlocationcontroller.output);
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
        if (counter >= 40) // 每40个更新周期执行一次pid调控
        {
            counter = 0;
            pidlocationcontroller.actual += (float)Encoder_GetCount();;
            PID_Update(&pidlocationcontroller);
            Moter_SetSpeed_Location((int8_t)pidlocationcontroller.output);
        }
    }
}
