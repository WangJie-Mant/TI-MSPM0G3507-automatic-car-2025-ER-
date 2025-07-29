#include "ti_msp_dl_config.h"
#include "encoder.h"

volatile int16_t encoder_Left_Count = 0;
volatile int16_t encoder_Right_Count = 0;

void encoder_Update(void)
{
    /*由外部中断调用的编码器读数函数*/
    if (DL_GPIO_getEnabledInterruptStatus(ENCODER_PORT, ENCODER_LEFT_A_PIN)) // 左轮A相触发上升沿中断
    {
        if (ENCODER_LEFT_B_STATUS == 0) // B相低电平
        {
            encoder_Left_Count++;
        }
        else if (ENCODER_LEFT_B_STATUS == 1) // B相高电平
        {
            encoder_Left_Count--;
        }
        DL_GPIO_clearInterruptStatus(ENCODER_PORT, ENCODER_LEFT_A_PIN); // 清除中断
    }

    if (DL_GPIO_getEnabledInterruptStatus(ENCODER_PORT, ENCODER_RIGHT_A_PIN)) // 右轮A相触发上升沿中断
    {
        if (ENCODER_RIGHT_B_STATUS == 0)
        {
            encoder_Right_Count++;
        }
        else if (ENCODER_RIGHT_B_STATUS == 1)
        {
            encoder_Right_Count--;
        }
        DL_GPIO_clearInterruptStatus(ENCODER_PORT, ENCODER_RIGHT_A_PIN);
    }

    if (DL_GPIO_getEnabledInterruptStatus(ENCODER_PORT, ENCODER_LEFT_B_PIN))
    {
        if (ENCODER_LEFT_A_STATUS == 0)
        {
            encoder_Left_Count++;
        }
        else if (ENCODER_LEFT_A_STATUS == 1)
        {
            encoder_Left_Count--;
        }
        DL_GPIO_clearInterruptStatus(ENCODER_PORT, ENCODER_LEFT_B_PIN);
    }

    if (DL_GPIO_getEnabledInterruptStatus(ENCODER_PORT, ENCODER_RIGHT_B_PIN))
    {
        if (ENCODER_RIGHT_A_STATUS == 0)
        {
            encoder_Right_Count++;
        }
        else if (ENCODER_RIGHT_A_STATUS == 1)
        {
            encoder_Right_Count--;
        }
        DL_GPIO_clearInterruptStatus(ENCODER_PORT, ENCODER_RIGHT_B_PIN);
    }
}

int read_pluse(int motor_num)
{
    int16_t pluse;
    if (motor_num == 1) // 左轮
    {
        pluse = encoder_Left_Count;
        encoder_Left_Count = 0;
    }
    else if (motor_num == 2)
    {
        pluse = encoder_Right_Count;
        encoder_Right_Count = 0;
    }
    else
    {
        pluse = 0;
        encoder_Left_Count = 0;
        encoder_Right_Count = 0;
    }
    return pluse;
}
