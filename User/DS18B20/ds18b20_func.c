#include "ds18b20_func.h"

/*
//��������DS18B20_Mode_IPU()
//���ܣ�����DQ����Ϊ��������ģʽ
//���룺null
//�����null
//���䣺
*/
static void DS18B20_Mode_IPU(void)
{
	/*��ʼ�������߿�*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_DS18B20_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIO_DS18B20_PORT, &GPIO_InitStruct);
}


/*
//��������DS18B20_Mode_OutPP()
//���ܣ�����DQ����Ϊ�������ģʽ
//���룺null
//�����null
//���䣺
*/
static void DS18B20_Mode_OutPP(void)
{

	/*��ʼ�������߿�*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_DS18B20_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_DS18B20_PORT, &GPIO_InitStruct);
}


/*
//��������DS18B20_Config()
//���ܣ����ó�ʼ��DS18B20���ݿڣ�����Ϊ�ߵ�ƽ���
//���룺null
//�����null
//���䣺null
*/
static void DS18B20_Config(void)
{
	/*ʹ��RCC*/
	RCC_APB2PeriphClockCmd(RCC_GPIO_DS18B20, ENABLE);
	DS18B20_Mode_OutPP();
	/*����ߵ�ƽ*/
	DS18B20_DATA_OUT(HIGH);
}


/*
//��������DS18B20_Reset()
//���ܣ���λDS18B20
//���룺null
//�����null
//���䣺�Ӹߵ�ƽ���õ��͵�ƽ����������480��s���ȴ�15-60��s���ٽ���ƽ���ߣ�
*/
static void DS18B20_Reset(void)
{
	DS18B20_Mode_OutPP();
	DS18B20_DATA_OUT(LOW);
	Delay_us(500);
	DS18B20_DATA_OUT(HIGH);
}


/*
//��������Check_Presence()
//���ܣ����������壨�͵�ƽ��
//���룺null
//�����0 --- �ɹ���⵽��������
				1 --- �ȴ��������峬ʱ
//			2 --- �����������
//���䣺�����������ģʽ����480��s
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
//��������DS18B20_WriteBit()
//���ܣ�����һλֵ��ģ��
//���룺OneBitData --- �������͵�ģ���ֵ
//�����0 --- �������
//  		1 --- ���ͳ�����ֵ��0��1
//���䣺����ʱ��100��s���ҡ�
//  		д0��
//					����͵�ƽ����дʱ��1��s< Time <15��s��
//					��������͵�ƽ���ӻ�����������ڽ��в�������15��s<�ӻ�����ʱ��<45��s������д0���̱����ڿ�����60��s< Time <120��s��
//					д0��Ϻ�����ߵ�ƽ�Ա�ʾ����дʱ���������1��s<Time<�ޣ�
//
//			д1��
//					����͵�ƽ����дʱ��1��s< Time <15��s��
//					����ߵ�ƽ���ӻ�����������ڽ��в�������15��s<�ӻ�����ʱ��<45��s��ȫ��ʱ�䰲����д0��ͬΪ���ѡ��
//			����ʱ��:
//		  		д0��Ϻ�����ߵ�ƽ�Ա�ʾ����дʱ���������1��s<Time<�ޣ�д1��Ϻ���ֱ���ӳ�ͬ��ʱ��
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
//��������DS18B20_WriteByte()
//���ܣ���ȡָ��ֵÿһλ�����ӵ�λ����λ�ֱ���ģ��
//���룺Data --- ָ��ֵ
//�����null
//���䣺null
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
//��������DS18B20_ReadBit()
//���ܣ���ȡһ������λ
//���룺null
//�������ȡ��������λ
//���䣺���ӳ�100��s��
//			��0����1��
//					����͵�ƽ������ʱ�򣬸�ʱ��ʱ�䣺1��s< Time <15��s��
//					����Ϊ��������ģʽ������һ�����15��s�ڿ�ʼ��ֵ���ȴ�45��s���ڼ�ģ����Զ����������ɸߵ�ƽ����
//					ģ���Զ��������裬�ָ��ߵ�ƽ����������Ϻ�����ߵ�ƽ�Ա�ʾ���ζ����ݽ�������ά��1��s<Time<�ޣ���
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
//��������DS18B20_ReadByte()
//���ܣ���ȡһ���ֽ�
//���룺null
//�����Data --- ��ȡ��������
//���䣺null
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
//��������DS18B20_Init()
//���ܣ���ʼ��DS18B20
//���룺null
//�����null
//���䣺null
*/
uint8_t DS18B20_Init(void)
{
	DS18B20_Config();
	DS18B20_Reset();
	return (DS18B20_Check_Presence());
}


/*
//��������DS18B20_ConvertTemp
//���ܣ�����ģ������¶�ת������ȡ�¶ȣ�
//���룺null
//�����null
//���䣺null
*/
static void DS18B20_ConvertTemp(void)
{
	DS18B20_Reset();
	DS18B20_Check_Presence();
	DS18B20_WriteByte(0xCC);
	DS18B20_WriteByte(0x44);
}

/*
//��������DS18B20_GetTemp()
//���ܣ���õ�ǰ�¶�����ֵ
//���룺null
//�����float���¶�ֵ
//���䣺null
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
