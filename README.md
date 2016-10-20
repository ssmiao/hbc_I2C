# hbc_I2C
A simple soft i2c  for stm32
# 1、所有文件、全局变量、函数和宏的名称均以hbc_开头，以防与其他库重名，你可以根据自己的喜好改成其他。

# 2、初始化方式

使用hbc_I2C_Type结构体和hbc_I2C_Init()函数来进行I2C的初始化。他们的定义分别如下。
typedef struct hbc_I2C_struct
{
		GPIO_TypeDef* 		GPIOx;
			uint16_t 		SCL;
			uint16_t 		SDA;
}hbc_I2C_Type;

int hbc_I2C_Init(struct hbc_I2C_struct* I2Cx);

下面是一个初始化的例子，这段代码将创建一个名为I2C_1的I2C端口，其数据线SDA为GPIOB7、时钟线为GPIOB6。
hbc_I2C_Type I2C_1;
I2C_1.GPIOx = GPIOB;
I2C_1.SCL = GPIO_Pin_6;
I2C_1.SDA = GPIO_Pin_7;
	
hbc_I2C_Init(&I2C_1);

可以使用任何GPIO来创建I2C端口，并且可以创建和使用任意个I2C端口，。

# 3、发出开始信号

void hbc_I2C_Start(struct hbc_I2C_struct* I2Cx);

# 4、发出结束信号

void hbc_I2C_Stop(struct hbc_I2C_struct* I2Cx);

# 5、发送地址和WR控制位（需要检查应答信号）

#define hbc_I2C_Write 0x00
#define hbc_I2C_Read 0x01
void hbc_I2C_SendAddress(struct hbc_I2C_struct* I2Cx,uint8_t address,uint8_t WR);

例如

hbc_I2C_SendAddress(&I2C_1,0x50,hbc_I2C_Write);/* 地址为0x50,写 */
hbc_I2C_SendAddress(&I2C_1,0x50,hbc_I2C_Write);/* 地址为0x50,读 */

# 6、发送数据(需要检查应答信号)

void hbc_I2C_SendData(struct hbc_I2C_struct* I2Cx,uint8_t data);

也可以使用这个函数发送地址和WR控制位，这种把地址和WR控制位合在一起当作地址的方式更为常见。
hbc_I2C_SendData(&I2C_1,0xa0);/* 地址为0x50,写 */
hbc_I2C_SendData(&I2C_1,0xa1);/* 地址为0x50,读 */

# 7、检查应答信号

int hbc_I2C_CheckACK(struct hbc_I2C_struct* I2Cx);
int hbc_I2C_WaitACK(struct hbc_I2C_struct* I2Cx,uint32_t timeout);

hbc_I2C_CheckACK()是一个已经被废弃的函数，功能是检查一次有没有应答信号，有返回1、无返回0。连续调用多次这个函数十分危险，最好不要使用这个函数来等待应答信号。

hbc_I2C_WaitACK()等待答信号，收到应答信号到则返回1、超时否则返回0。
timeout决定hbc_I2C_WaitACK()决定超时时间，但不是精确的时间值，而是单纯的递减。

# 8、接收数据(需要返回应答信号或非应答信号)

uint8_t hbc_I2C_ReceiveData(struct hbc_I2C_struct* I2Cx);

返回接收到的数据。

# 9、发送应答、非应答信号

void hbc_I2C_SendACK(struct hbc_I2C_struct* I2Cx);
void hbc_I2C_SendNOTACK(struct hbc_I2C_struct* I2Cx);
