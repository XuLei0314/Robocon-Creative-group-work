#include "chassis.h"
#include "PID.h"
#include "stm32f10x.h"
#include "string.h"
#include <math.h>


#define L 0.2185
#define W 0.2165

#define K 433.00f //mm
#define R 76			//mm

//#define K 433.00f //mm
//#define R 76			//mm


extern moto_measure_t moto_chassis[4]; 
extern chassis_t chassis;
extern pid_t pid_spd[4];


void chassis_pid_param_init(void)
{
	
	int k;
	for ( k = 0; k < 3; k++) {
		//max current = 20000
		PID_struct_init(&pid_spd[3], POSITION_PID, 11000, 1500, 18.0f, 1.0f, 1.0f); 
	}
}

void mecanum_calc(float vx, float vy, float vw, const int each_max_spd, float speed[])
{
	float   buf[4];
	int   i;
	float max = 0, rate;

	VAL_LIMIT(vx, -MAX_CHASSIS_VX_SPEED, MAX_CHASSIS_VX_SPEED);
  VAL_LIMIT(vy, -MAX_CHASSIS_VY_SPEED, MAX_CHASSIS_VY_SPEED);
  VAL_LIMIT(vw, -MAX_CHASSIS_VR_SPEED, MAX_CHASSIS_VR_SPEED);

			buf[0] = (float)( -vy - vx - vw*(L+W));  //391.55
			buf[1] = (float)( -vy + vx - vw*(L+W));
			buf[2] = (float)( +vy + vx - vw*(L+W));	
			buf[3] = (float)( +vy - vx - vw*(L+W));


	//find max item
	for (i = 0; i < 4; i++)
	{
			if (MyAbs(buf[i]) > max)
					max = MyAbs(buf[i]);
	}
	//equal proportion
	if (max > each_max_spd)
	{
			rate = each_max_spd / max;
			for (i = 0; i < 4; i++)
					buf[i] *= rate;
	}
	memcpy(speed, buf, sizeof(float) * 4);
}
void omni_calc(float vx, float vy, float vw, const int each_max_spd, float speed[])
{
	float   buf[3];
	int   i;
	float max = 0, rate;

	VAL_LIMIT(vx, -MAX_CHASSIS_VX_SPEED, MAX_CHASSIS_VX_SPEED);
  VAL_LIMIT(vy, -MAX_CHASSIS_VY_SPEED, MAX_CHASSIS_VY_SPEED);
  VAL_LIMIT(vw, -MAX_CHASSIS_VR_SPEED, MAX_CHASSIS_VR_SPEED);

			buf[0] = (float)(+OMNI_VX_VALUE*vy + OMNI_VY_VALUE*vx + OMNI_L_value*vw);  
			buf[1] = (float)(-OMNI_VX_VALUE*vy + OMNI_VY_VALUE*vx - OMNI_L_value*vw);
			buf[2] = (float)(vy - OMNI_L_value*vw);	


	//find max item
	for (i = 0; i < 3; i++)
	{
			if (MyAbs(buf[i]) > max)
					max = MyAbs(buf[i]);
	}
	//equal proportion
	if (max > each_max_spd)
	{
			rate = each_max_spd / max;
			for (i = 0; i < 3; i++)
					buf[i] *= rate;
	}
	
	memcpy(speed, buf, sizeof(float) * 3);
}
