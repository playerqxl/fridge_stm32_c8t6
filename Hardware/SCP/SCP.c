#include "global.h"

unsigned short scp_time_start;//连续工作计时
unsigned char scp_dhtj;
//scpi半导体制冷片继电器输入线开启标志位,scp_time_stop为半导体制冷片长时间工作错误标志位
unsigned char scpi,scp_time_stop;
char scp_set_temper[tempers],scp_temper[tempers];

void SCP_Configuration(void)
{
	/*设置A10为半导体制冷片继电器输入线*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOA时钟
	//配置结构体
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	scpi=scp_time_start=scp_time_stop=0;
}

/*至少成功更新一次温度且连续工作未超时;设定温度小于实际温度启动半导体制冷片继电器，
温度从高到低时，设定温度大于实际温度-5关闭半导体制冷片继电器；
温度从低到高时，设定温度大于实际温度关闭半导体制冷片继电器。
开漏，高阻态为set，接通继电器，接地为reset，断开继电器。*/
void SCP_run(void)
{
	char scpt;//1为温度从高到低，0为温度从低到高
	scp_dhtj=dhtj;
	if(scp_dhtj&&!scp_time_stop)
	{
		scp_set_temper[0]=set_temper[0];
		scp_temper[0]=temper[0];
		if(scp_set_temper[0]<scp_temper[0])
		{
			scpt=1;
			GPIO_SetBits(GPIOA, GPIO_Pin_4);
//			SCP_on();
			scpi=1;
		}
		else 
		if((scp_set_temper[0]-5)>scp_temper[0])
		{
			scpt=0;
			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
//			SCP_off();
			scpi=0;
		}
		else
//温度处于中间位置看scpt
		if(scpt){GPIO_SetBits(GPIOA, GPIO_Pin_4);scpi=1;}else{GPIO_ResetBits(GPIOA, GPIO_Pin_4);scpi=0;}	
	}
}

void SCP_on(void)//下拉输入时接地，继电器线圈闭合
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//配置结构体
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//下拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void SCP_off(void)//推挽输出，有二极管反向截止，继电器线圈断开
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//配置结构体
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

///*永久关闭连续工作超过240分钟的半导体制冷片*/
//void SCP_time_stop(void)
//{
//	unsigned char scp_hour=0;
//	if(scpi)
//	{
//		scp_time_start++;
//		if(scp_time_start>35999)
//		{
//			scp_time_start=0;
//			scp_hour++;
//			if(scp_hour>3)
//			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
//			scpi=0;
//			scp_time_stop=1;
//			scp_hour=0;
//		}
//	}
//	else
//	{
//		scp_time_start=scp_hour=0;
//	}
//}
