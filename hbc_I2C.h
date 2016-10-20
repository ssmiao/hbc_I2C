#ifndef __HBC_I2C_H__
#define __HBC_I2C_H__

#include <stdint.h>

#define hbc_I2C_Write 0x00
#define hbc_I2C_Read 0x01

typedef struct hbc_I2C_struct
{
	GPIO_TypeDef* 	GPIOx;
	uint16_t 		SCL;
	uint16_t 		SDA;
}hbc_I2C_Type;

int hbc_I2C_Init(struct hbc_I2C_struct* I2Cx);
void hbc_I2C_Start(struct hbc_I2C_struct* I2Cx);
void hbc_I2C_Stop(struct hbc_I2C_struct* I2Cx);
void hbc_I2C_SendAddress(struct hbc_I2C_struct* I2Cx,uint8_t address,uint8_t WR);
void hbc_I2C_SendData(struct hbc_I2C_struct* I2Cx,uint8_t data);
int hbc_I2C_CheckACK(struct hbc_I2C_struct* I2Cx);
int hbc_I2C_WaitACK(struct hbc_I2C_struct* I2Cx,uint32_t timeout);
uint8_t hbc_I2C_ReceiveData(struct hbc_I2C_struct* I2Cx);
void hbc_I2C_SendACK(struct hbc_I2C_struct* I2Cx);
void hbc_I2C_SendNOTACK(struct hbc_I2C_struct* I2Cx);

#endif
