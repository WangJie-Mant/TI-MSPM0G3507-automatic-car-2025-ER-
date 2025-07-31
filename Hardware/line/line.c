#include "control.h"
#include "jy60.h"
#include "pid.h"
#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <stdlib.h> // 为了使用 abs()
#define limit_abs(x, limit) ((x) > (limit) ? (limit) : ((x) < -(limit) ? -(limit) : (x)))


// 搜索策略参数 - 从STM32工程移植
#define INITIAL_SEARCH_ERR 2500   // 初始搜索误差
#define MAX_SEARCH_ERR 4500       // 最大搜索误差
#define SEARCH_INCREMENT 300      // 每次搜索误差增量
#define SWING_SEARCH_THRESHOLD 50 // 开始摆动搜索的阈值(周期数)
#define SWING_PERIOD 20           // 摆动周期(控制周期数)

// 脱轨检测参数 - 从STM32工程移植
#define OFF_TRACK_THRESHOLD 100 // 脱轨判断阈值(连续丢线周期数)
#define SEARCH_TIMEOUT 150      // 搜索超时时间(3秒)

static float line_pid_integral = 0;
static float line_pid_last_err = 0;
static int32_t g_line_last_err = 0;

// 搜索状态变量 - 从STM32工程移植
static int lost_line_counter = 0;                     // 丢线计数器
static int search_err_magnitude = INITIAL_SEARCH_ERR; // 当前搜索误差大小
static int swing_phase = 0;                           // 摆动搜索的相位计数器

// 灰度寻迹PID控制函数 - 从STM32工程移植
float line_pid_realize(int err)
{
    float p, i, d, out;

    p = LINE_KP * err;

    // 如果是丢线状态（误差绝对值很大），说明在寻找黑线
    if (abs(err) > 2000)
    {
        // 丢线时的处理策略
        lost_line_counter++;

        // 清零积分，避免积分累积影响寻线
        line_pid_integral = 0;

        // 纯比例控制，快速转向寻找黑线
        d = 0; // 暂时不使用微分，避免震荡
        i = 0;

        // 渐进式搜索：随着丢线时间增加，逐渐增大搜索力度
        if (lost_line_counter > 10 && lost_line_counter <= SWING_SEARCH_THRESHOLD)
        {
            // 第一阶段：渐进增强搜索
            if (lost_line_counter % 15 == 0)
            { // 每15个周期增强一次
                search_err_magnitude += SEARCH_INCREMENT;
                if (search_err_magnitude > MAX_SEARCH_ERR)
                {
                    search_err_magnitude = MAX_SEARCH_ERR;
                }
            }
        }
        else if (lost_line_counter > SWING_SEARCH_THRESHOLD)
        {
            // 第二阶段：摆动搜索策略
            swing_phase++;

            // 实现左右摆动搜索
            int swing_cycle = swing_phase % (SWING_PERIOD * 2);
            if (swing_cycle < SWING_PERIOD)
            {
                // 前半周期：朝一个方向搜索
                search_err_magnitude = MAX_SEARCH_ERR;
            }
            else
            {
                // 后半周期：朝相反方向搜索
                search_err_magnitude = MAX_SEARCH_ERR;
            }
        }
    }
    else
    {
        // 正常循迹状态 - 重置所有搜索状态
        lost_line_counter = 0;
        search_err_magnitude = INITIAL_SEARCH_ERR;
        swing_phase = 0;

        if (abs(err) < 20)
        { // 误差小于20时，积分开始累积
            if (abs(line_pid_last_err) < 20)
            { // 上次误差小于20时，才累积
                line_pid_integral += err;
            }
        }
        else if (abs(err) >= 20)
        { // 误差大于等于20时，清零积分
            line_pid_integral = 0;
        }

        i = LINE_KI * line_pid_integral;
        d = LINE_KD * (err - line_pid_last_err);
    }

    out = p + i + d;
    line_pid_last_err = err;

    // 积分限幅，防止积分过大
    if (line_pid_integral > 3500)
        line_pid_integral = 3500;
    if (line_pid_integral < -3500)
        line_pid_integral = -3500;

    // 输出限幅，防止输出过大（增大限幅以允许更强的转向）
    out = limit_abs(out, 6000);

    return out;
}

/**
 * @brief       灰度巡线补偿值获取 - 从STM32工程移植
 * @param       无
 * @retval      巡线偏移的值
 */
int32_t line_err(void)
{
    // 灰度寻迹加权平均法，权重按物理位置分布，间隔0.5cm，中心为0
    int sensor_val[8];
    float sensor_pos[8] = {1.75, 1.25, 0.75, 0.25, -0.25, -0.75, -1.25, -1.75}; // 单位cm，中心为0
    float sum = 0, weight = 0;

    // 灰度传感器编号：1~8，左到右
    sensor_val[0] = HW1;
    sensor_val[1] = HW2;
    sensor_val[2] = HW3;
    sensor_val[3] = HW4;
    sensor_val[4] = HW5;
    sensor_val[5] = HW6;
    sensor_val[6] = HW7;
    sensor_val[7] = HW8;

    for (int i = 0; i < 8; i++)
    {
        sum += sensor_val[i] * sensor_pos[i];
        weight += sensor_val[i];
    }

    if (weight == 0)
    {
        int32_t search_err;

        // 根据搜索状态决定返回的误差值
        if (lost_line_counter <= SWING_SEARCH_THRESHOLD)
        {
            if (g_line_last_err > 0)
            {
                search_err = search_err_magnitude; // 向右搜索，强度逐渐增加
            }
            else if (g_line_last_err < 0)
            {
                search_err = -search_err_magnitude; // 向左搜索，强度逐渐增加
            }
            else
            {
                // 如果没有历史误差，默认向右搜索
                search_err = search_err_magnitude;
                g_line_last_err = 1; // 初始方向
            }
        }
        else
        {
            int swing_cycle = swing_phase % (SWING_PERIOD * 2);
            if (swing_cycle < SWING_PERIOD)
            {
                // 前半周期：朝原方向搜索
                search_err = (g_line_last_err > 0) ? search_err_magnitude : -search_err_magnitude;
            }
            else
            {
                // 后半周期：朝相反方向搜索
                search_err = (g_line_last_err > 0) ? -search_err_magnitude : search_err_magnitude;
            }
        }

        return search_err;
    }
    else
    {
        // 正常情况下计算加权平均误差
        int32_t current_err = (int32_t)(sum / weight * 1000);
        g_line_last_err = current_err; // 保存当前有效的误差，用于丢线时参考
        return current_err;
    }
}

/**
 * @brief       检测是否压到任何黑线 - 从STM32工程移植
 * @retval      1: 检测到线, 0: 未检测到线
 */
int8_t is_line_detected(void)
{
    if (HW1 || HW2 || HW3 || HW4 || HW5 || HW6 || HW7 || HW8)
        return 1;
    else
        return 0;
}

/**
 * @brief       判断是否在直线上并且车身已对正 - 从STM32工程移植
 * @note        通过检测中间几个传感器是否稳定在黑线上判断
 * @retval      1: 在直线上且对正, 0: 否则
 */
int8_t is_on_straight_pattern(void)
{
    if (!HW3 && HW4 && HW5 && !HW6)
    {
        return 1;
    }
    if (!HW2 && HW3 && HW4 && HW5 && HW6 && !HW7)
    {
        return 1;
    }
    if (!HW3 && !HW4 && HW5 && !HW6 && !HW7)
    {
        return 1;
    }
    return 0;
}

/**
 * @brief       检测是否完全丢失黑线 - 从STM32工程移植
 * @retval      1: 丢失线, 0: 还有线
 */
int8_t is_line_lost(void)
{
    // 所有传感器都没有检测到黑线
    if (!HW1 && !HW2 && !HW3 && !HW4 && !HW5 && !HW6 && !HW7 && !HW8)
        return 1;
    else
        return 0;
}

/**
 * @brief       获取当前搜索状态（调试用） - 从STM32工程移植
 * @retval      搜索状态代码：0=正常循迹, 1=渐进搜索, 2=摆动搜索
 */
int get_search_status(void)
{
    if (lost_line_counter == 0)
    {
        return 0; // 正常循迹
    }
    else if (lost_line_counter <= SWING_SEARCH_THRESHOLD)
    {
        return 1; // 渐进搜索
    }
    else
    {
        return 2; // 摆动搜索
    }
}

/**
 * @brief       检测是否完全脱离循迹路段 - 从STM32工程移植
 * @note        当搜索超时或长期找不到线时，判断为脱轨
 * @retval      1: 完全脱轨, 0: 仍在路径附近
 */
int8_t is_completely_off_track(void)
{
    // 如果连续搜索超过阈值时间仍未找到线，认为完全脱轨
    if (lost_line_counter > SEARCH_TIMEOUT)
    {
        return 1;
    }

    // 如果在摆动搜索阶段超过一定时间，也认为脱轨
    if (lost_line_counter > OFF_TRACK_THRESHOLD && get_search_status() == 2)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief       角度巡线补偿值获取 - 从STM32工程移植
 * @param       无
 * @retval      角度偏移量
 */
/* 以0度为基准 */
int32_t yaw_err0(void)
{
  int32_t yaw_num;
  if ((g_yaw_jy60 > 0) && (g_yaw_jy60 < 90))
    yaw_num = g_yaw_jy60;
  else if ((g_yaw_jy60 > 270) && (g_yaw_jy60 < 360))
    yaw_num = g_yaw_jy60 - 360;
  else
    yaw_num = 0;
  return yaw_num;
}

/* 掉头后以180度为基准 - 从STM32工程移植 */
int32_t yaw_err180(void)
{
  int32_t yaw_num;
  if ((g_yaw_jy60 > 180) && (g_yaw_jy60 < 270))
    yaw_num = g_yaw_jy60 - 180;
  else if ((g_yaw_jy60 > 90) && (g_yaw_jy60 < 180))
    yaw_num = g_yaw_jy60 - 180;
  else
    yaw_num = 0;
  return yaw_num;
}

int detect_line(void)
{
  if (HW1 || HW2 || HW3 || HW4 || HW5 || HW6 || HW7 || HW8)
    return 0;
  else
  {
    // car_stop();
    // g_Stop_Flag = 0;
    return 1;
  }
}
