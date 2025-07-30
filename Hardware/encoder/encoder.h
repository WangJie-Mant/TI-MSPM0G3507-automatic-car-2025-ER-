#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <stdint.h>
// 初始化编码器硬件（可选）
void encoder_Init(void);
// 读取左编码器累计脉冲并清零
int read_encoder_left(void);
// 读取右编码器累计脉冲并清零
int read_encoder_right(void);
// 中断服务入口，由GROUP1_IRQHandler调用
void encoder_IRQHandler(void);

#endif // __ENCODER_H__
