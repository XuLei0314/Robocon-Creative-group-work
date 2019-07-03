/*****************************************************************************

******************************************************************************/


#include "gpio.h"
#include "delay.h"
#include "sys.h"
#include "OLED_I2C.h"
#include "usart.h" 	
#include "timer.h"	 	
#include "can.h"
#include "motor.h"
#include "DataScope_DP.h"
#include "PCA9685.h"
#include "PID.h"
#include "chassis.h"

#define ON 0
#define OFF 1

void Send_DataScope(float ch1,float ch2,float ch3,float ch4);

/*
		M1												F								  M2
		     LED0          ---------------         LED1 
		Encoder:TIM2						 |||						Encoder:TIM3
														 |||		
									           |||	
														 |||
														 |||
														 |||
	                          	M3											              
		                         LED2  
		                      Encoder:TIM4                       

*/

 moto_measure_t moto_chassis[4];
int wheel_speed[4] = {0}; 
int Encoder_Speed[4] = {0};

extern pid_t pid_spd[4];
chassis_t chassis = { 0 };
int16_t current_motor[3] = {0};

tReceRasPiData_t RecPiData = {0};


int main(void)
 {	 
	 int j;
	delay_init();	    
	 
	NVIC_Configuration(); 
	 
 	MY_GPIO_Init();			
	MOTOR_GPIO_Init();
	 
	TIM1_Int_Init(4999,72-1); //0.005s 
  uart1_init(115200);	 
	PCA9685_Init(); 
	OLED_Init();	 
	
	for(j = 0;j < 3;j++)
	 PID_struct_init(&pid_spd[j], POSITION_PID, 4095, 5000, 100.0f, 20.0f, 2.0f);
	 
	MOTOR_Encoder_Init();
	 
	
	OLED_CLS();

	OLED_ShowStr(20,0,"MiniRM V1.0",2);

	//PCA9685_Set_PWM(LED3,4095);
	while(1)
	{
		if(ONBOARD_PRESS)
		{
			PCA9685_Set_PWM(LED3,0);
			chassis.vy +=10;
	//		chassis.wheel_w_speed[0]+=10;
			while(ONBOARD_PRESS);
		}
		
		//ShowNum(20,2,moto_chassis[2].speed_rpm);
		//printf("speed is %d \n",moto_chassis[0].speed_rpm);
		Send_DataScope(moto_chassis[0].speed_rpm,chassis.wheel_w_speed[0],0,0);
		delay_ms(10);
	}
	 

	 



}
 
void Send_DataScope(float ch1,float ch2,float ch3,float ch4)
{
	unsigned char count;
	unsigned char Send_Count;
	
	DataScope_Get_Channel_Data( ch1 , 1 );
	DataScope_Get_Channel_Data( ch2 , 2 );
	DataScope_Get_Channel_Data( ch3 , 3 );
	DataScope_Get_Channel_Data( ch4 , 4 );
	
	Send_Count = DataScope_Data_Generate(4); 
	
	 for( count = 0 ; count < Send_Count; count++) 
          {
             while((USART1->SR&0X40)==0);  
            USART1->DR = DataScope_OutPut_Buffer[count]; 
          }
		delay_ms(10);
	
}

