#include "main.h"

uint8_t g_menu_selection = 0;
uint8_t g_oledstring[50];

// OLED一级显示菜单：
//  1. 第一问
//  2. 第二问
//  3. 第三问
void main_menu(void)
{
    int menu_selected_flag = 0;

    OLED_ShowString(0, 0, "1. []第一问", 16, 1);
    OLED_ShowString(0, 16, "2. []第二问", 16, 1);
    OLED_ShowString(0, 32, "3. []第三问", 16, 1);
    OLED_Refresh();

    while (!menu_selected_flag)
    {
        uint8_t key = key_Scan();

        if (key == 1) // 按键1用于选择菜单
        {
            g_menu_selection = (g_menu_selection + 1) % 3; // 循环选择菜单项
            // 根据选择的菜单项更新显示
            if (g_menu_selection == 1)
            {
                OLED_ShowString(0, 0, "1. [*]第一问", 16, 1);
            }
            else if (g_menu_selection == 2)
            {
                OLED_ShowString(0, 16, "2. [*]第二问", 16, 1);
            }
            else if (g_menu_selection == 3)
            {
                OLED_ShowString(0, 32, "3. [*]第三问", 16, 1);
            }
        }
        else if (key == 2) // 按键2用于确认选择
        {
            menu_selected_flag = 1; // 设置标志位，退出菜单选择循环
            OLED_Clear();
        }
    }

    if (g_menu_selection == 1)
    {
        menu_Q1();
    }
    else if (g_menu_selection == 2)
    {
        menu_Q2();
    }
    else if (g_menu_selection == 3)
    {
        menu_Q3();
    }
}

void menu_Q1(void)
{
    int circle_selected = 0;
    while (!circle_selected)
    {
        uint8_t key = key_Scan();

        if (key == 1)
        {
            // 按键一用于增加圈数
            circle_num++;
            // 用oled显示当前圈数
            OLED_ShowString(0, 0, "Q1 Circle:", 16, 1);
            sprintf((char *)g_oledstring, "%d", circle_num);
            OLED_ShowString(0, 16, g_oledstring, 16, 1);
            OLED_Refresh();
        }
        else if (key == 2)
        {
            // 按键二用于确认圈数
            circle_selected = 1; // 设置标志位，退出圈数选择循环
            OLED_Clear();
        }
    }

    task_Q1(circle_num);
}