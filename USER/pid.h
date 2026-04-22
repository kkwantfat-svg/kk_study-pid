#ifndef __PID_H
#define __PID_H
#include <stdint.h>

typedef struct {
    float Kp;           // 比例系数
    float Ki;            // 积分系数
    float Kd;           // 微分系数

    float target;       // 目标值
    float output;       // 输出值
    float actual;       // 实际值

    float integral;     // 积分累计值
    float prev_error;   // 上一次误差值
    float error;        // 当前误差值

    float output_limit_max; // 输出上限
    float output_limit_min; // 输出下限

    float integral_limit_max; // 积分上限
    float integral_limit_min; // 积分下限

} PID_Controller;

void PID_Update(PID_Controller *pid);

#endif
