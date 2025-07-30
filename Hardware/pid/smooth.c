#include "pid.h"
#include "math.h"

/**
 * @brief 目标值平滑处理函数
 * @param p PID结构体指针
 * @param target 目标值
 * @return 平滑后的目标值
 */
double smooth_target(pid_t *p, double target)
{
    // 确定运动方向
    if (fabs(target - p->current) < 0.001)
    {
        p->direction = 0;
    }
    else
    {
        p->direction = (target > p->current) ? 1 : -1;
    }

    // 计算目标值变化量
    double delta = target - p->last_smoothed_target;
    double max_delta = (p->direction >= 0) ? SPEED_RAMP_UP : SPEED_RAMP_DOWN;

    // 限制变化速率
    if (fabs(delta) > max_delta)
    {
        delta = max_delta * p->direction;
    }

    // 更新平滑目标值
    p->smoothed_target = p->last_smoothed_target + delta;
    p->last_smoothed_target = p->smoothed_target;

    return p->smoothed_target;
}

/**
 * @brief 速度低通滤波
 * @param p PID结构体指针
 * @param raw_speed 原始速度值
 * @return 滤波后的速度值
 */
double filter_speed(pid_t *p, double raw_speed)
{
    // 一阶低通滤波
    p->filtered_speed = FILTER_FACTOR * raw_speed +
                        (1.0 - FILTER_FACTOR) * p->last_speed;
    p->last_speed = p->filtered_speed;

    return p->filtered_speed;
}

/**
 * @brief 位置环软启动函数
 * @param current 当前位置
 * @param target 目标位置
 * @return 软启动后的目标位置
 */
double soft_start(double current, double target)
{
    double distance = fabs(target - current);
    double direction = (target > current) ? 1.0 : -1.0;

    // 在启动初期使用非线性曲线
    if (distance > POSITION_SOFT_START)
    {
        return current + direction * POSITION_SOFT_START;
    }

    return target;
}
