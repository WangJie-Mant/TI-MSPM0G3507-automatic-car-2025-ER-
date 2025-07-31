#include "main.h"
#include "task.h"

double first_long = 0;

void task_Q1(int circle_num)
{
    if (g_Stop_Flag == 0 && count == 0)
    {
        g_Stop_Flag = 1;
        car_go_line(105);
        first_long = g_motor1_journey_cm; // 记录第一次行驶的距离
    }
    if (g_Stop_Flag == 0 && count == 1)
    {
        g_Stop_Flag = 1;
        car_spin(left_90);
        turn_num++;
    }
    if (g_Stop_Flag == 0 && count == 2)
    {
        g_Stop_Flag = 1;
        car_go_line(105);
    }
    if (g_Stop_Flag == 0 && count == 3)
    {
        g_Stop_Flag = 1;
        car_spin(left_90);
        turn_num++;
    }
    if (g_Stop_Flag == 0 && count == 4)
    {
        g_Stop_Flag = 1;
        car_go_line(105);
    }
    if (g_Stop_Flag == 0 && count == 5)
    {
        g_Stop_Flag = 1;
        car_spin(left_90);
        turn_num++;
    }
    if (g_Stop_Flag == 0 && count == 6)
    {
        g_Stop_Flag = 1;
        car_go_line(105);
    }
    if (g_Stop_Flag == 0 && count == 7)
    {
        g_Stop_Flag = 1;
        car_spin(left_90);
        turn_num++;
    }
    if (g_Stop_Flag == 0 && count == 8)
    {
        if (4 * circle_num != turn_num)
        {
            count = 1; // 重置计
            g_Stop_Flag = 1;
            car_go_line(105);
            turn_num++;
        }
        else
        {
            g_Stop_Flag = 1;
            car_go_line(105 - first_long);
        }
    }
}