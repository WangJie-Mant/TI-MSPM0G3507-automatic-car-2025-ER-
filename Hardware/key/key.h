#ifndef __KEY_H__
#define __KEY_H__

#include "ti_msp_dl_config.h"
#include "main.h"

#define KEY1_STATE DL_GPIO_readPins(KEY_PORT, KEY_KEY1_PIN) != 0 ? 1 : 0
#define KEY2_STATE DL_GPIO_readPins(KEY_PORT, KEY_KEY2_PIN) != 0 ? 1 : 0

uint8_t key_Scan(void);

#endif
