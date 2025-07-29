#include "ti_msp_dl_config.h"
#include "encoder.h"
#include "pid.h"

pid_t g_pid_speed1, g_pid_speed2;
pid_t g_pid_location1, g_pid_location2;
pid_t g_pid_turn_angle;
pid_t g_pid_line;
pid_t g_pid_straight;


void pid_Init(pid_t *p, double kp, double ki, double kd, 
double out_max, double pout_max, double iout_max, double dout_max)
{
    /*根据添入的参数初始化指定的pid*/
    p->kp = kp;
    p->ki = ki;
    p->kd = kd;
    p->target = 0;
    p->current = 0;
    p->lasterror = 0;
    p->error = 0;
    p->integral = 0;
    p->out_max = out_max;
    p->pout_max = pout_max;
    p->iout_max = iout_max;
    p->dout_max = dout_max;
    // p->deadzone_process = deadzone_process;
    // p->limit_switch = limit_switch;
}

// double pid_Calc(pid_t *p)
// {
//     /*最基础的pid计算*/
//     p->error = p->target - p->current;
//     /*若在初始化时制定了对应的死区函数，则进行死区控制*/
//     if (p->deadzone_process)
//     {
//         p->error = p->deadzone_process(p->error);
//     }

//     p->pout = p->kp*p->error;
//     p->integral += p->error;
//     p->integral = limit_abs(p->integral, p->imax);
//     p->dout = p->kd * (p->error - p->lasterror);

//     p->pout = limit_abs(p->pout, p->pout_max);
//     p->iout = limit_abs(p->iout, p->iout_max);
//     p->dout = limit_abs(p->dout, p->dout_max);

//     p->out = p->pout + p->iout + p->dout;
//     p->out = limit_abs(p->out, p->out_max);

//     p->lasterror = p->error;

//     return p->out;
// }

void pid_Set_Target(pid_t *p, double target)
{
    /*设置指定pid的target参数*/
    p->target = target;
}

double pid_Get_Target(pid_t *p)
{
    /*返回制定pid的target参数*/
    return p->target;
}

void pid_Set_Params(pid_t *p, double kp, double ki, double kd)
{
    /*设置制定pid的参数*/
    p->kp = kp;
    p->ki = ki;
    p->kd = kd;
}


/*
* @retval       p->out
*
*
*/
double pid_location_Calc(pid_t *p, double current)
{
    /*位置环pid算法*/
    p->current = current;
    
    p->error = p->target - p->current;

    p->integral += p->error;
    p->pout = p->kp * p->error;
    p->iout = p->ki * p->integral;
    p->dout = p->kd * (p->error - p->lasterror);

    p->out = p->pout + p->iout + p->dout;
    p->lasterror = p->error;
    return p->out;
}

double pid_speed_Calc(pid_t *p, double current)
{
    /*速度环pid算法*/
    p->current = current;
    p->error = p->target - p->current;

    if ((p->error < 0.5) && (p->error > -0.5))
    {
        p->error = 0;
    }

    p->integral += p->error;
    p->integral = limit_abs(p->integral, 1000);

    p->pout = p->kp * p->error;
    p->iout = p->ki * p->integral;
    p->dout = p->kd * (p->error - p->lasterror);

    p->out = p->pout + p->iout + p->dout;
    p->lasterror = p->error;
    return p->out;
}

double pid_turn_angle_Calc(pid_t *p, double current)
{
    p->current = current;
    p->error = p->target - p->current;

    double raw_err = p->error;
    if (raw_err > 180.0)
    {
        p->error -= 358.0;
    }
    else if (raw_err < -180.0)
    {
        p->error += 358.0;
    }

    if ((p->error >= 358.0) || (p->error <= -358.0))
    {
        p->error = 0;
        p->integral = 0;
    }

    p->integral += p->error;
    p->integral = limit_abs(p->integral, 1000);

    p->pout = p->kp * p->error;
    p->iout = p->ki * p->integral;
    p->dout = p->kd * (p->error - p->lasterror);

    p->out = p->pout + p->iout + p->dout;
    p->lasterror = p->error;
    return p->out;
}

double pid_straight_Calc(pid_t *p, double current)
{
    /*直行辅助角度环pid算法*/
    p->current = current;
    p->error = p->target - p->current;

    while (p->error > 180.0)
    {
        p->error -= 360.0;
    }
    while (p->error < -180.0)
    {
        p->error += 360.0;
    }

    if (fabs(p->error) < 10.0)
    {
        p->integral += p->error;
        p->integral = limit_abs(p->integral, 50.0);
    } 
    else {
    p->integral = 0;
    }

    p->pout = p->kp * p->error;
    p->iout = p->ki * p->integral;
    p->dout = p->kd * (p->error - p->lasterror);

    p->out = p->pout + p->iout + p->dout;
    p->out = limit_abs(p->out, 2000.0);
    p->lasterror = p->error;
    return p->out;
}



double limit_abs(double x, double limit)
{
    /*将x限制在绝对值limit之内*/
    return ((x) > (limit) ? (limit) : ((x) < -(limit) ? -(limit) : (x)));
}