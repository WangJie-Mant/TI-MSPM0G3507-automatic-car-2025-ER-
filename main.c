/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "main.h"
#include "stdio.h"
#include "ti_msp_dl_config.h"

uint8_t g_oledstring[50];

extern uint8_t g_usart2_receivedata;
uint8_t g_mode = 0;
int count = 0;
int circle_num = 0;
int turn_num = 0; // 旋转次数
// uint8_t first_long;

int main(void)
{
    //  AIN1_RESET;
    //     AIN2_SET;
    // BIN1_RESET;
    //  BIN2_SET;
    SYSCFG_DL_init();
    DL_TimerA_startCounter(PWM_MOTOR_INST);
    motor1_set_enable();
    motor2_set_enable();
    line_init();

    OLED_Init();
    pid_Init(&g_pid_speed1, MOTOR1_SPD_KP, MOTOR1_SPD_KI, MOTOR1_SPD_KD, 0, 0, 0,
             0);
    pid_Init(&g_pid_speed2, MOTOR2_SPD_KP, MOTOR2_SPD_KI, MOTOR2_SPD_KD, 0, 0, 0,
             0);
    pid_Init(&g_pid_location1, MOTOR1_LOC_KP, MOTOR1_LOC_KI, MOTOR1_LOC_KD, 0, 0,
             0, 0);
    pid_Init(&g_pid_location2, MOTOR2_LOC_KP, MOTOR2_LOC_KI, MOTOR2_LOC_KD, 0, 0,
             0, 0);
    pid_Init(&g_pid_turn_angle, ANGLE_KP, ANGLE_KI, ANGLE_KD, 0, 0, 0, 0);
    pid_Init(&g_pid_line, LINE_KP, LINE_KI, LINE_KD, 0, 0, 0, 0);
    pid_Init(&g_pid_straight, STR_KP, STR_KI, STR_KD, 0, 0, 0, 0);

    // 设置正方形轨迹巡线圈数 (可以根据需要修改)
    circle_num = 1; // 默认走1圈正方形

    // OLED_Init();

    for (int i = 0; i < 100000; i++)
        ;

    buzz_Init();
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(UART1_INST_INT_IRQN);

    NVIC_ClearPendingIRQ(ENCODER_GPIOA_INT_IRQN);
    NVIC_ClearPendingIRQ(ENCODER_GPIOB_INT_IRQN);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART1_INST_INT_IRQN);

    NVIC_EnableIRQ(ENCODER_GPIOA_INT_IRQN);
    NVIC_EnableIRQ(ENCODER_GPIOB_INT_IRQN);

    uint8_t key = 0;
    uint16_t target_angle = 0;
    uint8_t oledbuff[50];
    static uint32_t display_counter = 0; // 显示更新计数器

    // DL_GPIO_setPins(MOTOR_PORT, MOTOR_AIN1_PIN);
    // DL_GPIO_clearPins(MOTOR_PORT, MOTOR_AIN2_PIN);

    // DL_GPIO_setPins(MOTOR_PORT, MOTOR_BIN1_PIN);
    // DL_GPIO_clearPins(MOTOR_PORT, MOTOR_BIN2_PIN);
    // 电机测试配置 - 不要手动设置方向，让motor_load_pwm函数处理
    // motor_load_pwm(300, 300); // 正值：motor1正转，motor2正转
    OLED_Clear();

    // 测试STM32移植的巡线功能
   car_go_line(100); // 巡线前进50cm来测试位置环
    //  car_spin(left_90); // 测试左转90度
    //  car_spin_degree(90);
    //  car_go(100, 0);

    while (1)
    {
        sprintf((char *)oledbuff, "line: %d%d%d%d%d%d%d%d", HW1, HW2, HW3, HW4, HW5, HW6, HW7, HW8);
        OLED_ShowString(0, 0, oledbuff, 16, 1);
        OLED_Refresh();

        sprintf((char *)oledbuff, "err: %d", g_line_num);
        OLED_ShowString(0, 16, oledbuff, 16, 1);
        OLED_Refresh();

        // 显示位置环目标和当前值，用于调试位置环
        sprintf((char *)oledbuff, "tgt:%.0f act:%ld", g_pid_location1.target, g_sigma_motor1pluse);
        OLED_ShowString(0, 32, oledbuff, 16, 1);
        OLED_Refresh();

        sprintf((char *)oledbuff, "m1:%.2f m2:%.2f", g_motor1_journey_cm, g_motor2_journey_cm);
        OLED_ShowString(0, 48, oledbuff, 16, 1);
        OLED_Refresh();

        // sprintf((char *)oledbuff, "SPD1:%.0f SPD2:%.0f", g_speed1_outval, g_speed2_outval);
        // OLED_ShowString(0, 48, oledbuff, 16, 1);
        // OLED_Refresh();

        // // 显示循迹传感器状态
        // sprintf((char *)oledbuff, "Line:%d%d%d%d%d%d%d%d", HW1, HW2, HW3, HW4,
        // HW5, HW6, HW7, HW8); OLED_ShowString(0, 32, oledbuff, 16, 1);
        // OLED_Refresh();

        // // 显示循迹误差
        // sprintf((char *)oledbuff, "Err:%ld", line_err());
        // OLED_ShowString(0, 48, oledbuff, 16, 1);
        // OLED_Refresh();

        // OLED_ShowString(5, 5, (char*)oledbuff);

        // 第一问
        // if (g_Stop_Flag == 0 && count == 0)
        // {
        //     g_Stop_Flag = 1;
        //     car_go_line(105);
        //     first_long = g_motor1_journey_cm; // 记录第一次行驶的距离
        // }
        // if (g_Stop_Flag == 0 && count == 1)
        // {
        //     g_Stop_Flag = 1;
        //     car_spin(left_90);
        //     turn_num++;
        // }
        // if (g_Stop_Flag == 0 && count == 2)
        // {
        //     g_Stop_Flag = 1;
        //     car_go_line(105);
        // }
        // if (g_Stop_Flag == 0 && count == 3)
        // {
        //     g_Stop_Flag = 1;
        //     car_spin(left_90);
        //     turn_num++;
        // }
        // if (g_Stop_Flag == 0 && count == 4)
        // {
        //     g_Stop_Flag = 1;
        //     car_go_line(105);
        // }
        // if (g_Stop_Flag == 0 && count == 5)
        // {
        //     g_Stop_Flag = 1;
        //     car_spin(left_90);
        //     turn_num++;
        // }
        // if (g_Stop_Flag == 0 && count == 6)
        // {
        //     g_Stop_Flag = 1;
        //     car_go_line(105);
        // }
        // if (g_Stop_Flag == 0 && count == 7)
        // {
        //     g_Stop_Flag = 1;
        //     car_spin(left_90);
        //     turn_num++;
        // }
        // if (g_Stop_Flag == 0 && count == 8)
        // {
        //     if (4 * N != turn_num)
        //     {
        //         count = 1; // 重置计
        //         g_Stop_Flag = 1;
        //         car_go_line(105);
        //         turn_num++;
        //     }
        //     else
        //     {
        //         g_Stop_Flag = 1;
        //         car_go_line(105 - first_long);
        //     }
        // }
        // 第二问
        // 第三问

        // 如果旋转次数小于4次，继续执行

        // if(g_Stop_Flag==0&&count==0){
        //    Buzz(10000);
        //    g_Stop_Flag=1;
        //     car_go(95);
        //  }
        // if(g_Stop_Flag==0&&count==1){
        //    Buzz(10000);
        //    g_Stop_Flag=1;
        //     car_go_line(122);
        //		 detect_line();
        //  }
        //	if(g_Stop_Flag==0&&count==2)
        // {
        //  g_Stop_Flag=1;
        //  Buzz(10000);
        //  car_spin_degree(179.5);
        //}
        // if(g_Stop_Flag==0&&count==3)
        // {
        //	  Buzz(10000);
        //	  g_Stop_Flag=1;
        //		car_go(105);
        // }
        //  if(g_Stop_Flag==0&&count==4)
        // {
        //	  Buzz(10000);
        //	  g_Stop_Flag=1;
        //		car_go_line(125);
        // }

        //
        // if(g_Stop_Flag==0&&count==0){
        //    Buzz(10000);
        //    g_Stop_Flag=1;
        //     car_go(130);
        //  }
        // if(g_Stop_Flag==0&&count==1){
        //    Buzz(10000);
        //    g_Stop_Flag=1;
        //     car_go_line(128);
        //		 detect_line();
        //  }
        //	if(g_Stop_Flag==0&&count==2)
        // {
        //  g_Stop_Flag=1;
        //  Buzz(10000);
        //  car_spin_degree(-128.66);
        //}
        // if(g_Stop_Flag==0&&count==3)
        // {
        //	  Buzz(10000);
        //	  g_Stop_Flag=1;
        //		car_go(130);
        // }
        //  if(g_Stop_Flag==0&&count==4)
        // {
        //	  Buzz(10000);
        //	  g_Stop_Flag=1;
        //		car_go_line(125);
        // }
        //
        // if (g_Stop_Flag == 0 && count == 0)
        // {
        //   Buzz(10000);
        //   g_Stop_Flag = 1;
        //   car_spin_degree(-35.66);
        // }
        // if (g_Stop_Flag == 0 && count == 1)
        // {
        //   Buzz(10000);
        //   g_Stop_Flag = 1;
        //   car_go(128);
        // }
        // if (g_Stop_Flag == 0 && count == 2)
        // {
        //   Buzz(10000);
        //   g_Stop_Flag = 1;
        //   car_go_line(128);
        // }
        // if (g_Stop_Flag == 0 && count == 3)
        // {
        //   g_Stop_Flag = 1;
        //   Buzz(10000);
        //   car_spin_degree(-179.5);
        // }
        // if (g_Stop_Flag == 0 && count == 4)
        // {
        //   Buzz(10000);
        //   g_Stop_Flag = 1;
        //   car_go(6);
        // }
        // if (g_Stop_Flag == 0 && count == 5)
        // {
        //   g_Stop_Flag = 1;
        //   Buzz(10000);
        //   car_spin_degree(-130.66);
        // }
        // if (g_Stop_Flag == 0 && count == 6)
        // {
        //   Buzz(10000);
        //   g_Stop_Flag = 1;
        //   car_go(130);
        // }
        // if (g_Stop_Flag == 0 && count == 7)
        // {
        //   Buzz(10000);
        //   g_Stop_Flag = 1;
        //   car_go_line(125);
        // }
        // sprintf((char*)g_oledstring, "Yaw:%6.2f", g_yaw_jy60);
        // OLED_ShowString(0, 32, (char*) g_oledstring);
    }
}

// void SysTick_Handler(void)
// {
//     if (delay_times!=0)
//     {
//         delay_times--;
//     }
// }
