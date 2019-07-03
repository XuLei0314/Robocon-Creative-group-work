#ifndef ___PCA9685_H_
#define ___PCA9685_H_
	#include "stm32f10x.h"
	/*  
	    SCL->PC.12 
	    SDA->PC.11
			OE->PC.15
			*/
			
			#define u8 unsigned char								//unsigned char = u8
			#define u16 unsigned short int						//unsigned short int = u16
			#define u32 unsigned long int 						//unsigned long int =u32
			
			#define PCA9685Address			 0x80 // Read +1
			#define Mode1Add 	 0x00
			#define Mode2Add 	 0x01
			#define PRE_SCALE  0xFE   //50HZ 0x80    
			#define LED0       0x06
			#define LED1       0x0A
			#define LED2       0x0E
			#define LED3       0x12
			#define LED4       0x16
			#define LED5       0x1A
			#define LED6       0x1E
			#define LED7       0x22
			#define LED8       0x26
			#define LED9       0x2A
			#define LED10      0x2E
			#define LED11      0x32
			#define LED12      0x36
			#define LED13      0x3A
			#define LED14      0x3E
			#define LED15      0x42

			
			void I2C_PCA_Configuration(void);
			void I2C_PCA_WriteByte(uint8_t addr,uint8_t data);

			
			void PCA9685_Init(void);
			void PCA9685_Set_PWM(u8 Address,unsigned int angle);

#endif
