#include "global.h"
volatile float temper[tempers],humid[tempers];//����¶����ݺ�ʪ������
float settemper[tempers];//��������¶����������¶�����
float set_temper[tempers];//Ӧ�������¶����ݣ�������ʵ���¶ȱȽ�
volatile unsigned char tempersplusj,tempersminusj;//�洢�¶����������Ӽ�ʱplusi��minusi���ϴ���
volatile unsigned char tempersplusi,tempersminusi;//�洢�¶����������Ӽ�ʱplusi��minusi�ı�����
unsigned char dhti,dhtj;//dhtiÿ�θ����¶���Ҫ�������εı�־λ��dhtj���ٸ��³ɹ�һ���¶�
/*dht22_tstΪ�����źŸ����׶α�־λ��0Ϊ�����д����źţ�1Ϊ���俪ʼǰ3s��ʱ��2Ϊ������ʼ�����źţ�3Ϊ�������߲���ʱ�ȴ���
4ΪDHT22��Ӧ���������5ΪDHT������ʱ׼�������6ΪDHT22����1bit����ʱ���ͣ�
7ΪDHT22����1bit����ʱ����,8Ϊ�źŴ�������������ͷ����ߣ�9ΪУ��׶Ρ�
dht22_tst_errΪ�����źŸ����׶δ����־λ������Ϊ��Ӧdht22_tst�ĸ����׶γ�ʱ����*/
volatile unsigned char dht22_tst,dht22_tst_err;
unsigned char dht22_tst_errcount;
volatile unsigned char dht22_ts_i;//�¶ȴ���������bit��
unsigned short dht22_time_count;//ˢ��ʱ�����
/*ʪ�ȸ�8λ+ʪ�ȵ�8λ+�¶ȸ�8λ+�¶ȵ�8λ=У���*/
volatile unsigned short temph_temper[tempers],templ_temper[tempers],temph_humid[tempers],templ_humid[tempers],checksum[tempers];
volatile int dcnt,d0cnt;

void DHT22_IPU(void)
{
	//���ýṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DHT22_PP(void)
{
	//���ýṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
��ʼ��DHT22�¶ȴ���������Ҫʹ���ⲿ�ź��ߣ�����GPIO�ⲿ�ź���APB2ʱ�ӣ�GPIOA��ʱ�ӿ���
*/
void DHT22_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	DHT22_0_Int_Init();//�¶ȴ�����0��GPIO������
	temper[currtemper0]=humid[currtemper0]=temper[currtemper1]=humid[currtemper1]=-99;//��ʼ����ʪ��
	set_temper[currtemper0]=set_temper[currtemper1]=5;//����¶�5��
//	settemper[currtemper1]=-5;//�䶳�¶�-5��
	settemper[currtemper0]=settemper[currtemper1]=5;//����¶�5��	
//	set_temper[currtemper1]=-5;//�䶳�¶�-5��
	dht22_tst_err=dht22_ts_i,dht22_tst_errcount=dht22_time_count=tempersplusi=tempersminusi=tempersplusj=tempersminusj=dhti=dhtj=0;
	dht22_tst=1;//һ��ʼ��Ҫˢ��һ���¶�
}

/*
��ʼ��DHT11�¶ȴ�����0��PC12Ϊ������0���ź���,���ýṹ�塣����DHT22˵���飬�ź�����5V��5K�����裬����Ϊ��©ģʽ,��ʼ��PB8Ϊ�ߵ�ƽ��
*/
void DHT22_0_Int_Init(void)
{
	DHT22_PP();
	//����PA0Ϊ�ߵ�ƽ
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

/*ͬ���¶����ã��¶��趨�䶳�¶Ȳ���С������¶�*/
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

/*����ͬ���¶����ã���ԭӦ�������¶�����*/
void reset_tempers(void)
{
	settemper[currtemper0]=set_temper[currtemper0];
	settemper[currtemper1]=set_temper[currtemper1];
}

void plus_tempers(void)
{
	tempersplusi=tempersplusj=0;//���
	if(seti==2)
	{
	settemper[currtemper0]++;
	}
	if(seti==3)
	{
	settemper[currtemper1]++;
	}
}

/*������ں�������ѯ��������ÿ��ִ�д˳����ӷ�������Ҫֻ���㱾�μӷ�������*/
void plus_count_tempers(void)
{
	volatile unsigned char dht22plusi;
	dht22plusi=plusi-5;//plusi����Ϊ5�������Ҫ����5
	tempersplusj=tempersplusi;//��¼�ϴ���
	tempersplusi=dht22plusi;//��¼������
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

/*������ں�������ѯ��������ÿ��ִ�д˳�������������Ҫֻ���㱾�μ���������*/
void minus_count_tempers(void)
{
	volatile unsigned char dht22minusi;
	dht22minusi=minusi-5;//plusi����Ϊ5�������Ҫ����5
	tempersminusj=tempersminusi;//��¼�ϴ���
	tempersminusi=dht22minusi;//��¼������
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
		case 1://��ʱ3s
			TIM4_TInt_Init(29999,7199);
			DHT22_PP();
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			dht22_tst=2;//�ȴ��ж�
		break;
		case 9:DHT22_checksum();break;
		case 10://Ϊ�˷�ֹ�������ʱDHT22���ڴ������ݣ���ʱ100ms�ȴ��������
			TIM4_TInt_Init(999,7199);
			dht22_tst=101;
		break;
		default:break;
	}
}

/*DHT22�����ź��жϺ������ú�����
dht22_tstΪ�����źŸ����׶α�־λ��0Ϊ�����д����źţ�1Ϊ���俪ʼǰ3s��ʱ��2Ϊ������ʼ�����źţ�3Ϊ�������߲���ʱ�ȴ���
4ΪDHT22��Ӧ���������5ΪDHT������ʱ׼�������6ΪDHT22��������ʱ׼������źš�0������1���͵�ƽ��7ΪDHT22��������źš�0������1���ߵ�ƽ��
�����ʱ�������жϴ��䡰0������1������8Ϊ�ж��źš�0������1�����ٸ��ݴ������ѡ�񷵻�7����ȥ��9,9Ϊ�źŴ�������������ͷ����ߣ�10ΪУ��׶Ρ�
dht22_tst_errΪ�����źŸ����׶δ����־λ������Ϊ��Ӧdht22_tst�ĸ����׶γ�ʱ����
*/
void DHT22_ts(void)
{
	unsigned char temp_ts;
	temp_ts=dht22_tst;
	dcnt=TIM_GetCounter(TIM4);//��ȡ��ʱ������
	TIM_SetCounter(TIM4,0);//���ö�ʱ������Ϊ0
	switch(temp_ts)
	{
		case 4:dht22_tst=5;TIM_SetAutoreload(TIM4,149);break;//DHT22��Ӧ��������׶�,��װ��TIM4��ʱ����ʱ150us
		case 5:dht22_tst=6;break;//DHT������ʱ׼������׶�,ʵ��68
		case 6:dht22_tst=7;break;//DHT22�����ͣ�׼������źš�0������1���͵�ƽ��ʵ��71
		case 7:dht22_tst=8;break;//����źš�0������1���͵�ƽ�������ߣ�ʱ���55us��ʵ��47
		case 8:DHT22_ts_bit();break;//����źš�0������1���ߵ�ƽ���ͣ�����1bit����������0��ʱ���30us����1��ʱ���75us��ʵ��23
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
		case 2://��ʱ3s����������������ʼ�ź�����ʱ��20ms
			TIM4_TInt_Init(19999,71);
			dht22_tst=3;
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		break;
		case 3://2����ʱ��20ms������������ʼ�ź����߳�ʱ��1��20ms
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			dht22_tst=4;
			DHT22_IPU();
			EXTIX_ENABLE(GPIO_PortSourceGPIOA,GPIO_PinSource0,EXTI_Line0);
		break;
//		case 9:break;
		case 10:break;
		case 101://ֹͣ��ʱ������PA0�˿�
			TIM_Cmd(TIM4, DISABLE);  //ֹͣ��ʱ
			TIM_SetCounter(TIM4,0);//���ü���,
			DHT22_PP();
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			if(dht22_tst_err|dhti){dht22_tst=1;}//����д�������ֻ��������һ�����ݣ������´�������
			else{dht22_tst=0;}
		break;
			
//		default:break;
		default:DHT22_tst_err();break;//�����׶���Ӧ��ʱ��������������
		}
}

void DHT22_ts_bit(void)//�����¶ȴ�����bit����,������Ϊ���ϼ������źš�0���ߵ�ƽʱ��22-30us,�źš�1���ߵ�ƽʱ��68-75us
{
	unsigned char dht22_ts_count;
	dht22_ts_count=dht22_ts_i;
	//	if(dcnt>=40&&dcnt<=60){DHT22_tst_err();}//��������ǣ������
/*ʪ�ȸ�8λ+ʪ�ȵ�8λ+�¶ȸ�8λ+�¶ȵ�8λ=У��ͣ�����Ӧ����д����ʱ�������Դ�У��*/
	if(dht22_ts_count==0){d0cnt=dcnt+20;temph_humid[0]=temph_humid[0]<<1;}//ʪ�����λ�ض�Ϊ0��dcn��d0cnt�Ƚ�С����0�������1
	else if(1<=dht22_ts_count&&dht22_ts_count<8)
		{if(dcnt<d0cnt){temph_humid[0]=temph_humid[0]<<1;}else{temph_humid[0]=temph_humid[0]<<1|1;}}
	else if(8<=dht22_ts_count&&dht22_ts_count<16)
		{if(dcnt<d0cnt){templ_humid[0]=templ_humid[0]<<1;}else{templ_humid[0]=templ_humid[0]<<1|1;}}
	else if(16<=dht22_ts_count&&dht22_ts_count<24)
		{if(dcnt<d0cnt){temph_temper[0]=temph_temper[0]<<1;}else{temph_temper[0]=temph_temper[0]<<1|1;}}
	else if(24<=dht22_ts_count&&dht22_ts_count<32)
		{if(dcnt<d0cnt){templ_temper[0]=templ_temper[0]<<1;}else{templ_temper[0]=templ_temper[0]<<1|1;}}
	else {if(dcnt<d0cnt){checksum[0]=checksum[0]<<1;}else{checksum[0]=checksum[0]<<1|1;}}
	
	if(dht22_ts_count<39)//�жϼ��������Ƿ�40�Σ���ת����Ӧ�׶�
	{
		dht22_tst=7;
		dht22_ts_i++;//����bit����
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
	temp=(temph_temper[0]+templ_temper[0]+temph_humid[0]+templ_humid[0])&0XFF;//��ͺ���ܻ����8λ��Ҫ��9-16λ��0
	
//	temper[0]=temph_temper[0];
//	temper[1]=templ_temper[0];
//	humid[0]=temph_humid[0];
//	humid[1]=templ_humid[0];
//	settemper[currtemper0]=checksum[0];
//	set_temper[currtemper0]=temp;
	
	if(temp==0){DHT22_tst_err();}//�������ȫΪ0Ҳ�Ǵ�
	else 
if(temp!=checksum[0]){DHT22_tst_err();}//У���
	else//д����ʪ������
	{
		temph_temper[0]=temph_temper[0]<<8;
		tempa=temph_temper[0]&32768;
		if(tempa)//�ж��¶ȵ�16λ�Ƿ�Ϊ1,32768��������Ƶ�16λΪ1������Ϊ0����ŵ���ʪ������Ϊ�������ݵ�1/10
		{
			temper[0]=(temph_temper[0]&32767)+templ_temper[0];//32767��������Ƶ�16λΪ0������Ϊ1�����ﲻ/����Ϊ������������������
			temper[0]=temper[0]/-10;
		}
		else{temper[0]=temph_temper[0]+templ_temper[0];temper[0]=temper[0]/10;}
		//ʪ��
		humid[0]=(temph_humid[0]<<8)+templ_humid[0];
		humid[0]=humid[0]/10;		
		//��Ҫ�����������β��ܻ�������¶ȣ�dhtiΪ0ʱ���������¶�,dhtiΪ1ʱ�������
		if(dhti)
		{
			dhti=0;
			dhtj=1;
			//�����־λ
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
	temph_temper[0]=templ_temper[0]=temph_humid[0]=templ_humid[0]=checksum[0]=0;//������short���´θ�������ʱ�����������ݣ�������
}

/*�����¶ȴ����������������=���䵽ĳһ�׶Σ������´��䣬���������3�Σ��������θ����¶�*/
void DHT22_tst_err(void)
{
	EXTIX_DISABLE(GPIO_PortSourceGPIOA,GPIO_PinSource0,EXTI_Line0);//�ر�A0�ⲿ�ж���
	TIM_Cmd(TIM4, DISABLE);  //�رն�ʱ
	TIM_SetCounter(TIM4,0);;//���ü��������������ϼ���������Ϊ0
	dht22_tst_err=dht22_tst;
	dht22_tst_errcount++;
	if(dht22_tst_errcount<=3){dht22_tst=10;dht22_time_count=0;}
	else{dht22_tst=dht22_time_count=0;}
}

void DHT22_time(void)//����ÿ��20��ˢ��һ���¶ȣ�����ÿ��1.5����ˢ��һ���¶ȣ�ϵͳʱ��Ƶ��10Hz����˼���=10*��
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
