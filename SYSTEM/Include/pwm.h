#ifndef __PWM_H
#define __PWM_H
#include <stdint.h>

void PWM_SetDutyCycle(uint8_t pulse);
void PWM_Init(void);

#endif
