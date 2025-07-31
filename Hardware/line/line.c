#include "control.h"
#include "jy60.h"
#include "pid.h"
#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <stdlib.h> // 为了使用 abs()
#define limit_abs(x, limit) ((x) > (limit) ? (limit) : ((x) < -(limit) ? -(limit) : (x)))

static float g_pid_line_integral = 0;
static float g_pid_line_last_err = 0;
static int32_t g_line_last_err = 0;

void line_init(void)
{
  pid_Init(&g_pid_line, LINE_KP, LINE_KI, LINE_KD, 0, 0, 0, 0);
}

double pid_line_Calc(int error)
{
  g_pid_line.error = error;
  g_pid_line.pout = g_pid_line.kp * g_pid_line.error;
  g_pid_line.integral += g_pid_line.error;
  g_pid_line.integral = limit_abs(g_pid_line.integral, 3500); // 32版本中这个限幅放在了计算pid输出下面，其实是完全没有用到的。如果ti调试有问题，试试注释这一句

  g_pid_line.iout = g_pid_line.ki * g_pid_line.integral;
  g_pid_line.dout = g_pid_line.kd * (g_pid_line.error - g_pid_line.lasterror);

  g_pid_line.out = g_pid_line.pout + g_pid_line.iout + g_pid_line.dout;

  // 限制PID输出，防止调整幅度过大
  g_pid_line.out = limit_abs(g_pid_line.out, 1200); // 适中的限制，避免过大转向

  g_pid_line.lasterror = g_pid_line.error;

  return g_pid_line.out;
}

/**
 * @brief       灰度巡线补偿值获取 - 基础版本
 * @param       无
 * @retval      巡线偏移的值
 */
int32_t line_err(void)
{
  // 灰度寻迹加权平均法，权重按物理位置分布，间隔0.5cm，中心为0
  /*返回偏移量，单位mm（放大1000倍）*/
  int sensor_val[8];
  float sensor_weight[8] = {-1.75, -1.25, -0.75, -0.25, 0.25, 0.75, 1.25, 1.75};
  float sum = 0, weight = 0;

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
    sum += sensor_val[i] * sensor_weight[i];
    weight += sensor_val[i];
  }
  if (weight == 0)
  {
    return 0;
  }
  else
  {
    return (int32_t)(sum / weight * 750); // 适中的放大倍数：500倍，兼顾响应和稳定
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
