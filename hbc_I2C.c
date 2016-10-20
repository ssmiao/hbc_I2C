/*
 *	Author  : hubenchang0515@outlook.com
 *	Date    : 2015-11-27
 *  Version : Beta2.0
 *	MCU     : STM32F10x
 */
#include <stm32f10x.h>
#include <hbc_I2C.h>

static void hbc_I2C_delay()
{
	uint8_t i = 10;
	while(i--);
}

/* Keep Evey Function Let SCL to be Low before return. */


int hbc_I2C_Init(struct hbc_I2C_struct* I2Cx)
{
	/* Define a GPIO initialization structure */
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Enable the GPIO's Clock */
	{
		if(I2Cx->GPIOx == GPIOA)
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		else if(I2Cx->GPIOx == GPIOB)
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		else if(I2Cx->GPIOx == GPIOC)
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		else if(I2Cx->GPIOx == GPIOD)
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
		else if(I2Cx->GPIOx == GPIOE)
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
		else if(I2Cx->GPIOx == GPIOF)
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
		else if(I2Cx->GPIOx == GPIOG)
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
		else
			return 0;
	}

	/* GPIO's Pin */
	GPIO_InitStruct.GPIO_Pin = I2Cx->SCL | I2Cx->SDA;
	/* GPIO's Mode */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	/* GPIO's Speed */
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	/* GPIO Initialized */
	GPIO_Init(I2Cx->GPIOx,&GPIO_InitStruct);

	/* I2C BUS Initialized */
	GPIO_ResetBits(I2Cx->GPIOx , I2Cx->SCL);
	GPIO_SetBits(I2Cx->GPIOx , I2Cx->SDA);


	/* Reset all */
	hbc_I2C_Stop(I2Cx);

	return 1;
}




void hbc_I2C_Start(struct hbc_I2C_struct* I2Cx)
{
	/* That SDA is becoming Low from High,when SCL is High,means START. */

	/* Let SDA to Be High */
	GPIO_SetBits(I2Cx->GPIOx , I2Cx->SDA);
	hbc_I2C_delay();

	/* Let SCL to be High */
	GPIO_SetBits(I2Cx->GPIOx , I2Cx->SCL);
	hbc_I2C_delay();

	/* Let SDA to be Low */
	GPIO_ResetBits(I2Cx->GPIOx , I2Cx->SDA);
	hbc_I2C_delay();

	/* Let SCL to be Low */
	GPIO_ResetBits(I2Cx->GPIOx , I2Cx->SCL);
	hbc_I2C_delay();

	return;
}





void hbc_I2C_Stop(struct hbc_I2C_struct* I2Cx)
{
	/* That SDA is becoming High from Low,when SCL is High,means STOP. */

	/* Let SDA to Be Low */
	GPIO_ResetBits(I2Cx->GPIOx , I2Cx->SDA);
	hbc_I2C_delay();

	/* Let SCL to he High */
	GPIO_SetBits(I2Cx->GPIOx , I2Cx->SCL);
	hbc_I2C_delay();

	/* Let SDA to be High */
	GPIO_SetBits(I2Cx->GPIOx , I2Cx->SDA);
	hbc_I2C_delay();

	/* Let SCL to be Low */
	GPIO_ResetBits(I2Cx->GPIOx , I2Cx->SCL);
	hbc_I2C_delay();

	return;
}






void hbc_I2C_SendAddress(struct hbc_I2C_struct* I2Cx,uint8_t address,uint8_t WR)
{
	/* High 7 bits of address means the Slave's I2C address. */
	/* Low 1 bit of address means Write(0) or Read(1). */

	uint8_t AddressWithWR = (address<<1) | WR;

	hbc_I2C_SendData(I2Cx,AddressWithWR);

	/* To check the Acknowledgement */
	//return hbc_I2C_CheckACK(I2Cx);
	return;
}






void hbc_I2C_SendData(struct hbc_I2C_struct* I2Cx,uint8_t data)
{

	uint8_t i = 0;

	for(i=0 ; i<8 ; i++)
	{

		if( ( (data<<i) & 0x80) )
			GPIO_SetBits(I2Cx->GPIOx , I2Cx->SDA);
		else
			GPIO_ResetBits(I2Cx->GPIOx , I2Cx->SDA);
		hbc_I2C_delay();

		/* Let SCL to he High , SDA send. */
		GPIO_SetBits(I2Cx->GPIOx , I2Cx->SCL);
		hbc_I2C_delay();

		/* Let SCL to be Low , SDA could change. */
		GPIO_ResetBits(I2Cx->GPIOx , I2Cx->SCL);
		hbc_I2C_delay();
	}

	/* To check the Acknowledgement */
	//return hbc_I2C_CheckACK(I2Cx);
	return;
}






int hbc_I2C_CheckACK(struct hbc_I2C_struct* I2Cx)
{
	int ACK = 0;

	/* Let SDA to be High */
	GPIO_SetBits(I2Cx->GPIOx , I2Cx->SDA);

	/* Let SCL to he High , SDA send. */
	GPIO_SetBits(I2Cx->GPIOx , I2Cx->SCL);
	hbc_I2C_delay();

	/* Read the Acknowledgement */
	ACK = GPIO_ReadInputDataBit(I2Cx->GPIOx , I2Cx->SDA);

	/* Let SCL to be Low , SDA could change. */
	GPIO_ResetBits(I2Cx->GPIOx , I2Cx->SCL);
	hbc_I2C_delay();

	return ACK;
}






int hbc_I2C_WaitACK(struct hbc_I2C_struct* I2Cx,uint32_t timeout)
{
	/* Let SDA to be High */
	GPIO_SetBits(I2Cx->GPIOx , I2Cx->SDA);

	/* Let SCL to he High , SDA send. */
	GPIO_SetBits(I2Cx->GPIOx , I2Cx->SCL);
	hbc_I2C_delay();

	/* Read the Acknowledgement */
	while(GPIO_ReadInputDataBit(I2Cx->GPIOx , I2Cx->SDA) && timeout--);

	/* Let SCL to be Low , SDA could change. */
	GPIO_ResetBits(I2Cx->GPIOx , I2Cx->SCL);
	hbc_I2C_delay();

	return (timeout>0);
}





uint8_t hbc_I2C_ReceiveData(struct hbc_I2C_struct* I2Cx)
{
	uint8_t ReceiveData = 0;
	uint8_t i = 8;

	/* Let SDA to be High , To get input */
	GPIO_SetBits(I2Cx->GPIOx , I2Cx->SDA);
	hbc_I2C_delay();

	for( i=8 ; i>0 ; i--)
	{
		/* Let SCL to he High , SDA Receive. */
		GPIO_SetBits(I2Cx->GPIOx , I2Cx->SCL);
		hbc_I2C_delay();

		ReceiveData <<= 1;
		ReceiveData |= GPIO_ReadInputDataBit(I2Cx->GPIOx , I2Cx->SDA);

		/* Let SCL to be Low , SDA could change. */
		GPIO_ResetBits(I2Cx->GPIOx , I2Cx->SCL);
		hbc_I2C_delay();
	}

	return ReceiveData;
}







void hbc_I2C_SendACK(struct hbc_I2C_struct* I2Cx)
{

	GPIO_ResetBits(I2Cx->GPIOx , I2Cx->SDA);
	hbc_I2C_delay();

	GPIO_SetBits(I2Cx->GPIOx , I2Cx->SCL);
	hbc_I2C_delay();

	GPIO_ResetBits(I2Cx->GPIOx , I2Cx->SCL);
	hbc_I2C_delay();

	GPIO_SetBits(I2Cx->GPIOx , I2Cx->SDA);
	hbc_I2C_delay();
}






void hbc_I2C_SendNOTACK(struct hbc_I2C_struct* I2Cx)
{

	GPIO_SetBits(I2Cx->GPIOx , I2Cx->SDA);
	hbc_I2C_delay();

	GPIO_SetBits(I2Cx->GPIOx , I2Cx->SCL);
	hbc_I2C_delay();

	GPIO_ResetBits(I2Cx->GPIOx , I2Cx->SCL);
	hbc_I2C_delay();
}




int hbc_I2C_CheckBusy(struct hbc_I2C_struct* I2Cx)
{
	return !GPIO_ReadInputDataBit(I2Cx->GPIOx , I2Cx->SDA);
}
