#include "PCA9685.h"
#include "delay.h"
#include "stm32f10x_i2c.h"
#include "Iic.h"

void I2C_PCA_Configuration(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	/*STM32F103RCT6芯片的硬件I2C2: PB10 -- SCL; PB11 -- SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C必须开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C2);//使用I2C1
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;//主机的I2C地址,随便写的
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;//400K

	I2C_Cmd(I2C2, ENABLE);
	I2C_Init(I2C2, &I2C_InitStructure);
}
void I2C_PCA_WriteByte(uint8_t addr,uint8_t data)
{
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C2, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	I2C_Send7bitAddress(I2C2, PCA9685Address, I2C_Direction_Transmitter);//器件地址 -- 默认0x78
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C2, addr);//寄存器地址
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C2, data);//发送数据
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C2, ENABLE);//关闭I2C1总线
}


void PCA9685_Iic_Write(u8 REG_Address,u8 REG_Data)
{
	Iic_Start();
	Iic_Send_Byte(SlaveAddress);
	Iic_Wait_Ack();
	Iic_Send_Byte(REG_Address);
	Iic_Wait_Ack();
	Iic_Send_Byte(REG_Data);
	Iic_Wait_Ack();
	Iic_Stop();
}


void PCA9685_Init()
{
	
	Iic_Init();
	
	
	PCA9685_Iic_Write(Mode1Add,0x10);         
	PCA9685_Iic_Write(PRE_SCALE,0x80);
	delay_ms(1);
	PCA9685_Iic_Write(Mode2Add,0x0c);
	PCA9685_Iic_Write(Mode1Add,0x00);
	
}

void PCA9685_Set_PWM(u8 Address,unsigned int angle)    //  (angle-10)*0.005ms  高电平持续时间
{ 
	
	
	u8 i,j;
	i=angle>>8|0x00;
	j=(angle&0x00ff)|0x00;
	PCA9685_Iic_Write(Address,0x00);       //1/4096  0.005ms
	PCA9685_Iic_Write(Address+1,0x00);
	PCA9685_Iic_Write(Address+2,j);
	PCA9685_Iic_Write(Address+3,i);
	
//	
//	u8 i,j;
//	i=angle>>8|0x00;
//	j=(angle&0x00ff)|0x00;
//	I2C_PCA_WriteByte(Address,0x00);       //1/4096  0.005ms
//	I2C_PCA_WriteByte(Address+1,0x00);
//	I2C_PCA_WriteByte(Address+2,j);
//	I2C_PCA_WriteByte(Address+3,i);

}
