#include "global.h"

unsigned volatile char dp0j;//dp0jΪ����ҳ���־λ
unsigned char spr[1];
float dptemper[tempers],dphumid[tempers];//����¶����ݺ�ʪ������
float dpdht22_tst_err0,dpdht22_tst_err01,dpdht22_tst;//dht22�����źŴ�����Ϣ������Ϊ��Ӧdht22_tst�ĸ����׶γ�ʱ����
volatile signed char dp0i,seti;//dp0iΪһ��ҳ���־λ,setiΪ������ʾ�趨�¶�2�������������˸��־λ
volatile unsigned char setj;//������ʾ�趨�¶�2�����������Ϩ��Ϊ0<=setj<15������Ϊ����15<=setj<30��һ������Ϊ0.1s
volatile unsigned char dpi;//�����Ƿ�ˢ����ʾ����ʾ��̬ҳ�治��Ҫһֱˢ��
volatile unsigned short dpti;
unsigned char dpt,dpscp_time_stop;
volatile unsigned char dptj;
//������ʾ������־λ��ȷ�������ء��ӡ������Ӻͼ�����λ
volatile unsigned char dpenter,dpback,dpplus,dpminus,keyenter,keyback,keyplus,keyminus,dpplusi,dpminusi;

/*
��ʾ���ݳ�ʼ��
*/
void DP_Configuration(void)
{
	dp0j=seti=setj=dpenter=dpback=dpplus=dpminus=dpplusi=dpminusi=0;
	dpi=1;
	dpt=dpti=dptj=0;
	dp0i=0;
}

/*
��ʾ����
*/
void dp0_show(void)
{
	switch(dptj)//��Ļ��ʱִ�У�0����ʱ��1������������ʾ��־λ��2����,3�������а�ť������̧��,���۰�ʲô��ť�������ض��ǳ�ʼ����
	{
		case 0:
			dp_key();
			if(dp0i>2){dp0i=0;}//ҳ����������
			if(dp0i<0){dp0i=2;}//ҳ����������
			switch(dp0i)
			{
				case 0:curr_temper();break;
				case 1:set_temper0();break;
				case 2:sys_info();break;
				default:dp0i=0;curr_temper();break;
			}
			break;
		case 1:
			OLED_CLS();
			dp0i=dp0j=dpi=0;
			dptj=2;
			break;
		case 2:break;//����
		case 3:if(enter==0&&back==0&&plus==0&&minus==0){dpt=0;dptj=4;dpi=1;}break;
		case 4:dpenter=dpback=dpplus=dpminus=keyenter=keyback=keyplus=keyminus=dp0i=dp0j=dptj=0;break;
		default:dptj=0;break;
	}
}

void curr_temper(void)
{
	if(dpi)
	{
		dptemper[currtemper0]=temper[0];
		dptemper[currtemper1]=temper[1];
		dphumid[currtemper0]=humid[0];
		dphumid[currtemper1]=humid[1];

		
//		dptemper[currtemper1]=templ_humid[0];
////		dphumid[currtemper0]=d0cnt;
//		dphumid[currtemper1]=temph_temper[0];
//		dphumid[currtemper1]=templ_temper[0];
//		sprintf((char *)spr,"%05.1f",settemper[currtemper0]);//����תΪ�ַ���������һλС��
//		OLED_ShowStr(26,0,spr,2);
//		sprintf((char *)spr,"%05.1f",set_temper[currtemper0]);//����תΪ�ַ���������һλС��
//		OLED_ShowStr(26,4,spr,2);
		
		OLED_CLS();
		OLED_ShowStr(16,0,"curr_temper0",2);//��ʾ�ַ�
		sprintf((char *)spr,"%05.1f",dptemper[currtemper0]);//����תΪ�ַ���������һλС��
		OLED_ShowStr(26,2,spr,1);
		OLED_ShowStr(67,2,"'C",1);
		sprintf((char *)spr,"%05.1f",dphumid[currtemper0]);//����תΪ�ַ���������һλС��
		OLED_ShowStr(26,3,spr,1);
		OLED_ShowStr(66,3,"%RH",1);
		
//���ڵ�ǰֻ����һ�������������û�б�Ҫ��ʾ������1�¶�
//		OLED_ShowStr(16,4,"curr_temper1",2);//��ʾ�ַ�
//		sprintf((char *)spr,"%05.1f",dptemper[currtemper1]);//����תΪ�ַ���������һλС��
//		OLED_ShowStr(26,6,spr,1);
//		OLED_ShowStr(67,6,"'C",1);
//		
//		sprintf((char *)spr,"%05.1f",dphumid[currtemper1]);//����תΪ�ַ���������һλС��
//		OLED_ShowStr(26,7,spr,1);
//		OLED_ShowStr(66,7,"%RH",1);
		
		dpi=0;
	}
}

void set_temper0(void)
{
	if(dp0j)
	{
	set_temper1();
	}
	else
	{
		if(dpi)
		{
		OLED_CLS();
		settemper0();//��ʾ�趨�¶�0
			//���ڵ�ǰֻ����һ�������������û�б�Ҫ��ʾ������1�¶�����
//		settemper1();//��ʾ�趨�¶�1
		dpi=0;
		}
	}
}

void set_temper1(void)//����ҳ�棬������˸Ƶ��1s����ѡ��ĳһ�������¶ȴ���������ʾ�������¶ȴ����������У������һ���¶ȴ�����������
{
	switch(seti)
	{
		case 0:
			if(dpi)
			{
				if(setj<10)//ǰ10��0.1s��������¶ȴ�����0�����֣�����ʾ�����¶ȴ�����1������
				{
					ClearOLEDLine(seti);
					settemper1();
				}
				else
				{
					settemper0();//��10��0.1s�ָ������¶ȴ�����0������
				}
				dpi=0;
			}
			break;
			
		case 1:
			if(dpi)
			{
				if(setj<10)//ǰ10��0.1s��������¶ȴ�����1�����֣�����ʾ�����¶ȴ�����0������
				{
					ClearOLEDLine(seti);
					settemper0();
				}
				else
				{
					settemper1();//��10��0.1s�ָ������¶ȴ�����1������
				}
				dpi=0;
			}
			break;
			
		case 2://ѡ�������¶ȴ�����0����������¶ȴ�����0��1������
			if(dpi)
			{
				OLED_CLS();
				/*������ʾ�����¶ȴ�����0�����֣������¶ȴ�����0��������*/
				settemper0();
				dpi=0;
			}
			break;
		
		case 3://ѡ�������¶ȴ�����1����������¶ȴ�����0��1������
			if(dpi)
			{
				OLED_CLS();
				/*������ʾ�����¶ȴ�����1�����֣��¶ȴ�����1��������*/
				settemper1();
				dpi=0;
			}
			break;
		default:seti=0;break;
	}
};

void dp_setj_counter(void)//�¶����ö���ҳ�棬������˸������ÿ����˸����0��10ʱ������ʾˢ��
{
	if(dp0i==1&&dp0j==1)
	{ 
		if(seti==0||seti==1)
		{
		setj++;
		if(setj>=20){setj=0;}
		if(setj==0||setj==10){dpi=1;}
		}else {setj=0;}
	}else {setj=0;}
}

void settemper0(void)//��ʾ�趨�¶�0
{
	OLED_ShowStr(16,0,"set_temper0",2);//��ʾ�ַ�
	sprintf((char *)spr,"%03.0f",settemper[currtemper0]);//����תΪ�ַ���������һλС��
	OLED_ShowStr(42,2,spr,2);
	OLED_ShowStr(67,2,"o",1);
	OLED_ShowStr(74,2,"C",2);
}

void settemper1(void)//��ʾ�趨�¶�1
{
	//���ڵ�ǰֻ����һ�������������û�б�Ҫ��ʾ������1����
//	OLED_ShowStr(16,4,"set_temper1",2);//��ʾ�ַ�
//	sprintf((char *)spr,"%03.0f",settemper[currtemper1]);//����תΪ�ַ���������һλС��
//	OLED_ShowStr(42,6,spr,2);
//	OLED_ShowStr(67,6,"o",1);
//	OLED_ShowStr(74,6,"C",2);
}

void sys_info(void)//��ʾ�¶ȴ�����ͨ�Ŵ������
{
	dpdht22_tst_err0=dht22_tst_err;
	if(dpdht22_tst_err01!=dpdht22_tst_err0){dpdht22_tst_err01=dpdht22_tst_err0;dpi=1;}//����¶ȴ������������ı䣬��Ҫˢ��һ��
	if(dpi)
	{
		OLED_CLS();
		OLED_ShowStr(16,0,"DHT22_ts_err",2);//��ʾ�ַ�
		sprintf((char *)spr,"%03.0f",dpdht22_tst_err0);//����תΪ�ַ���������һλС��
		OLED_ShowStr(42,2,spr,2);
		
		dpscp_time_stop=scp_time_stop;//�¶ȴ���������ʱ�����ֹͣ��Ϣ
		if(dpscp_time_stop)
		{
			OLED_ShowStr(0,4,"scp work so long",2);//��ʾ�ַ�
		}
		dpdht22_tst=dht22_tst;
		OLED_ShowStr(16,4,"DHT22_tst",2);//��ʾ�ַ�
		sprintf((char *)spr,"%03.0f",dpdht22_tst);//����תΪ�ַ���������һλС��
		OLED_ShowStr(42,6,spr,2);
		
		dpi=0;
	}
}

/*��ʾ���޻�ж�����5���ӣ���ʾ���رգ��ص���ǰ�¶Ƚ��档����а�ť���£���������ʾ����ж�*/
void dp_open(void)
{
		dpti=0;
		if(dptj==2){dptj=3;}
}

/*��ʾ���޻�ж�����5���ӣ���ʾ���رգ��ص���ǰ�¶Ƚ��档����а�ť���£���������ʾ����ж�*/
void DP_close(void)
{
		if(!dpt)//ֻ����ʾ���������б�Ҫ����
		{
			dpti++;
			if(dpti>2999)//5����Ϊ5*60*10-1=2999
			{
				dpti=0;
				dpt=dptj=1;
			}
		}
}

/*�ĸ���ť��Ӧ����ʾ�߼���ÿ����ť��־λ���ж�Ӧ����ʾ��ť��־λ���ڰ��°�ťʱ��Ӧ����ʾ��ť��־λ��1���ٵ���ʱ���Ա���ʾ��ť��־λ��
��ť��־λ��ͬ������Ӧ��ť���ܡ������Ӻͼ���ÿ����ʾ��������־λ��ȥ��ʾ������־λ������ֵ����4�õ��˴�Ҫ��/����ֵ��Ȼ��ͬ��������־
λ��ֵΪ������־λ��
*/
void dp_key(void)
{
	keyenter=enter;
	keyback=back;
	keyplus=plus;
	keyminus=minus;	
	if(keyenter){dpenter=1;}//����ȷ�ϼ�
	else
	{
		if(dpenter==1)//̧��ȷ�ϼ�
		{
			if(dp0i==1)//����ҳ��ȷ����������
			{
				if(dp0j)
				{
					switch(seti)
					{
						case 0: seti=2;break;
						case 2: set_tempers();seti=0;break;
						case 1: seti=3;break;
						case 3: set_tempers();seti=1;break;
						default:seti=0;break;
					}
				}
				else{dp0j=1;}				
				dpi=1;
			}
			dpenter=0;
		}
	}
	
	if(keyback){dpback=1;}//���·��ؼ�
	else
	{
		if(dpback)//̧�𷵻ؼ�
		{
			if(dp0i==1)
			{
				if(dp0j)//���������¶�ҳ�濪ʼ���ز�����
				{
					switch(seti)
					{
						case 2: reset_tempers();seti=0;break;
						case 3: reset_tempers();seti=1;break;
						default:seti=0;dp0j=0;break;//�������������ʾ�����¶�ҳ��
					}
					dpi=1;
				}
			}
			dpback=0;
		}
	}
	
	if(keyplus)//���¼Ӽ�
	{
		dpplus=1;
		dpplusi=plusi;
		if(dp0i==1&&dp0j==1)//�¶����ö���ҳ��
		{
			if(dpplusi>=5)//�����Ӽ�
			{
				switch(seti)
				{
					case 2:plus_count_tempers();break;
					case 3:plus_count_tempers();break;
					default:break;
				}
				dpi=1;
			}
		}
	}
	else
	{
		if(dpplus)//̧��Ӽ�
		{
			switch(dp0i)//һ��ҳ������ѭ��
			{				
				case 1:
					if(!dp0j){++dp0i;}
					else
					{
						switch(seti)//����ҳ������ѭ����0��1�����������¶ȴ�������������˸���棬2��3�����������¶ȴ���������ҳ��
						{	
							//���ڵ�ǰֻ����һ�������������û�б�Ҫ�л�������
//							case 0:seti=1;break;
//							case 1:seti=0;break;
							case 2:plus_tempers();break;
							case 3:plus_tempers();break;
							default:seti=0;break;
						}
					}
					break;
					default:++dp0i;break;
			}
			dpplus=0;
			dpi=1;
		}
	}
	
	if(keyminus)//���¼���
	{
		dpminus=1;
		dpminusi=minusi;
		if(dp0i==1&&dp0j==1)//�¶����ö���ҳ��
		{
			if(dpminusi>=5)//��������
			{
				switch(seti)
				{
					case 2:minus_count_tempers();break;
					case 3:minus_count_tempers();break;
					default:break;
				}
				dpi=1;
			}
		}
	}
	else
	{
		if(dpminus)//̧�����
		{
			switch(dp0i)//һ��ҳ������ѭ��
			{
				case 1:
					if(!dp0j){--dp0i;}
					else
					{
						switch(seti)//����ҳ������ѭ����0��1�����������¶ȴ�������������˸���棬2��3�����������¶ȴ���������ҳ��
						{
							//���ڵ�ǰֻ����һ�������������û�б�Ҫ�л�������
//							case 0:seti=1;break;
//							case 1:seti=0;break;
							case 2:minus_tempers();break;
							case 3:minus_tempers();break;
							default:seti=0;break;
						}
					}
					break;
				default:--dp0i;break;
			}
			dpminus=0;
			dpi=1;
		}
	}
}
