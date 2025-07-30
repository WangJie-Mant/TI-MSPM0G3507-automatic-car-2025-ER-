#include "control.h"
#include "jy60.h"
#include "pid.h"
#include "ti_msp_dl_config.h"

pid_t line_pid;

// 巡线PID初始化函数
void line_pid_init(void)
{
  pid_Init(&line_pid, LINE_KP, LINE_KI, LINE_KD, 0, 0, 0, 0);
}

double line_pid_Calc(int error)
{
  line_pid.error = error;
  line_pid.pout = line_pid.kp * line_pid.error;
  line_pid.integral += line_pid.error;
  line_pid.integral = limit_abs(
      line_pid.integral,
      3500); // 32版本中这个限幅放在了计算pid输出下面，其实是完全没有用到的。如果ti调试有问题，试试注释这一句

  line_pid.iout = line_pid.ki * line_pid.integral;
  line_pid.dout = line_pid.kd * (line_pid.error - line_pid.lasterror);

  line_pid.out = line_pid.pout + line_pid.iout + line_pid.dout;

  return line_pid.out;
}

int32_t line_err(void)
{
  /*返回偏移量，单位mm（放大1000倍）*/
  int sensor_val[8];
  float sensor_weight[8] = {1.75, 1.25, 0.75, 0.25, -0.25, -0.75, -1.25, -1.75};
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
    return (int32_t)(sum / weight * 1000); // 返回偏移量，单位mm，放大1000倍用于pid控制
  }
}

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
