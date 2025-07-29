#ifndef __LINE_H__
#define __LINE_H__

#include "main.h"
#include "ti_msp_dl_config.h"

#define LINE_KP 31.6
#define LINE_KI 0.003
#define LINE_KD 0.164

#define     HW1     (DL_GPIO_readPins(INF_INF1_PORT, INF_INF1_PIN) > 0 ? 1:0)
#define     HW2     (DL_GPIO_readPins(INF_INF2_PORT, INF_INF2_PIN) > 0 ? 1:0)
#define     HW3     (DL_GPIO_readPins(INF_INF3_PORT, INF_INF3_PIN) > 0 ? 1:0)
#define     HW4     (DL_GPIO_readPins(INF_INF4_PORT, INF_INF4_PIN) > 0 ? 1:0)
#define     HW5     (DL_GPIO_readPins(INF_INF5_PORT, INF_INF5_PIN) > 0 ? 1:0)
#define     HW6     (DL_GPIO_readPins(INF_INF6_PORT, INF_INF6_PIN) > 0 ? 1:0)
#define     HW7     (DL_GPIO_readPins(INF_INF7_PORT, INF_INF7_PIN) > 0 ? 1:0)
#define     HW8     (DL_GPIO_readPins(INF_INF8_PORT, INF_INF8_PIN) > 0 ? 1:0)

double line_pid_Calc(int error);
int32_t line_err(void);
int32_t yaw_err0(void);
int32_t yaw_err180(void);
int detect_line(void);



#endif