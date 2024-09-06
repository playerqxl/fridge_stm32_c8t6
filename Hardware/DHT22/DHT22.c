#include "global.h"
volatile float temper[tempers],humid[tempers];//存放温度数据和湿度数据
float settemper[tempers];//存放设置温度数据设置温度数据
float set_temper[tempers];//应用设置温度数据，用于与实际温度比较
volatile unsigned char tempersplusj,tempersminusj;//存储温度设置连续加减时plusi和minusi的上次量
volatile unsigned char tempersplusi,tempersminusi;//存储温度设置连续加减时plusi和minusi的本次量
unsigned char dhti,dhtj;//dhti每次更新温度需要更新两次的标志位，dhtj至少更新成功一次温度
/*dht22_tst为传输信号各个阶段标志位，0为不进行传输信号，1为传输开始前3s延时，2为主机开始发送信号，3为主机拉高并延时等待，
4为DHT22响应拉低输出，5为DHT拉高延时准备输出，6为DHT22传输1bit数据时拉低，
7为DHT22传输1bit数据时拉高,8为信号传输结束传感器释放总线，9为校验阶段。
dht22_tst_err为传输信号各个阶段错误标志位，数字为对应dht22_tst的各个阶段超时错误。*/
volatile unsigned char dht22_tst,dht22_tst_err;
unsigned char dht22_tst_errcount;
volatile unsigned char dht22_ts_i;//温度传感器传输bit数
unsigned short dht22_time_count;//刷新时间计数
/*湿度高8位+湿度低8位+温度高8位+温度低8位=校验和*/
volatile unsigned short temph_temper[tempers],templ_temper[tempers],temph_humid[tempers],templ_humid[tempers],checksum[tempers];
volatile int dcnt,d0cnt;

void DHT22_IPU(void)
{
	//配置结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DHT22_PP(void)
{
	//配置结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
初始化DHT22温度传感器，需要使能外部信号线，配置GPIO外部信号线APB2时钟，GPIOA口时钟开启
*/
void DHT22_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	DHT22_0_Int_Init();//温度传感器0的GPIO口配置
	temper[currtemper0]=humid[currtemper0]=temper[currtemper1]=humid[currtemper1]=-99;//初始化温湿度
	set_temper[currtemper0]=set_temper[currtemper1]=5;//冷藏温度5℃
//	settemper[currtemper1]=-5;//冷冻温度-5℃
	settemper[currtemper0]=settemper[currtemper1]=5;//冷藏温度5℃	
//	set_temper[currtemper1]=-5;//冷冻温度-5℃
	dht22_tst_err=dht22_ts_i,dht22_tst_errcount=dht22_time_count=tempersplusi=tempersminusi=tempersplusj=tempersminusj=dhti=dhtj=0;
	dht22_tst=1;//一开始就要刷新一次温度
}

/*
初始化DHT11温度传感器0，PC12为传感器0的信号线,配置结构体。按照DHT22说明书，信号线有5V和5KΩ电阻，所以为开漏模式,初始化PB8为高电平。
*/
void DHT22_0_Int_Init(void)
{
	DHT22_PP();
	//设置PA0为高电平
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

/*同步温度设置，温度设定冷冻温度不能小于冷藏温度*/
void set_tempers(void)
{
	if(settemper[currtemper1]>=settemper[currtemper0])
	{
		set_temper[currtemper0]=settemper[currtemper0];
		set_temper[currtemper1]=settemper[currtemper1];
	}
	else
		settemper[currtemper1]=settemper[currtemper0];
		set_temper[currtemper0]=settemper[currtemper0];
		set_temper[currtemper1]=settemper[currtemper1];
}

/*放弃同步温度设置，还原应用设置温度数据*/
void reset_tempers(void)
{
	settemper[currtemper0]=set_temper[currtemper0];
	settemper[currtemper1]=set_temper[currtemper1];
}

void plus_tempers(void)
{
	tempersplusi=tempersplusj=0;//清除
	if(seti==2)
	{
	settemper[currtemper0]++;
	}
	if(seti==3)
	{
	settemper[currtemper1]++;
	}
}

/*由于入口函数是轮询法，所以每次执行此长按加法函数都要只计算本次加法增量。*/
void plus_count_tempers(void)
{
	volatile unsigned char dht22plusi;
	dht22plusi=plusi-5;//plusi至少为5，因此需要消除5
	tempersplusj=tempersplusi;//记录上次量
	tempersplusi=dht22plusi;//记录本次量
	if(seti==2)
	{
	settemper[currtemper0]+=tempersplusi-tempersplusj;
	}
	if(seti==3)
	{
	settemper[currtemper1]+=tempersplusi-tempersplusj;
	}
}

void minus_tempers(void)
{
	tempersminusi=tempersminusj=0;
	if(seti==2)
	{
	settemper[currtemper0]--;
	}
	if(seti==3)
	{
	settemper[currtemper1]--;
	}
}

/*由于入口函数是轮询法，所以每次执行此长按减法函数都要只计算本次减法增量。*/
void minus_count_tempers(void)
{
	volatile unsigned char dht22minusi;
	dht22minusi=minusi-5;//plusi至少为5，因此需要消除5
	tempersminusj=tempersminusi;//记录上次量
	tempersminusi=dht22minusi;//记录本次量
	if(seti==2)
	{
	settemper[currtemper0]-=tempersminusi-tempersminusj;
	}
	if(seti==3)
	{
	settemper[currtemper1]-=tempersminusi-tempersminusj;
	}
}

void DHT22_get(void)
{
	unsigned char temp_tst;
	temp_tst=dht22_tst;
	switch(temp_tst)
	{
		case 1://定时3s
			TIM4_TInt_Init(29999,7199);
			DHT22_PP();
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			dht22_tst=2;//等待中断
		break;
		case 9:DHT22_checksum();break;
		case 10://为了防止传输结束时DHT22还在传输数据，定时100ms等待传输结束
			TIM4_TInt_Init(999,7199);
			dht22_tst=101;
		break;
		default:break;
	}
}

/*DHT22传输信号中断函数调用函数。
dht22_tst为传输信号各个阶段标志位，0为不进行传输信号，1为传输开始前3s延时，2为主机开始发送信号，3为主机拉高并延时等待，
4为DHT22响应拉低输出，5为DHT拉高延时准备输出，6为DHT22再拉低延时准备输出信号“0”、“1”低电平，7为DHT22拉高输出信号“0”、“1”高电平（
这里的时间用于判断传输“0”、“1”），8为判断信号“0”、“1”，再根据传输次数选择返回7还是去到9,9为信号传输结束传感器释放总线，10为校验阶段。
dht22_tst_err为传输信号各个阶段错误标志位，数字为对应dht22_tst的各个阶段超时错误。
*/
void DHT22_ts(void)
{
	unsigned char temp_ts;
	temp_ts=dht22_tst;
	dcnt=TIM_GetCounter(TIM4);//获取定时器计数
	TIM_SetCounter(TIM4,0);//重置定时器计数为0
	switch(temp_ts)
	{
		case 4:dht22_tst=5;TIM_SetAutoreload(TIM4,149);break;//DHT22响应拉低输出阶段,重装载TIM4定时器超时150us
		case 5:dht22_tst=6;break;//DHT拉高延时准备输出阶段,实测68
		case 6:dht22_tst=7;break;//DHT22再拉低，准备输出信号“0”、“1”低电平，实测71
		case 7:dht22_tst=8;break;//输出信号“0”、“1”低电平，并拉高，时间最长55us，实测47
		case 8:DHT22_ts_bit();break;//输出信号“0”、“1”高电平拉低（传输1bit结束），“0”时间最长30us，“1”时间最长75us，实测23
//		case 8:d0cnt=dcnt;DHT22_tst_err();break;
		default:break;
	}
}

void DHT22_tst_stop(void)
{
	unsigned char temp_tst_stop;
	temp_tst_stop=dht22_tst;
	switch(temp_tst_stop)
		{
		case 1:break;
		case 2://定时3s结束，设置主机起始信号拉低时间20ms
			TIM4_TInt_Init(19999,71);
			dht22_tst=3;
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		break;
		case 3://2拉低时间20ms结束，主机起始信号拉高超时是1的20ms
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			dht22_tst=4;
			DHT22_IPU();
			EXTIX_ENABLE(GPIO_PortSourceGPIOA,GPIO_PinSource0,EXTI_Line0);
		break;
//		case 9:break;
		case 10:break;
		case 101://停止计时，重置PA0端口
			TIM_Cmd(TIM4, DISABLE);  //停止计时
			TIM_SetCounter(TIM4,0);//重置计数,
			DHT22_PP();
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			if(dht22_tst_err|dhti){dht22_tst=1;}//如果有错误代码或只正常传输一次数据，则重新传输数据
			else{dht22_tst=0;}
		break;
			
//		default:break;
		default:DHT22_tst_err();break;//其他阶段响应超时，进入错误处理程序
		}
}

void DHT22_ts_bit(void)//传输温度传感器bit数据,计数器为向上计数，信号“0”高电平时间22-30us,信号“1”高电平时间68-75us
{
	unsigned char dht22_ts_count;
	dht22_ts_count=dht22_ts_i;
	//	if(dcnt>=40&&dcnt<=60){DHT22_tst_err();}//如果都不是，则错误
/*湿度高8位+湿度低8位+温度高8位+温度低8位=校验和，将对应数据写入临时变量，以待校验*/
	if(dht22_ts_count==0){d0cnt=dcnt+20;temph_humid[0]=temph_humid[0]<<1;}//湿度最高位必定为0，dcn与d0cnt比较小就是0，大就是1
	else if(1<=dht22_ts_count&&dht22_ts_count<8)
		{if(dcnt<d0cnt){temph_humid[0]=temph_humid[0]<<1;}else{temph_humid[0]=temph_humid[0]<<1|1;}}
	else if(8<=dht22_ts_count&&dht22_ts_count<16)
		{if(dcnt<d0cnt){templ_humid[0]=templ_humid[0]<<1;}else{templ_humid[0]=templ_humid[0]<<1|1;}}
	else if(16<=dht22_ts_count&&dht22_ts_count<24)
		{if(dcnt<d0cnt){temph_temper[0]=temph_temper[0]<<1;}else{temph_temper[0]=temph_temper[0]<<1|1;}}
	else if(24<=dht22_ts_count&&dht22_ts_count<32)
		{if(dcnt<d0cnt){templ_temper[0]=templ_temper[0]<<1;}else{templ_temper[0]=templ_temper[0]<<1|1;}}
	else {if(dcnt<d0cnt){checksum[0]=checksum[0]<<1;}else{checksum[0]=checksum[0]<<1|1;}}
	
	if(dht22_ts_count<39)//判断计数次数是否到40次，跳转到相应阶段
	{
		dht22_tst=7;
		dht22_ts_i++;//传送bit计数
	}
	else
	{
		TIM_Cmd(TIM4, DISABLE);
		TIM_SetCounter(TIM4,0);
		EXTIX_DISABLE(GPIO_PortSourceGPIOA,GPIO_PinSource0,EXTI_Line0);
		dht22_tst=9;
		dht22_ts_i=0;
	}
}

void DHT22_checksum(void)
{
	volatile unsigned short temp,ddp0i,tempa;
	temp=(temph_temper[0]+templ_temper[0]+temph_humid[0]+templ_humid[0])&0XFF;//求和后可能会溢出8位，要将9-16位置0
	
//	temper[0]=temph_temper[0];
//	temper[1]=templ_temper[0];
//	humid[0]=temph_humid[0];
//	humid[1]=templ_humid[0];
//	settemper[currtemper0]=checksum[0];
//	set_temper[currtemper0]=temp;
	
	if(temp==0){DHT22_tst_err();}//如果数据全为0也是错
	else 
if(temp!=checksum[0]){DHT22_tst_err();}//校验和
	else//写入温湿度数据
	{
		temph_temper[0]=temph_temper[0]<<8;
		tempa=temph_temper[0]&32768;
		if(tempa)//判断温度第16位是否为1,32768代表二进制第16位为1，其余为0，存放的温湿度数据为传输数据的1/10
		{
			temper[0]=(temph_temper[0]&32767)+templ_temper[0];//32767代表二进制第16位为0，其余为1，这里不/是因为整数除法不保留余数
			temper[0]=temper[0]/-10;
		}
		else{temper[0]=temph_temper[0]+templ_temper[0];temper[0]=temper[0]/10;}
		//湿度
		humid[0]=(temph_humid[0]<<8)+templ_humid[0];
		humid[0]=humid[0]/10;		
		//需要连续更新两次才能获得最新温度，dhti为0时继续更新温度,dhti为1时更新完成
		if(dhti)
		{
			dhti=0;
			dhtj=1;
			//清除标志位
			dht22_tst=0;
			dht22_tst_err=0;
			ddp0i=dp0i;
			if(ddp0i==0){dpi=1;}
		}
		else
		{
			dhti=1;
			dht22_tst=10;
			dht22_tst_err=0;
		}
	}
	temph_temper[0]=templ_temper[0]=temph_humid[0]=templ_humid[0]=checksum[0]=0;//由于是short，下次更新数据时残留本次数据，因此清空
}

/*处理温度传感器出错，出错代码=传输到某一阶段，并重新传输，传输出错超过3次，放弃本次更新温度*/
void DHT22_tst_err(void)
{
	EXTIX_DISABLE(GPIO_PortSourceGPIOA,GPIO_PinSource0,EXTI_Line0);//关闭A0外部中断线
	TIM_Cmd(TIM4, DISABLE);  //关闭定时
	TIM_SetCounter(TIM4,0);;//重置计数器，由于向上计数，所以为0
	dht22_tst_err=dht22_tst;
	dht22_tst_errcount++;
	if(dht22_tst_errcount<=3){dht22_tst=10;dht22_time_count=0;}
	else{dht22_tst=dht22_time_count=0;}
}

void DHT22_time(void)//开屏每隔20秒刷新一次温度，黑屏每隔1.5分钟刷新一次温度，系统时钟频率10Hz，因此计数=10*秒
{
	unsigned char tdptj;
	tdptj=dptj;
	dht22_time_count++;
	if(dht22_time_count>199)
	{
		if(tdptj==0)
		{
			dht22_tst=1;
			dht22_time_count=0;
		}
		else if(dht22_time_count>899)
		{
			dht22_tst=1;
			dht22_time_count=0;
		}
	}
	
}
