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

	/*STM32F103RCT6оƬ��Ӳ��I2C2: PB10 -- SCL; PB11 -- SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C���뿪©���
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C2);//ʹ��I2C1
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;//������I2C��ַ,���д��
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;//400K

	I2C_Cmd(I2C2, ENABLE);
	I2C_Init(I2C2, &I2C_InitStructure);
}
void I2C_PCA_WriteByte(uint8_t addr,uint8_t data)
{
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C2, ENABLE);//����I2C1
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	I2C_Send7bitAddress(I2C2, PCA9685Address, I2C_Direction_Transmitter);//������ַ -- Ĭ��0x78
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C2, addr);//�Ĵ�����ַ
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C2, data);//��������
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C2, ENABLE);//�ر�I2C1����
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

void PCA9685_Set_PWM(u8 Address,unsigned int angle)    //  (angle-10)*0.005ms  �ߵ�ƽ����ʱ��
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
