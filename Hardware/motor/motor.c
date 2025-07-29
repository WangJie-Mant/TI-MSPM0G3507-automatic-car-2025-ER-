#include "ti_msp_dl_config.h"
#include "motor.h"

uint8_t g_is_motor1_enabled = 0;
uint8_t g_is_motor2_enabled = 0;

void motor_load_pwm(int motor1_pwm, int motor2_pwm)
{
    /*将pid运算结果装载进电机*/
    if (motor1_pwm > 0)
    {
        AIN1_RESET;
        AIN2_SET;
    }
    else {
        AIN1_SET;
        AIN2_RESET;
    }
    DL_Timer_setCaptureCompareValue(PWM_MOTOR_INST, find_abs(motor1_pwm), DL_TIMER_CC_0_INDEX);

    if (motor2_pwm > 0)
    {
        BIN1_RESET;
        BIN2_SET;
    }
    else {
        BIN1_SET;
        BIN2_RESET;
    }
    DL_Timer_setCaptureCompareValue(PWM_MOTOR_INST, find_abs(motor2_pwm), DL_TIMER_CC_1_INDEX);
}

void motor_limit_pwm(int *p_motor1_pwm,int *p_motor2_pwm)
{
    if(*p_motor1_pwm > PWM_MAX)*p_motor1_pwm = PWM_MAX;
    if(*p_motor1_pwm < PWM_MIN)*p_motor1_pwm = PWM_MIN;
    
    if(*p_motor2_pwm > PWM_MAX)*p_motor2_pwm = PWM_MAX;
    if(*p_motor2_pwm < PWM_MIN)*p_motor2_pwm = PWM_MIN;
}

void motor1_set_enable(void)
{
    g_is_motor1_enabled = 1;
    DL_TimerA_startCounter(PWM_MOTOR_INST);
}

void motor1_set_disable(void)
{
    g_is_motor1_enabled = 0;
    DL_TimerA_stopCounter(PWM_MOTOR_INST);
}

void motor2_set_enable(void)
{
    g_is_motor2_enabled = 1;
    DL_TimerA_startCounter(PWM_MOTOR_INST);
}

void motor2_set_disable(void)
{
    g_is_motor2_enabled = 0;
    DL_TimerA_stopCounter(PWM_MOTOR_INST);
}

int find_abs(int p)
{
    int q;
    q = p>0?p:(-p);
    return q;
}
