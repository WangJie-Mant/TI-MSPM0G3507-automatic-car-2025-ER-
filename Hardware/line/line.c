#include "control.h"
#include "jy60.h"
#include "pid.h"
#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <stdlib.h> // 为了使用 abs()
#define limit_abs(x, limit) ((x) > (limit) ? (limit) : ((x) < -(limit) ? -(limit) : (x)))

static float line_pid_integral = 0;
static float line_pid_last_err = 0;
static int32_t g_line_last_err = 0;

// 灰度寻迹PID控制函数 - 基础版本
float line_pid_realize(int err)
{
  float p, i, d, out;

  p = LINE_KP * err;

  // 基础积分控制
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

  out = p + i + d;
  line_pid_last_err = err;

  // 积分限幅，防止积分过大
  if (line_pid_integral > 3500)
    line_pid_integral = 3500;
  if (line_pid_integral < -3500)
    line_pid_integral = -3500;

  // 输出限幅，防止输出过大
  out = limit_abs(out, 1000);

  return out;
}

/**
 * @brief       灰度巡线补偿值获取 - 基础版本
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
    // 简单处理：丢线时返回上次的误差值，保持方向
    return 0;
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
 * @brief       检测是否压到任何黑线
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
