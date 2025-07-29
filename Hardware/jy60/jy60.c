#include "jy60.h"

static uint8_t RxBuffer[11];
static volatile uint8_t RxState = 0;
static uint8_t RxIndex = 0;
float g_roll_jy60, g_pitch_jy60, g_yaw_jy60;

void jy60_ReceiveData(uint8_t RxData) {
  /*接收数据包并处理*/
  uint8_t i, sum = 0;
  if (RxState == 0) {
    if (RxData == 0x55) {
      RxBuffer[RxIndex] = RxData;
      RxState = 1;
      RxIndex = 1;
    }
  } else if (RxState == 1) {
    if (RxData == 0x53) {
      /*判断数据内容。修改这里可以改变要读的数据内容，0x53为角度输出*/
      RxBuffer[RxIndex] = RxData;
      RxState = 2;
      RxIndex = 2;
    }
  } else if (RxState == 2) {
    RxBuffer[RxIndex++] = RxData;
    if (RxIndex == 11) {
      for (i = 0; i < 10; i++) {
        sum += RxBuffer[i]; // 计算校验和
      }
      if (sum == RxBuffer[10]) {
        /*计算数据，根据数据内容选择对应的公式*/
        // g_roll_jy60 =
        //     ((uint16_t)((uint16_t)RxBuffer[3] << 8 | (uint16_t)RxBuffer[2])) /
        //     32768.0f * 180.0f;
        // g_pitch_jy60 =
        //     ((uint16_t)((uint16_t)RxBuffer[5] << 8 | (uint16_t)RxBuffer[4])) /
        //     32768.0f * 180.0f;
        g_yaw_jy60 =
            ((int16_t)(RxBuffer[7] << 8 | RxBuffer[6])) / 32768.0f * 180.0f;
      }
      RxState = 0;
      RxIndex = 0;
    }
  }
}