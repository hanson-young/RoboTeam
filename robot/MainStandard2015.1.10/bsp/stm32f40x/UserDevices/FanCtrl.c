#include "includes.h"
#include "math.h"

#define MAX_HIGHT  50.5

float  Factor = 0.0f;
float start_pos = 0.0f;//��ʼλ�ã��ɿ������
void InitElevator(void)
{
	float MidHight = 30.0f;
	float elevator_speed = 300.0f;
	MOSI_SWITCH[0]=0x01;
	MOSI_SWITCH[1]=0x00;
	Write_Database(S_SWITCH_ID);//����Э������ɳ�ʼ��
	while(MOSI_STATUS[0] != 0x01);//�ȴ���ʼ�����
	start_pos = Motor_Pos[4];
	W_MOTOR_OLD_FUNC(W_MOTOR5_OLD_ID, -(start_pos+MidHight*Factor) , elevator_speed , CMD_SET_PSG);//ʹ����λ��������������
	MOSI_SWITCH[1]=0x00;//��λ��־��Ϊ����ֹͣʹ��
	MOSI_SWITCH[1]=0x00;
	Write_Database(S_SWITCH_ID);
}

void InitStepper(void)
{
//��Ӻ���
	MOSI_SWITCH[2]=0x01;
	Write_Database(S_SWITCH_ID);
	MOSI_SWITCH[2]=0x00;
}

void SetElevator(float elevator_hight,int16_t elevator_speed)
{
	if(elevator_hight>50.5)
		elevator_hight = 48;
	if(elevator_hight<0)
		elevator_hight = 3;
	W_MOTOR_OLD_FUNC(W_MOTOR5_OLD_ID, -(start_pos+elevator_hight*Factor) , elevator_speed , CMD_SET_PSG);//���������߶�
}

void SetStepper(float stepper_angle)
{
//��Ӻ���
	if(stepper_angle > 360)
		stepper_angle = 360;
	if(stepper_angle < -360)
		stepper_angle = -360;
	MOSI_STEPPER.f32_data = stepper_angle;	
	Write_Database(W_STEPPER_ID);

}


void SetFan(float fan_power)
{
	if(fan_power>75)//����ռ�ձ����һ�㲻����7.5%
		fan_power = 75;
	if(fan_power<50)//����ռ�ձ���Сһ�㲻����5%
		fan_power = 50;
	MOSI_FAN.f32_data = fan_power;
	Write_Database(W_FAN_ID);
}

void test_elevator(void)
{
	rt_uint32_t key_value;
	float elevator_hight = 0.0;
	int elevator_speed = 0.0;
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========test_elevator=========");
		LCD_SetXY(0,1);
		LCD_Printf("cur_pos:");
		LCD_WriteDouble((double)(Motor_Pos[4] - start_pos));
		LCD_SetXY(0,2);
		LCD_Printf("cur_hight:");
		LCD_WriteDouble((double)((Motor_Pos[4] - start_pos)/Factor));
		LCD_SetXY(0,3);
		LCD_Printf("Factor:");
		LCD_WriteDouble((double)Factor);
		LCD_SetXY(0,4);
		LCD_Printf("MOSI_SWITCH:");
		LCD_WriteInt(MOSI_SWITCH[0]);
		LCD_Printf("  ");
		LCD_WriteInt(MOSI_SWITCH[1]);
		LCD_Printf("  ");
		LCD_WriteInt(MOSI_SWITCH[2]);
		LCD_SetXY(0,5);
		LCD_Printf("MOSI_STATUS:");
		LCD_WriteInt(MOSI_STATUS[0]);
		LCD_Printf("  ");
		LCD_WriteInt(MOSI_STATUS[1]);
		LCD_Printf("  ");
		LCD_WriteInt(MOSI_STATUS[2]);
		LCD_SetXY(0,6);
		LCD_Printf("1:InitElevator");
		LCD_SetXY(0,7);
		LCD_Printf("2:TextElevator");
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{
			switch(key_value)
			{
				case key1:
				{
					InitElevator();
				}
					break;
				case key2:
				{
					Input_FloatValue(&elevator_hight,"hight");
					Input_IntValue(&elevator_speed,"speed");
					SetElevator(elevator_hight,elevator_speed);
				}
				case keyback:
					return;
					break;
				default:
					break;
			}
		}
		Delay_ms(10);
	}
}

void stepper_fan(void)
{
	rt_uint32_t key_value;
	float fan_power = 0.0f;
	float stepper_angle = 0.0f;
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========stepper_fan=========");
		LCD_SetXY(0,1);
		LCD_Printf("fan_power:");
		LCD_WriteDouble(fan_power);
		LCD_SetXY(0,2);
		LCD_Printf("stepper_angle:");
		LCD_WriteDouble(stepper_angle);
		LCD_SetXY(0,3);
		LCD_Printf("MOSI_SWITCH:");
		LCD_WriteInt(MOSI_SWITCH[0]);
		LCD_Printf("  ");
		LCD_WriteInt(MOSI_SWITCH[1]);
		LCD_Printf("  ");
		LCD_WriteInt(MOSI_SWITCH[2]);
		LCD_SetXY(0,4);
		LCD_Printf("MOSI_STATUS:");
		LCD_WriteInt(MOSI_STATUS[0]);
		LCD_Printf("  ");
		LCD_WriteInt(MOSI_STATUS[1]);
		LCD_Printf("  ");
		LCD_WriteInt(MOSI_STATUS[2]);
		LCD_SetXY(0,5);
		LCD_Printf("1:InitStepper");
		LCD_Printf("2:SetStepper");
		LCD_SetXY(0,6);
		LCD_Printf("3:SetFan");
		
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{
			switch(key_value)
			{
				case key1:
				{
					InitStepper();
					break;
				}
				case key2:
				{
					Input_FloatValue(&stepper_angle,"stepper_angle");
					SetStepper(stepper_angle);
				}
					break;
				case key3:
				{
					Input_FloatValue(&fan_power,"fan_power");
					SetFan(fan_power);
				}
					break;
				case keyback:
					return;
				default:
					break;
			}
		}
		Delay_ms(10);
	}
}