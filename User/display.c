#include "global.h"

unsigned volatile char dp0j;//dp0j为二级页面标志位
unsigned char spr[1];
float dptemper[tempers],dphumid[tempers];//存放温度数据和湿度数据
float dpdht22_tst_err0,dpdht22_tst_err01,dpdht22_tst;//dht22传输信号错误信息，数字为对应dht22_tst的各个阶段超时错误
volatile signed char dp0i,seti;//dp0i为一级页面标志位,seti为进入显示设定温度2级界面后，文字闪烁标志位
volatile unsigned char setj;//进入显示设定温度2级界面后，文字熄灭为0<=setj<15，亮起为大于15<=setj<30，一个计数为0.1s
volatile unsigned char dpi;//决定是否刷新显示，显示静态页面不需要一直刷新
volatile unsigned short dpti;
unsigned char dpt,dpscp_time_stop;
volatile unsigned char dptj;
//六个显示按键标志位，确定、返回、加、减，加和减计数位
volatile unsigned char dpenter,dpback,dpplus,dpminus,keyenter,keyback,keyplus,keyminus,dpplusi,dpminusi;

/*
显示内容初始化
*/
void DP_Configuration(void)
{
	dp0j=seti=setj=dpenter=dpback=dpplus=dpminus=dpplusi=dpminusi=0;
	dpi=1;
	dpt=dpti=dptj=0;
	dp0i=0;
}

/*
显示程序
*/
void dp0_show(void)
{
	switch(dptj)//屏幕超时执行，0不超时，1清屏并重置显示标志位，2黑屏,3黑屏后有按钮动作并抬起,无论按什么按钮，开屏必定是初始界面
	{
		case 0:
			dp_key();
			if(dp0i>2){dp0i=0;}//页面计数防溢出
			if(dp0i<0){dp0i=2;}//页面计数防溢出
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
		case 2:break;//黑屏
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
//		sprintf((char *)spr,"%05.1f",settemper[currtemper0]);//数字转为字符串，保留一位小数
//		OLED_ShowStr(26,0,spr,2);
//		sprintf((char *)spr,"%05.1f",set_temper[currtemper0]);//数字转为字符串，保留一位小数
//		OLED_ShowStr(26,4,spr,2);
		
		OLED_CLS();
		OLED_ShowStr(16,0,"curr_temper0",2);//显示字符
		sprintf((char *)spr,"%05.1f",dptemper[currtemper0]);//数字转为字符串，保留一位小数
		OLED_ShowStr(26,2,spr,1);
		OLED_ShowStr(67,2,"'C",1);
		sprintf((char *)spr,"%05.1f",dphumid[currtemper0]);//数字转为字符串，保留一位小数
		OLED_ShowStr(26,3,spr,1);
		OLED_ShowStr(66,3,"%RH",1);
		
//由于当前只用了一个传感器，因此没有必要显示传感器1温度
//		OLED_ShowStr(16,4,"curr_temper1",2);//显示字符
//		sprintf((char *)spr,"%05.1f",dptemper[currtemper1]);//数字转为字符串，保留一位小数
//		OLED_ShowStr(26,6,spr,1);
//		OLED_ShowStr(67,6,"'C",1);
//		
//		sprintf((char *)spr,"%05.1f",dphumid[currtemper1]);//数字转为字符串，保留一位小数
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
		settemper0();//显示设定温度0
			//由于当前只用了一个传感器，因此没有必要显示传感器1温度设置
//		settemper1();//显示设定温度1
		dpi=0;
		}
	}
}

void set_temper1(void)//二级页面，文字闪烁频率1s，若选中某一个设置温度传感器，显示该设置温度传感器所在行，清除另一个温度传感器所在行
{
	switch(seti)
	{
		case 0:
			if(dpi)
			{
				if(setj<10)//前10个0.1s清除设置温度传感器0行文字，并显示设置温度传感器1行文字
				{
					ClearOLEDLine(seti);
					settemper1();
				}
				else
				{
					settemper0();//后10个0.1s恢复设置温度传感器0行文字
				}
				dpi=0;
			}
			break;
			
		case 1:
			if(dpi)
			{
				if(setj<10)//前10个0.1s清除设置温度传感器1行文字，并显示设置温度传感器0行文字
				{
					ClearOLEDLine(seti);
					settemper0();
				}
				else
				{
					settemper1();//后10个0.1s恢复设置温度传感器1行文字
				}
				dpi=0;
			}
			break;
			
		case 2://选中设置温度传感器0，清除设置温度传感器0和1行文字
			if(dpi)
			{
				OLED_CLS();
				/*重新显示设置温度传感器0行文字，更新温度传感器0设置数据*/
				settemper0();
				dpi=0;
			}
			break;
		
		case 3://选中设置温度传感器1，清除设置温度传感器0和1行文字
			if(dpi)
			{
				OLED_CLS();
				/*重新显示设置温度传感器1行文字，温度传感器1设置数据*/
				settemper1();
				dpi=0;
			}
			break;
		default:seti=0;break;
	}
};

void dp_setj_counter(void)//温度设置二级页面，文字闪烁计数，每到闪烁计数0和10时启用显示刷新
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

void settemper0(void)//显示设定温度0
{
	OLED_ShowStr(16,0,"set_temper0",2);//显示字符
	sprintf((char *)spr,"%03.0f",settemper[currtemper0]);//数字转为字符串，保留一位小数
	OLED_ShowStr(42,2,spr,2);
	OLED_ShowStr(67,2,"o",1);
	OLED_ShowStr(74,2,"C",2);
}

void settemper1(void)//显示设定温度1
{
	//由于当前只用了一个传感器，因此没有必要显示传感器1设置
//	OLED_ShowStr(16,4,"set_temper1",2);//显示字符
//	sprintf((char *)spr,"%03.0f",settemper[currtemper1]);//数字转为字符串，保留一位小数
//	OLED_ShowStr(42,6,spr,2);
//	OLED_ShowStr(67,6,"o",1);
//	OLED_ShowStr(74,6,"C",2);
}

void sys_info(void)//显示温度传感器通信错误代码
{
	dpdht22_tst_err0=dht22_tst_err;
	if(dpdht22_tst_err01!=dpdht22_tst_err0){dpdht22_tst_err01=dpdht22_tst_err0;dpi=1;}//如果温度传感器错误代码改变，就要刷新一次
	if(dpi)
	{
		OLED_CLS();
		OLED_ShowStr(16,0,"DHT22_ts_err",2);//显示字符
		sprintf((char *)spr,"%03.0f",dpdht22_tst_err0);//数字转为字符串，保留一位小数
		OLED_ShowStr(42,2,spr,2);
		
		dpscp_time_stop=scp_time_stop;//温度传感器工作时间过长停止信息
		if(dpscp_time_stop)
		{
			OLED_ShowStr(0,4,"scp work so long",2);//显示字符
		}
		dpdht22_tst=dht22_tst;
		OLED_ShowStr(16,4,"DHT22_tst",2);//显示字符
		sprintf((char *)spr,"%03.0f",dpdht22_tst);//数字转为字符串，保留一位小数
		OLED_ShowStr(42,6,spr,2);
		
		dpi=0;
	}
}

/*显示器无活动判定超过5分钟，显示器关闭，回到当前温度界面。如果有按钮按下，则重置显示器活动判定*/
void dp_open(void)
{
		dpti=0;
		if(dptj==2){dptj=3;}
}

/*显示器无活动判定超过5分钟，显示器关闭，回到当前温度界面。如果有按钮按下，则重置显示器活动判定*/
void DP_close(void)
{
		if(!dpt)//只有显示器开启才有必要计数
		{
			dpti++;
			if(dpti>2999)//5分钟为5*60*10-1=2999
			{
				dpti=0;
				dpt=dptj=1;
			}
		}
}

/*四个按钮对应的显示逻辑，每个按钮标志位都有对应的显示按钮标志位，在按下按钮时对应的显示按钮标志位置1，再弹起时，对比显示按钮标志位和
按钮标志位不同则进入对应按钮功能。长按加和减，每次显示，长按标志位减去显示长按标志位，所得值乘以4得到此次要加/减的值，然后同步长按标志
位的值为长按标志位。
*/
void dp_key(void)
{
	keyenter=enter;
	keyback=back;
	keyplus=plus;
	keyminus=minus;	
	if(keyenter){dpenter=1;}//按下确认键
	else
	{
		if(dpenter==1)//抬起确认键
		{
			if(dp0i==1)//设置页面确定键才有用
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
	
	if(keyback){dpback=1;}//按下返回键
	else
	{
		if(dpback)//抬起返回键
		{
			if(dp0i==1)
			{
				if(dp0j)//二级设置温度页面开始返回才有用
				{
					switch(seti)
					{
						case 2: reset_tempers();seti=0;break;
						case 3: reset_tempers();seti=1;break;
						default:seti=0;dp0j=0;break;//其它情况返回显示设置温度页面
					}
					dpi=1;
				}
			}
			dpback=0;
		}
	}
	
	if(keyplus)//按下加键
	{
		dpplus=1;
		dpplusi=plusi;
		if(dp0i==1&&dp0j==1)//温度设置二级页面
		{
			if(dpplusi>=5)//长按加键
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
		if(dpplus)//抬起加键
		{
			switch(dp0i)//一级页面向上循环
			{				
				case 1:
					if(!dp0j){++dp0i;}
					else
					{
						switch(seti)//二级页面向上循环，0和1是两个设置温度传感器的文字闪烁界面，2和3是两个设置温度传感器进入页面
						{	
							//由于当前只用了一个传感器，因此没有必要切换传感器
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
	
	if(keyminus)//按下减键
	{
		dpminus=1;
		dpminusi=minusi;
		if(dp0i==1&&dp0j==1)//温度设置二级页面
		{
			if(dpminusi>=5)//长按减键
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
		if(dpminus)//抬起减键
		{
			switch(dp0i)//一级页面向下循环
			{
				case 1:
					if(!dp0j){--dp0i;}
					else
					{
						switch(seti)//二级页面向上循环，0和1是两个设置温度传感器的文字闪烁界面，2和3是两个设置温度传感器进入页面
						{
							//由于当前只用了一个传感器，因此没有必要切换传感器
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
