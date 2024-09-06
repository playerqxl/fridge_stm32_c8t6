#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h" 
#include "iwdg.h"
#include "delay.h"
#include "OLED_I2C.h"
#include "DHT22.h"
#include "KEY.h"
#include "display.h"
#include "exti.h"
#include "tim.h"
#include "SCP.h"

#define tempers 2//�¶ȴ���������2
#define currtemper0 0//��ǰ��ʪ��0
#define currtemper1 1//��ǰ��ʪ��1
#define sysinfo 2//ϵͳ��Ϣ

extern volatile unsigned char dp0j;//dp0iΪһ��ҳ���־λ,dp0jΪ����ҳ���־λ
extern volatile signed char dp0i,seti;//dp0iΪһ��ҳ���־λ,������ʾ�趨�¶�2�������������˸��־λ
extern volatile unsigned char setj;//������ʾ�趨�¶�2�����������Ϩ��Ϊ0<=setj<3������Ϊ����3<=setj<6��һ������Ϊ0.5s
extern volatile unsigned char dpi;//�����Ƿ�ˢ����ʾ����ʾ��̬ҳ�治��Ҫһֱˢ��
extern volatile unsigned char dptj;
//extern unsigned char tim3;//��ʱ��tim3��ʱ������־
extern float tim3;
extern volatile unsigned char enter,back,plus,minus,plusi,minusi;//����������־λ��ȷ�������ء��ӡ������Ӻͼ�����λ
extern volatile float temper[tempers],humid[tempers];//����¶����ݺ�ʪ������
extern float settemper[tempers];//��������¶�����
extern float set_temper[tempers];//Ӧ�������¶����ݣ�������ʵ���¶ȱȽ�
/*dht22_tstΪ�����źŸ����׶α�־λ��0Ϊ�����д����źţ�1Ϊ���俪ʼǰ3s��ʱ��2Ϊ������ʼ�����źţ�3Ϊ�������߲���ʱ�ȴ���
4ΪDHT22��Ӧ���������5ΪDHT������ʱ׼�������6ΪDHT22����1bit����ʱ���ͣ�
7ΪDHT22����1bit����ʱ����,8Ϊ�źŴ�������������ͷ����ߣ�9ΪУ��׶Ρ�
dht22_tst_errΪ�����źŸ����׶δ����־λ������Ϊ��Ӧdht22_tst�ĸ����׶γ�ʱ����*/
volatile extern unsigned char dht22_tst,dht22_tst_err;
extern unsigned char dhtj;//���ٸ��³ɹ�һ���¶�
extern unsigned char scp_time_stop;//�뵼������Ƭ��ʱ�乤�������־λ
extern volatile unsigned short temph_temper[tempers],templ_temper[tempers],temph_humid[tempers],templ_humid[tempers],checksum[tempers];
extern volatile int dcnt,d0cnt;

