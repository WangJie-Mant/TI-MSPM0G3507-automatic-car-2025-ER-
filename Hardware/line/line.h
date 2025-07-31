#ifndef __LINE_H__
#define __LINE_H__

#include "main.h"
#include "ti_msp_dl_config.h"
#include <stdint.h>

// PID参数 - 降低以便调试
#define LINE_KP 3.0f // 降低KP值，避免抖动过大
#define LINE_KI 0.0f
#define LINE_KD 0.0f

// 传感器宏定义保持原有
#define HW1 (DL_GPIO_readPins(INF_INF1_PORT, INF_INF1_PIN) > 0 ? 1 : 0)
#define HW2 (DL_GPIO_readPins(INF_INF2_PORT, INF_INF2_PIN) > 0 ? 1 : 0)
#define HW3 (DL_GPIO_readPins(INF_INF3_PORT, INF_INF3_PIN) > 0 ? 1 : 0)
#define HW4 (DL_GPIO_readPins(INF_INF4_PORT, INF_INF4_PIN) > 0 ? 1 : 0)
#define HW5 (DL_GPIO_readPins(INF_INF5_PORT, INF_INF5_PIN) > 0 ? 1 : 0)
#define HW6 (DL_GPIO_readPins(INF_INF6_PORT, INF_INF6_PIN) > 0 ? 1 : 0)
#define HW7 (DL_GPIO_readPins(INF_INF7_PORT, INF_INF7_PIN) > 0 ? 1 : 0)
#define HW8 (DL_GPIO_readPins(INF_INF8_PORT, INF_INF8_PIN) > 0 ? 1 : 0)

// 从STM32工程移植的函数声明
float line_pid_realize(int err); // STM32工程中的主要PID函数
int32_t line_err(void);
int32_t yaw_err0(void);
int32_t yaw_err180(void);
int detect_line(void);

// 基础功能函数声明
int8_t is_line_detected(void);

#endif