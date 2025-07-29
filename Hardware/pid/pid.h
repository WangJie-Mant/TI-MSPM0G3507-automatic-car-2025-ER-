#ifndef __PID_H__
#define __PID_H__

//电机1使用参数
/*位置环相关参数*/
#define     MOTOR1_LOC_KP       0.3
#define     MOTOR1_LOC_KI       0.0
#define     MOTOR1_LOC_KD       0.0
/*速度环相关参数*/
#define     MOTOR1_SPD_KP       6.0
#define     MOTOR1_SPD_KI       8.5
#define     MOTOR1_SPD_KD       0.0

//电机2使用参数
/*位置环相关参数*/
#define     MOTOR2_LOC_KP       0.3
#define     MOTOR2_LOC_KI       0.0
#define     MOTOR2_LOC_KD       0.0
/*速度环相关参数*/
#define     MOTOR2_SPD_KP       6.0
#define     MOTOR2_SPD_KI       8.5
#define     MOTOR2_SPD_KD       0.0    

//角度环相关参数
#define     ANGLE_KP            1.2
#define     ANGLE_KI            0.0
#define     ANGLE_KD            0.0

//转向速度相关参数
// #define     LINE_KP             3.5
// #define     LINE_KI             0.0
// #define     LINE_KD             1.0

//角度环辅助直行相关参数
#define     STR_KP              290.0
#define     STR_KI              0.0
#define     STR_KD              0.0

typedef struct
{
    //uint8_t limit_switch;           //这个开关用于决定是否限幅，以
    //double (*deadzone_process)(double error);
    double integral, imax;                  //由于累加方式的不同，这里给出两个参数用于pid_calc
    double kp, ki, kd;
    double target, current;
    double out, pout, iout, dout;
    double error, lasterror;
    double out_max, iout_max;
    double dout_max, pout_max;
} pid_t;

extern pid_t g_pid_speed1, g_pid_speed2;
extern pid_t g_pid_location1, g_pid_location2;
extern pid_t g_pid_turn_angle;
extern pid_t g_pid_line;
extern pid_t g_pid_straight;


void pid_Init(pid_t *p, double kp, double ki, double kd, double out_max, double pout_max, double iout_max, double dout_max);
void pid_Set_Target(pid_t *p, double target);
double pid_Get_Target(pid_t *p);
void pid_Set_Params(pid_t *p, double kp, double ki, double kd);

double pid_location_Calc(pid_t *p, double current);
double pid_speed_Calc(pid_t *p, double current);
double pid_turn_angle_Calc(pid_t *p, double current);
double pid_straight_Calc(pid_t *p, double current);

double limit_abs(double x, double limit);

#endif