#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "sys.h"


/*
		M2												F								  M1
		T8C4  PA10 (----)  ---------------   (----) T8C1 PA5
		Encoder:TIM1						 |||						Encoder:TIM3
														 |||		
									           |||	
														 |||
														 |||
														 |||
		M3											 |||                M4
		T8C3  PB5  (----)  ---------------   (----) T8C2 PA4
		Encoder:TIM4                            Encoder:TIM2

*/




#define ENCODER_TIM_PERIOD (u16)(65535)  

#define M1_DIR		PAout(5)

#define M2_DIR		PAout(10)

#define M3_DIR		PBout(5)

#define M4_DIR		PAout(4)


void MOTOR_PWM_Init(u16 arr,u16 psc);
void MOTOR_DIR_GPIO_Init(void);
void MOTOR_Encoder_Init(void);

void MOTOR_Speed(int16_t s1,int16_t s2,int16_t s3);  //-10000 -- +10000

void MOTOR_Encoder_Init_TIM1(void);
void MOTOR_Encoder_Init_TIM2(void);
void MOTOR_Encoder_Init_TIM3(void);
void MOTOR_Encoder_Init_TIM4(void);

#endif
