#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "ti_msp_dl_config.h"

#define AIN1_SET DL_GPIO_setPins(MOTOR_PORT, MOTOR_AIN1_PIN)
#define AIN1_RESET DL_GPIO_clearPins(MOTOR_PORT, MOTOR_AIN1_PIN)
#define AIN2_SET DL_GPIO_setPins(MOTOR_PORT, MOTOR_AIN2_PIN)
#define AIN2_RESET DL_GPIO_clearPins(MOTOR_PORT, MOTOR_AIN2_PIN)
#define BIN1_SET DL_GPIO_setPins(MOTOR_PORT, MOTOR_BIN1_PIN)
#define BIN1_RESET DL_GPIO_clearPins(MOTOR_PORT, MOTOR_BIN1_PIN)
#define BIN2_SET DL_GPIO_setPins(MOTOR_PORT, MOTOR_BIN2_PIN)
#define BIN2_RESET DL_GPIO_clearPins(MOTOR_PORT, MOTOR_BIN2_PIN)

#define PWM_MAX 7500 // 接近定时器period 8000，留些余量
#define PWM_MIN -7500

extern uint8_t g_is_motor1_enabled;
extern uint8_t g_is_motor2_enabled;

void motor_load_pwm(int motor1_pwm, int motor2_pwm);
int find_abs(int p);
void motor_limit_pwm(int *p_motor1_pwm, int *p_motor2_pwm);
void motor1_set_enable(void);
void motor1_set_disable(void);
void motor2_set_enable(void);
void motor2_set_disable(void);

#endif
