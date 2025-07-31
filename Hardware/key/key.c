#include "main.h"

uint8_t key_Scan(void)
{
    uint8_t key = 0;

    if (KEY1_STATE == 1)
    {
        // 按键消抖
        Delay_ms(10);
        if (KEY1_STATE == 1) // 确认按键状态
        {
            while (KEY1_STATE == 1)
                ; // 等待按键释放
        }
        key = 1; // KEY1 pressed
    }
    else if (KEY2_STATE == 1)
    {
        // 按键消抖
        Delay_ms(10);
        if (KEY2_STATE == 1) // 确认按键状态
        {
            while (KEY2_STATE == 1)
                ; // 等待按键释放
        }
        key = 2; // KEY2 pressed
    }

    return key;
}