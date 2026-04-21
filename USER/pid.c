#include "pid.h"

/**
 * @brief 更新PID控制器
 * @param pid PID控制器指针
 */
void PID_Update(PID_Controller *pid)
{
    // 计算误差
    pid->error = pid->target - pid->actual;

    // 计算积分
    if (pid->Ki != 0.0f) // 只有当积分系数不为0时才计算积分
    {
        pid->integral += pid->error;
    }
    else
    {
        pid->integral = 0.0f; // 如果积分系数为0，重置积分累计值
    }

    // 计算微分
    float derivative = pid->error - pid->prev_error;

    // 计算输出
    pid->output = (pid->Kp * pid->error) + (pid->Ki * pid->integral) + (pid->Kd * derivative);

    // 输出限制（根据实际需求调整）
    pid->output = (pid->output > pid->output_limit_max) ? pid->output_limit_max : pid->output;
    pid->output = (pid->output < pid->output_limit_min) ? pid->output_limit_min : pid->output;

    // 更新上一次误差
    pid->prev_error = pid->error;
}

