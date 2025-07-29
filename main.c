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

#include "ti_msp_dl_config.h"
#include "main.h"
#include "stdio.h"

uint8_t g_oledstring[50];

extern uint8_t g_usart2_receivedata;
uint8_t g_mode = 0;
int count = 0;

int main(void)
{
    SYSCFG_DL_init();

    motor1_set_enable();
    motor2_set_enable();

    pid_Init(&g_pid_speed1, MOTOR1_SPD_KP, MOTOR1_SPD_KI, MOTOR1_SPD_KD, 0,0,0,0);
    pid_Init(&g_pid_speed2, MOTOR2_SPD_KP, MOTOR2_SPD_KI, MOTOR2_SPD_KD, 0, 0, 0, 0);
    pid_Init(&g_pid_location1, MOTOR1_LOC_KP, MOTOR1_LOC_KI, MOTOR1_LOC_KD, 0, 0, 0, 0);
    pid_Init(&g_pid_location2, MOTOR2_LOC_KP, MOTOR2_LOC_KI, MOTOR2_LOC_KD, 0,0,0,0);
    pid_Init(&g_pid_location2, MOTOR2_LOC_KP, MOTOR2_LOC_KI, MOTOR2_LOC_KD, 0, 0, 0, 0);
    pid_Init(&g_pid_turn_angle, ANGLE_KP, ANGLE_KI, ANGLE_KD, 0, 0, 0, 0);
    pid_Init(&g_pid_line, LINE_KP, LINE_KI, LINE_KD, 0, 0, 0, 0);
    pid_Init(&g_pid_straight, STR_KP, STR_KI, STR_KD, 0, 0, 0, 0);

    OLED_Init();
    
    buzz_Init();

    uint8_t key = 0;
    uint16_t target_angle = 0;

    /*test*/
    // car_go(120, 0);
    // car_spin_degree(120);

    while (1)
    {
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
        sprintf((char*)g_oledstring, "Yaw:%6.2f", g_yaw_jy60);
        OLED_ShowString(0, 32, (char*) g_oledstring);
    }
}


// void SysTick_Handler(void)
// {
//     if (delay_times!=0)
//     {
//         delay_times--;
//     }
// }
