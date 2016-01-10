
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "math.h"

#define Acc_Limit_Enable 0
#define Max_Acc 50
#define RATE 0.56
#define PI 3.1415926

//���������ϵ�ǻ����˵ľֲ�����ϵ
double Speed_X;				// mm/s
double Speed_Y;				// mm/s
double Speed_Rotation;		// (rad/s)/100

double coeff_x,coeff_y,coeff_r; //�ſ˱Ⱦ��󻯼���ϵ��

//struct Mecanum_State Mec_FL={W_MOTOR1_SPEED_ID, 1};		//���ֵ�״̬����
//struct Mecanum_State Mec_FR={W_MOTOR2_SPEED_ID, -1};
//struct Mecanum_State Mec_BL={W_MOTOR3_SPEED_ID, 1};
//struct Mecanum_State Mec_BR={W_MOTOR4_SPEED_ID, -1};


struct Mecanum_State Mec_FL={4, -1};		//���ֵ�״̬����
struct Mecanum_State Mec_FR={3,  1};
struct Mecanum_State Mec_BL={2, -1};
struct Mecanum_State Mec_BR={1,  1};

static double robo_L = 636.6;//�����־�
static double robo_W = 636.2;//ǰ���־�
static double wheel_r = 76.0;

double motor_speed=0;

double Speed_Limit = 1000;

/*
 * ������: Mecanum_test
 * ��  ��: ���ӵ���
 * ��  ��: ����״̬��Ϣ
 * ��  ��: ��
 * ��  ��: �ⲿ����
 */
void Mecanum_test(struct Mecanum_State *mec)
{
	rt_uint32_t key_value;
	double speed_pwm=50;
	
/*->*/mec_test:
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======Mecanum=======");
		LCD_SetXY(0,1);
		LCD_WriteString("1.Speed");
		LCD_SetXY(0,2);
		LCD_WriteString("2.ChangeArg");
		LCD_SetXY(0,3);
		LCD_WriteString("3.ChangePort now:");
		LCD_WriteInt((*mec).port);
//		
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{
			switch(key_value)
			{
				case key1:
					goto mec_speed;
				case key2:
					Input_IntValue(&(*mec).arg,"Argument");
					break;
				case key3:
// 					Input_IntValue(int(&(*mec).ID_NUM),"Can");
					break;
				case keyback:
					return;
			}
		}
		Delay_ms(10);
	}
	
/*->*/mec_speed:
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======Mecanum=======");
		LCD_SetXY(0,1);
		LCD_WriteString("1.SpeedUp");
		LCD_SetXY(0,2);
		LCD_WriteString("2.SpeedDown");
		LCD_SetXY(0,3);
		LCD_WriteString("3.Stop");
		LCD_SetXY(0,4);
		LCD_WriteString("4.Set Speed");
		
		
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{
			switch(key_value)
			{
				case key1:
					speed_pwm+=1;
					break;
				case key2:
					speed_pwm-=1;
					break;
				case key3:
					speed_pwm=50;
					break;
				case key4:
					Input_DoubleValue(&speed_pwm,"Goal Speed");
					break;
				case keyback:	
// 					motor_speed=0;
					goto mec_test;
			}
			PWM_SetDuty((*mec).port,speed_pwm);
		}
		Delay_ms(10);
	}
}


/*
 * ������: SetSpeed
 * ��  ��: Ϊ�����˸��ٶ�
 * ��  ��: ����������ϵX�����ٶ�(mm/s)��Y�����ٶ�(mm/s)����ת�ٶ�(rad/s)/100
 * ��  ��: ��
 * ��  ��: �ⲿ����
 */
//void SetSpeed(double fl_speed,double fr_speed,double bl_speed,double br_speed)
void SetSpeed(double speed_x,double speed_y,double speed_rotation)
{
	
	double fl_speed;
	double fr_speed;
	double bl_speed;
	double br_speed;
	
	double fl_delta;
	double fr_delta;
	double bl_delta;
	double br_delta;
	
	double real_fl_speed;
	double real_fr_speed;
	double real_bl_speed;
	double real_br_speed;
	
	static double fl_last=0;	//�������ֵ��ٶ�
	static double fr_last=0;
	static double bl_last=0;
	static double br_last=0;
	

	double delta_max;			//�ٶȱ仯�����ֵ
	double PWM_Max;				//���ɵ�PWM���Ӧ���ٶ����ֵ
	
	double PWM_FL,PWM_FR,PWM_BL,PWM_BR;
/*******************anti-slip************************/	

// 	coeff_x = coeff_y = 1/wheel_r/(2*pi)*60;
// 	coeff_r = -(robo_L+robo_W)/2/wheel_r/(2*pi)*60;
//	������������������ ���������ٶȹ��켴�� ��Ҫ�������ſ˱Ⱦ���ĸ���������
	coeff_x = coeff_y = 0.7071*0.05;
	coeff_r = 0.9806;
	
/****************************ͨ���ſ˱Ⱦ�����ٶȷֽ⵽��������******************************/
	fr_speed = + coeff_r*speed_rotation + coeff_x*speed_x - coeff_y*speed_y;
	fl_speed = + coeff_r*speed_rotation + coeff_x*speed_x + coeff_y*speed_y;
	bl_speed = + coeff_r*speed_rotation - coeff_x*speed_x + coeff_y*speed_y;
	br_speed = + coeff_r*speed_rotation - coeff_x*speed_x - coeff_y*speed_y;
	/****************************�Լ��ٶȽ�������(�������ٶ����Ƹ��ٶ�)******************************/
	
	fl_delta = fl_speed - fl_last;
	fr_delta = fr_speed - fr_last;
	bl_delta = bl_speed - bl_last;
	br_delta = br_speed - br_last;
	
	delta_max=MaxFour(Abs(fl_delta),Abs(fr_delta),Abs(bl_delta),Abs(br_delta));
	
	if((delta_max >= Max_Acc) && (Acc_Limit_Enable==1))
	{
		fl_last += Max_Acc*(fl_delta/delta_max);
		fr_last += Max_Acc*(fr_delta/delta_max);
		bl_last += Max_Acc*(bl_delta/delta_max);
		br_last += Max_Acc*(br_delta/delta_max);
	} 
	else if((delta_max <= -Max_Acc) && (Acc_Limit_Enable==1))
	{
		fl_last -= Max_Acc*(fl_delta/delta_max);
		fr_last -= Max_Acc*(fr_delta/delta_max);
		bl_last -= Max_Acc*(bl_delta/delta_max);
		br_last -= Max_Acc*(br_delta/delta_max);
	}
	else
	{
		fl_last = fl_speed;
		fr_last = fr_speed;
		bl_last = bl_speed;
		br_last = br_speed;
	}
	
	fl_speed = fl_last * Mec_FL.arg;
	fr_speed = fr_last * Mec_FR.arg;
	bl_speed = bl_last * Mec_BL.arg;
	br_speed = br_last * Mec_BR.arg;
	
	/**************************�ٶ�ת��Ϊת��*****************************

	             r = v / c 
	
	 *************************************************************************/	
	
	real_fl_speed = fl_speed * RATE *14;
	real_fr_speed = fr_speed * RATE *14;
	real_bl_speed = bl_speed * RATE *14;
	real_br_speed = br_speed * RATE *14;
	
	
//	W_MOTOR_OLD_FUNC(W_MOTOR2_SPEED_ID, 0, real_fr_speed, CMD_SET_SG);
//	W_MOTOR_OLD_FUNC(W_MOTOR4_SPEED_ID, 0, real_br_speed, CMD_SET_SG);
//	W_MOTOR_OLD_FUNC(W_MOTOR1_SPEED_ID, 0, real_fl_speed, CMD_SET_SG);
//	W_MOTOR_OLD_FUNC(W_MOTOR3_SPEED_ID, 0, real_bl_speed, CMD_SET_SG);
	
	
		/*******************************************************
	
	 *************************************************************************/	
 	PWM_FL = (50.0/6000)*14*fl_speed+50;
 	PWM_FR = (50.0/6000)*14*fr_speed+50;
 	PWM_BL = (50.0/6000)*14*bl_speed+50;
 	PWM_BR = (50.0/6000)*14*br_speed+50;
// 	
// 	/************************�������ٶ��ٴν�������****************************/
// 	
 	PWM_Max=MaxFour(Abs(PWM_FL-50),Abs(PWM_FR-50),Abs(PWM_BL-50),Abs(PWM_BR-50));
 	if(PWM_Max >= 48)
 	{
 		PWM_FL = 48*(PWM_FL-50)/PWM_Max+50;
 		PWM_FR = 48*(PWM_FR-50)/PWM_Max+50;
 		PWM_BL = 48*(PWM_BL-50)/PWM_Max+50;
 		PWM_BR = 48*(PWM_BR-50)/PWM_Max+50;
 	}
	/************************����Ӧ�Ķ˿ڽ��и�ֵ******************************/

	PWM_SetDuty(Mec_FL.port,PWM_FL);
 	PWM_SetDuty(Mec_FR.port,PWM_FR);
 	PWM_SetDuty(Mec_BL.port,PWM_BL);
 	PWM_SetDuty(Mec_BR.port,PWM_BR);
}
/**********************************END OF FILE*********************************/
