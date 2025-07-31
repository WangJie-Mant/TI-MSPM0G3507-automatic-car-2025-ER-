#include "encoder.h"
#include "main.h"
#include "ti_msp_dl_config.h"
#include <math.h> // 为了使用 fabs() 函数

long g_sigma_motor1pluse = 0;
long g_sigma_motor2pluse = 0;     // 电机2累计脉冲
short g_unittime_motor1pluse = 0; // 电机1单位时间脉冲
short g_unittime_motor2pluse = 0; // 电机2单位时间脉冲

double g_motor1_journey_cm = 0; // 电机1走过的路程
double g_motor2_journey_cm = 0; // 电机2走过的路程

double g_ftarget_journey =
    0; // 存放小车左右轮走的路程,单位cm，需要在下一阶段任务中设置(car_go),用于判断车子到达目标位置

uint8_t g_stop_count = 0; // 停止计时，用于判断是否停车
uint8_t g_spin_count = 0; // 旋转计时，用于判断是否完成旋转

uint8_t g_location_control_count =
    0; // 执行频率不需要那么高的用这个事件计数，用在中断中，控制串级pid内外环执行频率
uint8_t g_turn_angle_control_count =
    0; // 执行频率不需要那么高的用这个事件计数，用在中断中，控制串级pid内外环执行频率
uint8_t g_line_speed_control_count =
    0; // 执行频率不需要那么高的用这个事件计数，用在中断中，控制串级pid内外环执行频率

double g_speed1_outval = 0;    // 电机1速度环输出值
double g_location1_outval = 0; // 电机1位置环输出值
double g_speed2_outval = 0;    // 电机2速度环输出值
double g_location2_outval = 0; // 电机2位置环输出值
double g_turn_outval = 0;      // 电机差速输出值，用于角度环和巡线环
double g_speed3_outval = 0;    // 电机1转向速度环输出值
double g_speed4_outval = 0;    // 电机2转向速度环输出值

int g_motor1_pwm = 0; // 电机1装载pwm值
int g_motor2_pwm = 0; // 电机2装载pwm值

uint8_t g_Line_Flag = 0;         // 巡线标志位,0不巡线,1巡线
uint8_t g_Stop_Flag = 0;         // 停止标志位,0行驶,1停止
uint8_t g_Spin_Start_Flag = 0;   // 转向开始标志位
uint8_t g_Spin_Succeed_Flag = 0; // 转向结束标志位
uint8_t g_Turn_Flag = 0;         // 转向标志位,好像没用到
uint8_t g_Angle_Flag = 0;        // 角度环调试标志位
uint8_t g_Gostraght = 0;
int32_t g_line_num = 0;   // 灰度巡线偏移量
double g_line_outval = 0; // 巡线差值量
int32_t g_yaw_err = 0;    // 角度偏移量
uint8_t g_Straight_Flag = 0;

extern uint8_t g_mode; // 运行哪个功能

double angle_kp = 2.0f;
double angle_kd = 0.1f;
static double g_last_angle_error = 0;

double spin90_cm = 0;
int count2 = 0;
int count3 = 0;

void TIMER_0_INST_IRQHandler(void)
{
  /*20ms周期的定时器中断，pid控制等等在此处进行*/
  switch (DL_TimerA_getPendingInterrupt(TIMER_0_INST))
  {
  case DL_TIMER_IIDX_ZERO:
  {
    // // 读取编码器脉冲计数并清零，添加原子操作保护

    g_unittime_motor1pluse = -read_encoder_left();
    g_unittime_motor2pluse = -read_encoder_right();

    g_sigma_motor1pluse += g_unittime_motor1pluse;
    g_sigma_motor2pluse += g_unittime_motor2pluse;

    g_motor1_journey_cm =
        (g_sigma_motor1pluse / (REDUCTION_RATIO * ENCODER_TOTAL_RESOLUTION)) *
        (WHEEL_D * 3.1416);
    g_motor2_journey_cm =
        (g_sigma_motor2pluse / (REDUCTION_RATIO * ENCODER_TOTAL_RESOLUTION)) *
        (WHEEL_D * 3.1416);

    if (g_Line_Flag == 1)
    {
      if (g_motor1_journey_cm >= g_ftarget_journey - 2)
      {
        // 循迹任务完成
        g_Line_Flag = 0;     // 停止循迹
        g_Stop_Flag = 1;     // 设置停止标志
        g_Straight_Flag = 0; // 清除直行标志
        g_stop_count = 0;
        motor1_set_disable(); // 停止电机
        motor2_set_disable();
        count++; // 增加任务计数，允许主程序继续执行下一个任务
      }
      // else if (g_Straight_Flag == 1)
      // {
      //   pid_Set_Target(&g_pid_turn_angle, 179.5);
      //   turn_angle_pid_control();

      //   g_motor1_pwm = g_speed3_outval;
      //   g_motor2_pwm = g_speed4_outval;

      //   motor_limit_pwm(&g_motor1_pwm, &g_motor2_pwm);
      //   motor_load_pwm(g_motor1_pwm, g_motor2_pwm);

      //   if (g_yaw_jy60 <= g_pid_turn_angle.target + 2 &&
      //       g_yaw_jy60 >= g_pid_turn_angle.target - 2)
      //   {
      //     count3++;
      //     if (count3 >= 10)
      //     {
      //       motor1_set_disable();
      //       motor2_set_disable();
      //       g_Angle_Flag = 0;
      //       g_Stop_Flag = 0;
      //       count3 = 0;
      //       count++;
      //     }
      //   }
      // }
      else
      {
        g_Stop_Flag = 1;
        g_stop_count = 0;
      }
      if (g_is_motor1_enabled == 1 ||
          g_is_motor2_enabled == 1) // 电机在使能状态下才进行控制处理
      {
        // 使用STM32工程的巡线速度控制 - 提供基础速度+巡线补偿
        line_speed_control();

        // 巡线PWM输出 - 使用speed3和speed4（巡线专用输出）
        g_motor1_pwm = g_speed3_outval;
        g_motor2_pwm = g_speed4_outval;

        motor_limit_pwm(&g_motor1_pwm, &g_motor2_pwm);
        motor_load_pwm(g_motor1_pwm, g_motor2_pwm);
      }
    }
    if (g_Gostraght == 1)
    {
      // 1. 持续进行PID控制，计算并加载PWM值，让小车行驶
      if (g_is_motor1_enabled == 1 ||
          g_is_motor2_enabled == 1) // 电机在使能状态下才进行控制处理
      {
        location_speed_control(); // 位置环速度环串级PID的输出是速度环输出的PWM值
        // float angle_out = pid_straight_Calc(&g_pid_straight, g_yaw_jy60);
        //  叠加角度环（直行环）的控制量
        g_motor1_pwm = g_speed1_outval;
        g_motor2_pwm = g_speed2_outval;

        motor_limit_pwm(&g_motor1_pwm, &g_motor2_pwm);
        motor_load_pwm(g_motor1_pwm, g_motor2_pwm);
      }

      // 2. 判断是否到达目标位置
      if (g_motor1_journey_cm >= g_ftarget_journey - 5)
      {
        g_Gostraght = 0; // 退出直行模式
        g_Stop_Flag = 0; // 清除停止标志，为下一个动作做准备
        motor1_set_disable();
        motor2_set_disable();
        count++; // 任务完成，count自增，主函数可以继续执行
      }
    }
    if (g_Spin_Start_Flag == 1)
    {
      if (g_is_motor1_enabled == 1 ||
          g_is_motor2_enabled == 1) // 电机在使能状态下才进行控制处理
      {
        location_speed_control(); // 位置环速度环串级PID的输出是速度环输出的PWM值
        g_motor1_pwm = g_speed1_outval;
        g_motor2_pwm = g_speed2_outval;
        motor_limit_pwm(&g_motor1_pwm, &g_motor2_pwm);
        motor_load_pwm(g_motor1_pwm, g_motor2_pwm);
        // 修正停止条件：使用脉冲数判断，更精确
        double target_pulse = fabs(g_pid_location1.target);
        if ((fabs(g_sigma_motor1pluse) >= target_pulse - 10) &&
            (fabs(g_sigma_motor2pluse) >= target_pulse - 10))
        {
          motor1_set_disable();
          motor2_set_disable();
          g_Spin_Start_Flag = 0;
          g_Stop_Flag = 0;
          count++;
        }

        // 安全停止：防止无限旋转（超过目标值太多时强制停止）
        if ((fabs(g_sigma_motor1pluse) > target_pulse + 50) ||
            (fabs(g_sigma_motor2pluse) > target_pulse + 50))
        {
          motor1_set_disable();
          motor2_set_disable();
          g_Spin_Start_Flag = 0;
          g_Stop_Flag = 0;
          count++;
        }
      }
    }
    if (g_Angle_Flag == 1)
    {
      motor1_set_enable(); // 使能电机1控制PWM输出
      motor2_set_enable(); // 使能电机2控制PWM输出
      if (g_is_motor1_enabled == 1 ||
          g_is_motor2_enabled == 1) // 电机在使能状态下才进行控制处理
      {
        turn_angle_speed_control(); // 位置环速度环串级PID的输出是速度环输出的PWM值
        g_motor1_pwm = g_speed3_outval;
        g_motor2_pwm = g_speed4_outval;
        motor_limit_pwm(&g_motor1_pwm, &g_motor2_pwm);
        motor_load_pwm(g_motor1_pwm, g_motor2_pwm);
        if (g_pid_turn_angle.target > 0)
        {
          if (g_yaw_jy60 >=
              g_pid_turn_angle.target - 12) // 这里的角度误差可以调节
          {

            motor1_set_disable();
            motor2_set_disable();
            g_Angle_Flag = 0;
            g_Stop_Flag = 0;
            count++;
            // 转向完成后将标志位清零
          }
        }
        else if (g_pid_turn_angle.target < 0)
        {

          if (g_yaw_jy60 <= g_pid_turn_angle.target + 2 &&
              g_yaw_jy60 >=
                  g_pid_turn_angle.target - 2) // 这里的角度误差可以调节
          {
            count2++;
            if (count2 >= 10)
            {
              motor1_set_disable();
              motor2_set_disable();
              g_Angle_Flag = 0;
              g_Stop_Flag = 0;
              count2 = 0;
              count++;
            }
            // 转向完成后将标志位清零
          }
        }
      }

      DL_TimerA_clearInterruptStatus(TIMER_0_INST, DL_TIMERA_IIDX_ZERO);
      break;
    }
  }
  default:
    break;
  }
}
// }

void car_stop(void)
{
  // 清除所有运动标志位
  g_Line_Flag = 0;         // 巡线标志位,0不巡线,1巡线
  g_Spin_Start_Flag = 0;   // 转向开始标志位
  g_Spin_Succeed_Flag = 0; // 转向结束标志位
  g_Turn_Flag = 0;         // 转向标志位
  g_Angle_Flag = 0;        // 角度环调试标志位
  g_Gostraght = 0;         // 直行标志位

  // 禁用电机
  motor1_set_disable();
  motor2_set_disable();
}

void car_go(int32_t distance_cm, double degree)
{
  g_pid_straight.target = degree;

  double target_pluse;

  g_Line_Flag = 0;
  g_Spin_Start_Flag = 0;
  g_Spin_Succeed_Flag = 0;
  g_Gostraght = 1;
  /* 清除上次走的路程记录 */
  g_motor1_journey_cm = 0;
  g_motor2_journey_cm = 0;
  g_sigma_motor1pluse = 0;
  g_sigma_motor2pluse = 0;

  g_ftarget_journey = distance_cm;

  target_pluse = (distance_cm / (WHEEL_D * 3.1416)) *
                 (REDUCTION_RATIO * ENCODER_TOTAL_RESOLUTION);

  pid_Set_Target(&g_pid_location1, target_pluse);
  pid_Set_Target(&g_pid_location2, target_pluse);

  motor1_set_enable();
  motor2_set_enable();
}

void car_go_line(int32_t distance_cm)
{
  double target_pluse;

  /* 设置对应标志位 */
  g_Line_Flag = 1;
  g_Spin_Start_Flag = 0;
  g_Spin_Succeed_Flag = 0;
  g_Gostraght = 0;
  /* 清除上次走的路程记录 */
  g_motor1_journey_cm = 0;
  g_motor2_journey_cm = 0;
  g_sigma_motor1pluse = 0;
  g_sigma_motor2pluse = 0;

  g_ftarget_journey = distance_cm;

  target_pluse = (distance_cm / (WHEEL_D * 3.1416)) *
                 (REDUCTION_RATIO * ENCODER_TOTAL_RESOLUTION);

  pid_Set_Target(&g_pid_location1, target_pluse);
  pid_Set_Target(&g_pid_location2, target_pluse);

  motor1_set_enable();
  motor2_set_enable();
}

void car_spin(spin_dir direction)
{
  double car_turn_pluse;

  g_Line_Flag = 0;
  g_Gostraght = 0;
  g_Spin_Start_Flag = 1;
  g_Spin_Succeed_Flag = 0;

  /* 清除上次路程 */
  g_motor1_journey_cm = 0;
  g_motor2_journey_cm = 0;
  g_sigma_motor1pluse = 0;
  g_sigma_motor2pluse = 0;

  // 重置PID状态，避免积分累积
  g_pid_location1.integral = 0;
  g_pid_location1.lasterror = 0;
  g_pid_location2.integral = 0;
  g_pid_location2.lasterror = 0;
  g_pid_speed1.integral = 0;
  g_pid_speed1.lasterror = 0;
  g_pid_speed2.integral = 0;
  g_pid_speed2.lasterror = 0;

  /* 自旋90度的距离 */
  spin90_cm =
      0.25 * 3.1416 * LUN_JU; // 以车中心为圆心，小车两轮距离为直径，小车旋转90度即为圆的1/4

  if (direction == left_90)
  {
    car_turn_pluse = (spin90_cm / (WHEEL_D * 3.142)) *
                     (REDUCTION_RATIO * ENCODER_TOTAL_RESOLUTION);
  }
  else if (direction == right_90)
  {
    car_turn_pluse = -(spin90_cm / (WHEEL_D * 3.142)) *
                     (REDUCTION_RATIO * ENCODER_TOTAL_RESOLUTION);
  }
  else if (direction == back_180)
  {
    car_turn_pluse = -(spin90_cm / (WHEEL_D * 3.142)) *
                     (REDUCTION_RATIO * ENCODER_TOTAL_RESOLUTION) * 2;
  }

  pid_Set_Target(&g_pid_location1, car_turn_pluse);  // 左轮正转用负目标
  pid_Set_Target(&g_pid_location2, -car_turn_pluse); // 右轮反转用正目标

  motor1_set_enable();
  motor2_set_enable();
}

void car_spin_degree(double angle)
{
  /* 设置对应标志位 */
  g_Line_Flag = 0;
  g_Angle_Flag = 1;
  g_Spin_Start_Flag = 0;
  g_Spin_Succeed_Flag = 0;
  g_Gostraght = 0;
  /* 清除上次走的路程记录 */
  g_motor1_journey_cm = 0;
  g_motor2_journey_cm = 0;
  g_sigma_motor1pluse = 0;
  g_sigma_motor2pluse = 0;

  pid_Set_Target(&g_pid_turn_angle, angle);
  motor1_set_enable();
  motor2_set_enable();
}

void location_speed_control(void)
{
  if (g_is_motor1_enabled == 1 ||
      g_is_motor2_enabled == 1) // 电机在使能状态下才进行控制处理
  {
    /* 位置环控制 */
    g_location_control_count++;
    if (g_location_control_count >= 2) // 内环执行频率大于外环
    {
      g_location_control_count = 0;
      g_location1_outval = location1_pid_control();
      g_location2_outval = location2_pid_control();
    }

    /* 位置环输出作为速度环输入 */
    pid_Set_Target(&g_pid_speed1, g_location1_outval); // 每次都必须有位置环的值
    pid_Set_Target(&g_pid_speed2, g_location2_outval); // 每次都必须有位置环的值

    /* 速度环控制 */
    g_speed1_outval =
        speed1_pid_control(); // 要是电机转向不符合预期，就在这两句里取反数值
    g_speed2_outval = speed2_pid_control();
  }
}

double location1_pid_control(void)
{
  double cont_val = 0.0;
  int32_t actual_location;

  actual_location = g_sigma_motor1pluse;

  cont_val = pid_location_Calc(&g_pid_location1, actual_location);

  cont_val = limit_abs(cont_val, TARGET_SPEED_MAX);

  return cont_val;
}

double speed1_pid_control(void)
{
  double cont_val = 0.0; // 当前控制值
  int32_t actual_speed;  // 车轮实际转速度,单位rpm(转/min)

  actual_speed =
      ((double)g_unittime_motor1pluse * 1000.0 * 60.0) /
      (ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO * SPEED_PID_PERIOD);

  cont_val = pid_speed_Calc(&g_pid_speed1, actual_speed);

  return cont_val;
}

double location2_pid_control(void)
{
  double cont_val = 0.0;
  int32_t actual_location;

  actual_location = g_sigma_motor2pluse;

  cont_val = pid_location_Calc(&g_pid_location2, actual_location);

  cont_val = limit_abs(cont_val, TARGET_SPEED_MAX);

  return cont_val;
}

double speed2_pid_control(void)
{
  double cont_val = 0.0; // 当前控制值
  int32_t actual_speed;  // 车轮实际转速度,单位rpm(转/min)

  actual_speed =
      ((double)g_unittime_motor2pluse * 1000.0 * 60.0) /
      (ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO * SPEED_PID_PERIOD);

  cont_val = pid_speed_Calc(&g_pid_speed2, actual_speed);

  return cont_val;
}

void turn_angle_speed_control(void)
{
  if (g_is_motor1_enabled || g_is_motor2_enabled)
  {
    g_turn_angle_control_count++;
    if (g_turn_angle_control_count >= 2)
    {
      g_turn_angle_control_count = 0;
      g_turn_outval = turn_angle_pid_control();
    }
    pid_Set_Target(&g_pid_speed1, -g_turn_outval);
    pid_Set_Target(&g_pid_speed2, g_turn_outval);

    g_speed3_outval = speed1_pid_control();
    g_speed4_outval = speed2_pid_control();
  }
}

double turn_angle_pid_control(void)
{
  double cont_val = 0.0;
  int32_t actual_angle;

  actual_angle = g_yaw_jy60;

  cont_val = pid_turn_angle_Calc(&g_pid_turn_angle, actual_angle);

  cont_val = limit_abs(cont_val, TARGET_SPEED_MAX);

  return cont_val;
}

/**
 * @brief       巡线速度串级PID实现，通过巡线实现差速转弯 - 从STM32工程移植
 * @param       无
 * @retval      无
 */
void line_speed_control(void)
{
  if (g_is_motor1_enabled == 1 ||
      g_is_motor2_enabled == 1) // 电机在使能状态下才进行控制处理
  {
    g_line_speed_control_count++; // 这个是巡线串级pid外环
    if (g_line_speed_control_count >= 2)
    {
      g_line_speed_control_count = 0;
      g_turn_outval = line_pid_control();
    }

    // 这个的800是基础的速度加减g_turn_outval实现差速
    pid_Set_Target(&g_pid_speed1, 800 - g_turn_outval);
    pid_Set_Target(&g_pid_speed2, 800 + g_turn_outval);
    g_speed3_outval = speed1_pid_control();
    g_speed4_outval = speed2_pid_control();
  }
}

/**
 * @brief       巡线环控制 - 基础版本
 * @param       无
 * @retval      巡线环的输出值，实际上是两个轮子的差速值
 */
double line_pid_control(void)
{
  double cont_val = 0.0; // 当前控制值

  // 使用灰度传感器巡线偏差作为PID输入
  int32_t err = line_err();
  cont_val = line_pid_realize(err); // 基础PID控制，不放大

  return cont_val;
}
