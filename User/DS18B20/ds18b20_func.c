#include "ds18b20_func.h"

/*
//函数名：DS18B20_Mode_IPU()
//功能：配置DQ总线为上拉输入模式
//输入：null
//输出：null
//补充：
*/
static void DS18B20_Mode_IPU(void)
{
	/*初始化单总线口*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_DS18B20_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIO_DS18B20_PORT, &GPIO_InitStruct);
}


/*
//函数名：DS18B20_Mode_OutPP()
//功能：配置DQ总线为推挽输出模式
//输入：null
//输出：null
//补充：
*/
static void DS18B20_Mode_OutPP(void)
{

	/*初始化单总线口*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_DS18B20_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_DS18B20_PORT, &GPIO_InitStruct);
}


/*
//函数名：DS18B20_Config()
//功能：配置初始化DS18B20数据口，设置为高电平输出
//输入：null
//输出：null
//补充：null
*/
static void DS18B20_Config(void)
{
	/*使能RCC*/
	RCC_APB2PeriphClockCmd(RCC_GPIO_DS18B20, ENABLE);
	DS18B20_Mode_OutPP();
	/*输出高电平*/
	DS18B20_DATA_OUT(HIGH);
}


/*
//函数名：DS18B20_Reset()
//功能：复位DS18B20
//输入：null
//输出：null
//补充：从高电平设置到低电平，持续最少480μs，等待15-60μs，再将电平拉高；
*/
static void DS18B20_Reset(void)
{
	DS18B20_Mode_OutPP();
	DS18B20_DATA_OUT(LOW);
	Delay_us(500);
	DS18B20_DATA_OUT(HIGH);
}


/*
//函数名：Check_Presence()
//功能：检测存在脉冲（低电平）
//输入：null
//输出：0 --- 成功检测到存在脉冲
				1 --- 等待存在脉冲超时
//			2 --- 存在脉冲过长
//补充：主机变成输入模式至少480μs
*/
static uint8_t DS18B20_Check_Presence(void)
{
	DS18B20_Mode_IPU();
	uint32_t Counter_Time = 0, Delay_Time = 500;
	while(GPIO_ReadInputDataBit(GPIO_DS18B20_PORT, GPIO_DS18B20_PIN) )
	{
		if(Counter_Time > 100)
		{
			return 1;
		}
		else
		{
			Delay_us(1);
			Counter_Time++;
		}
	}
	Delay_Time -= Counter_Time;
	Counter_Time = 0;
	while(!GPIO_ReadInputDataBit(GPIO_DS18B20_PORT, GPIO_DS18B20_PIN) )
	{
		
		if(Counter_Time > 240)
		{
			return 2;
		}
		else	
		{
				Delay_us(1);
				Counter_Time++;
		}
	}
	Delay_Time -= Counter_Time;
	Delay_us(Delay_Time);
	return 0;
}


/*
//函数名：DS18B20_WriteBit()
//功能：发送一位值到模块
//输入：OneBitData --- 即将发送到模块的值
//输出：0 --- 发送完毕
//  		1 --- 发送出错，该值非0或1
//补充：总延时在100μs左右。
//  		写0：
//					输出低电平启动写时序，1μs< Time <15μs；
//					继续输出低电平（从机在这个窗口期进行采样），15μs<从机采样时间<45μs，整个写0过程必须在控制在60μs< Time <120μs；
//					写0完毕后输出高电平以表示本次写时序结束，且1μs<Time<∞；
//
//			写1：
//					输出低电平启动写时序，1μs< Time <15μs；
//					输出高电平（从机在这个窗口期进行采样），15μs<从机采样时间<45μs，全程时间安排与写0相同为最佳选择。
//			结束时序:
//		  		写0完毕后输出高电平以表示本次写时序结束，且1μs<Time<∞；写1完毕后则直接延迟同样时间
//
*/
static uint8_t DS18B20_WriteBit(uint8_t OneBitData)
{
	DS18B20_Mode_OutPP();
	if(OneBitData == 0)
	{
		DS18B20_DATA_OUT(LOW);
		/*10+85*/
		Delay_us(95);
		DS18B20_DATA_OUT(HIGH);
		Delay_us(5);
	}
	else if(OneBitData == 1)
	{
		DS18B20_DATA_OUT(LOW);
		Delay_us(10);
		DS18B20_DATA_OUT(HIGH);
		Delay_us(90);
	}
	else
		return 1;
	return 0;		
}


/*
//函数名：DS18B20_WriteByte()
//功能：获取指令值每一位，并从低位到高位分别发送模块
//输入：Data --- 指令值
//输出：null
//补充：null
*/
static void DS18B20_WriteByte(uint8_t Data)
{
	uint8_t Counter = 0, OneBitData = 0;
	for(; Counter < 8; Counter++)
	{
		OneBitData = Data & 0x01;
		DS18B20_WriteBit(OneBitData);
		Data = Data >> 1;
	}
}


/*
//函数名：DS18B20_ReadBit()
//功能：读取一个数据位
//输入：null
//输出：读取到的数据位
//补充：总延迟100μs。
//			读0、读1：
//					输出低电平启动读时序，该时序时间：1μs< Time <15μs；
//					设置为上拉输入模式，在上一步骤的15μs内开始读值，等待45μs（期间模块会自动上拉电阻变成高电平）；
//					模块自动上拉电阻，恢复高电平；读数据完毕后输出高电平以表示本次读数据结束，且维持1μs<Time<∞；；
*/
static uint8_t DS18B20_ReadBit()
{
	DS18B20_Mode_OutPP();
	uint8_t OneBitData;
	DS18B20_DATA_OUT(LOW);
	DS18B20_Mode_IPU();
	Delay_us(13);
	OneBitData = GPIO_ReadInputDataBit(GPIO_DS18B20_PORT, GPIO_DS18B20_PIN);
	Delay_us(45+42);
	return OneBitData;
}


/*
//函数名：DS18B20_ReadByte()
//功能：读取一个字节
//输入：null
//输出：Data --- 读取到的数据
//补充：null
*/
static uint8_t DS18B20_ReadByte()
{
	DS18B20_DATA_OUT(HIGH);
	uint8_t Counter = 0, Data = 0;
	for(; Counter < 8; Counter++)
		Data |= DS18B20_ReadBit() << Counter;
	return Data;
}


/*
//函数名：DS18B20_Init()
//功能：初始化DS18B20
//输入：null
//输出：null
//补充：null
*/
uint8_t DS18B20_Init(void)
{
	DS18B20_Config();
	DS18B20_Reset();
	return (DS18B20_Check_Presence());
}


/*
//函数名：DS18B20_ConvertTemp
//功能：命令模块进行温度转换（获取温度）
//输入：null
//输出：null
//补充：null
*/
static void DS18B20_ConvertTemp(void)
{
	DS18B20_Reset();
	DS18B20_Check_Presence();
	DS18B20_WriteByte(0xCC);
	DS18B20_WriteByte(0x44);
}

/*
//函数名：DS18B20_GetTemp()
//功能：获得当前温度最终值
//输入：null
//输出：float型温度值
//补充：null
*/
float DS18B20_GetTemp(void)
{
	float Temp = 0;
	uint8_t H_Temp, L_Temp;
	short Total_Temp = 0;
	DS18B20_ConvertTemp();
	
	DS18B20_Reset();
	DS18B20_Check_Presence();
	DS18B20_WriteByte(0xCC);
	DS18B20_WriteByte(0xBE);
	L_Temp = DS18B20_ReadByte();
	H_Temp = DS18B20_ReadByte();
	Total_Temp |= H_Temp << 8;
	Total_Temp |= L_Temp;
	
	if(Total_Temp < 0)
		Temp = 0- ( (~Total_Temp + 1) * 0.0625 );
	else
		Temp = Total_Temp * 0.0625;

	return Temp;
}
