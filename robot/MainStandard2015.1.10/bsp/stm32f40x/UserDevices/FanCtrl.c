#include "includes.h"
#include "math.h"

#define MAX_HIGHT  50.5

float  Factor = 0.0f;
float start_pos = 0.0f;//起始位置，可看作零点
void InitElevator(void)
{
	float MidHight = 30.0f;
	float elevator_speed = 300.0f;
	MOSI_SWITCH[0]=0x01;
	MOSI_SWITCH[1]=0x00;
	Write_Database(S_SWITCH_ID);//控制协主控完成初始化
	while(MOSI_STATUS[0] != 0x01);//等待初始化完成
	start_pos = Motor_Pos[4];
	W_MOTOR_OLD_FUNC(W_MOTOR5_OLD_ID, -(start_pos+MidHight*Factor) , elevator_speed , CMD_SET_PSG);//使涵道位于升降机构中心
	MOSI_SWITCH[1]=0x00;//限位标志作为紧急停止使用
	MOSI_SWITCH[1]=0x00;
	Write_Database(S_SWITCH_ID);
}

void TextElevator(float elevator_hight,int16_t elevator_speed)
{
	if(elevator_hight>50.5)
		elevator_hight = 48;
	if(elevator_hight<0)
		elevator_hight = 3;
	W_MOTOR_OLD_FUNC(W_MOTOR5_OLD_ID, -(start_pos+elevator_hight*Factor) , elevator_speed , CMD_SET_PSG);//调节升降高度
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
					TextElevator(elevator_hight,elevator_speed);
				}
					break;
				default:
					break;
			}
		}
		Delay_ms(10);
	}
}