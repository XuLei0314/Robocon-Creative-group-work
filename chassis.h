#ifndef _CHASSIS_H
#define _CHASSIS_H

#include "stm32f10x.h"

#define PI 3.1415
#define MyAbs(x) ((x > 0) ? (x) : (-x))


#define MAX_WHEEL_SPEED 50
#define MAX_CHASSIS_VX_SPEED 50
#define MAX_CHASSIS_VY_SPEED 50
#define MAX_CHASSIS_VR_SPEED 50

#define auto_to_chassis_x 1.f
#define auto_to_chassis_y 1.f
#define auto_to_chassis_w 1.72992f


#define OMNI_VX_VALUE           (0.5f)
//#define OMNI_VY_VALUE           ((sqrt(3)/2.f))    
#define OMNI_VY_VALUE           70   
#define OMNI_L_value            (55.f) 



#define VAL_LIMIT(val, min, max)\
if(val<=min)\
{\
	val = min;\
}\
else if(val>=max)\
{\
	val = max;\
}\


void chassis_pid_param_init(void);
void mecanum_calc(float vx, float vy, float vw, const int each_max_spd, float speed[]);
void omni_calc(float vx, float vy, float vw, const int each_max_spd, float speed[]);

typedef struct
{
    int16_t  speed_rpm;

    int16_t  given_current;
    uint8_t  hall;

    uint16_t offset_angle;
    int32_t  round_cnt;
    int32_t  total_ecd;
    int32_t  total_angle;
	  u32      msg_cnt;
	
	  uint16_t angle; //abs angle range:[0,8191]
    uint16_t last_angle; //abs angle range:[0,8191]
} moto_measure_t;

typedef enum {
	  CHASSIS_AUTO = 0, 
	
    CHASSIS_STOP, // stall
    CHASSIS_RELAX, // relax
    CHASSIS_OPEN_LOOP,
    CHASSIS_CLOSE_GYRO_LOOP,
    CHASSIS_FOLLOW_GIMBAL,

} eChassisMode;

typedef struct
{
    float           vx; // forward/back
    float           vy; // left/right
    float           vw; // 
    float           kb_vx;
    float           kb_vy;
    float           mouse_vw;
    eChassisMode    mode;
    eChassisMode    last_mode;
    float           target_angle; //
    float           angle_from_gyro;
		float           palstance_from_gyro;
    float           last_angle;
    int             is_snipe_mode; //gimbal chassis separate
    int16_t         wheel_speed[4];
		float           wheel_w_speed[4];
} chassis_t; //chassis status


#endif
