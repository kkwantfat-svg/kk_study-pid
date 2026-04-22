#include "pid.h"

#define INTEGRAL_SEPARATION_THRESHOLD 5.0f // 积分分离阈值，根据实际需求调整
#define STOP_POSITION                 5.0f
#define OUT_REMOVE                    5.0f // 输出偏移，根据实际需求调整

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

        // 积分限制（根据实际需求调整）
        pid->integral = (pid->integral > pid->integral_limit_max) ? pid->integral_limit_max : pid->integral;
        pid->integral = (pid->integral < pid->integral_limit_min) ? pid->integral_limit_min : pid->integral;

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

/**
 * @brief 更新位置PID控制器
 * @param pid PID控制器指针
 */
void PID_Update_Location(PID_Controller *pid)
{
    // 计算误差
    pid->error = pid->target - pid->actual;

    // 积分分离
    if (pid->Ki != 0.0f) // 只有当积分系数不为0时才计算积分
    {
        if(fabs(pid->error) < INTEGRAL_SEPARATION_THRESHOLD) // 当误差较小时才允许积分作用，避免积分过大导致系统不稳定
        {
            pid->integral += pid->error;
        }
    }
    else
    {
        pid->integral = 0.0f; // 如果积分系数为0，重置积分累计值
    }

    
    // 计算微分
    float derivative = pid->error - pid->prev_error;

    // 计算输出
    pid->output = (pid->Kp * pid->error) + (pid->Ki * pid->integral) + (pid->Kd * derivative);

    // 输入死区 + 输出偏移
    // if(pid->output > 0.0f)
    // {
    //     pid->output += OUT_REMOVE; // 正向输出增加偏移，确保足够的驱动力克服静摩擦
    // }
    // else if(pid->output < 0.0f)
    // {
    //     pid->output -= OUT_REMOVE; // 反向输出增加偏移，确保足够的驱动力克服静摩擦
    // }
    // if(fabs(pid->error) < STOP_POSITION)
    // {
    //     pid->output = 0.0f; // 如果输出小于停止位置，停止电机
    // }

    // 输出限制（根据实际需求调整）
    pid->output = (pid->output > pid->output_limit_max) ? pid->output_limit_max : pid->output;
    pid->output = (pid->output < pid->output_limit_min) ? pid->output_limit_min : pid->output;

    // 更新上一次误差
    pid->prev_error = pid->error;
}
