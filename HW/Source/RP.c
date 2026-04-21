#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"

/** @brief  初始化RP传感器
  * @param  None
  * @retval None
  */
void RP_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADC时钟配置，72MHz/6=12MHz，满足ADC时钟不超过14MHz的要求

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;         // 单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;   // 单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // 软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC2, &ADC_InitStructure);
    
    ADC_Cmd(ADC2, ENABLE);
    
    // 进行ADC校准
    ADC_ResetCalibration(ADC2);
    while (ADC_GetResetCalibrationStatus(ADC2));
    
    ADC_StartCalibration(ADC2);
    while (ADC_GetCalibrationStatus(ADC2));
}

/** @brief  获取RP传感器的ADC值
  * @param  n: 通道号 (1-4)
  * @retval ADC转换值
  */
uint16_t RP_GetValue(uint8_t n)
{
	if (n == 1)
	{
		ADC_RegularChannelConfig(ADC2, ADC_Channel_2, 1, ADC_SampleTime_55Cycles5);
	}
	else if (n == 2)
	{
		ADC_RegularChannelConfig(ADC2, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);
	}
	else if (n == 3)
	{
		ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
	}
	else if (n == 4)
	{
		ADC_RegularChannelConfig(ADC2, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5);
	}
	
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);
	while (ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC2);
}
