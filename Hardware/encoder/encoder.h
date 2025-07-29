#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "ti_msp_dl_config.h"

extern volatile int16_t encoder_Left_Count;
extern volatile int16_t encoder_Right_Count;

#define ENCODER_LEFT_B_STATUS DL_GPIO_readPins(ENCODER_PORT, ENCODER_LEFT_B_PIN) > 0 ? 1 : 0
#define ENCODER_RIGHT_B_STATUS DL_GPIO_readPins(ENCODER_PORT, ENCODER_RIGHT_B_PIN) > 0 ? 1 : 0
#define ENCODER_LEFT_A_STATUS DL_GPIO_readPins(ENCODER_PORT, ENCODER_LEFT_A_PIN) > 0 ? 1 : 0
#define ENCODER_RIGHT_A_STATUS DL_GPIO_readPins(ENCODER_PORT, ENCODER_RIGHT_A_PIN) > 0 ? 1 : 0

void encoder_Update(void);
int read_pluse(int motor_num);

#endif