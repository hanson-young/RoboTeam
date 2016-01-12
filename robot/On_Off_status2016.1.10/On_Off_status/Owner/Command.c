#include "all.h"
u16 H_HIT=0;
u16 L_HIT=0;

//void W_PWM_FUNC(void)
//{
//	PWM1_SetDuty(MOSI_PWM.u16_data[0]);
//	PWM2_SetDuty(MOSI_PWM.u16_data[1]);
//	PWM3_SetDuty(MOSI_PWM.u16_data[2]);
//	PWM4_SetDuty(MOSI_PWM.u16_data[3]);
//	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
//}

void Moto_Set_GSpeed(ID_NUMDEF ID_NUM, int16_t speed )   //goal speed
{
 	W_MOTOR_OLD_FUNC(ID_NUM, 0 , speed , CMD_SET_SG);
}

void W_UP_PWM_FUNC(void)
{
	PWM2_SetDuty(MOSI_UP_PWM.u16_data[1]);
}

void S_SWITCH_FUNC(void)
{
	Write_Database(S_SWITCH_ID);
}

void S_MOTOR_ANGLE_FUNC(void)
{
	MOSI_MOTOR_ANGLE.i32_data=1;
	Write_Database(S_MOTOR5_ANGLE_ID);
}


u8		IO_State[8];//电机控制盒上的ST的状态，比如:电机1对应 IO_State[0]
float	Motor_Pos[8];//电机的绝对位置，比如:电机1对应 Motor_Pos[0]
int16_t Motor_RealAim[8];//电机控制盒认为的目标位置*1000后得到的整数

void S_MOTOR5_ENC_FUNC(void)
{
	union  fi32_to_u8 temp;
	IO_State[0] = MISO_M5_ENC[0];	
	temp.u8_data[0] = MISO_M5_ENC[1];
	temp.u8_data[1] = MISO_M5_ENC[2];
	temp.u8_data[2] = MISO_M5_ENC[3];
	temp.u8_data[3] = MISO_M5_ENC[4];
	Motor_RealAim[4] = MISO_M5_ENC[6]*256+MISO_M5_ENC[5];
	Motor_Pos[4] = temp.f32_data;
}
void S_MOTOR6_ENC_FUNC(void)
{
	union  fi32_to_u8 temp;
	IO_State[0] = MISO_M6_ENC[0];	
	temp.u8_data[0] = MISO_M6_ENC[1];
	temp.u8_data[1] = MISO_M6_ENC[2];
	temp.u8_data[2] = MISO_M6_ENC[3];
	temp.u8_data[3] = MISO_M6_ENC[4];
	Motor_RealAim[5] = MISO_M6_ENC[6]*256+MISO_M6_ENC[5];
	Motor_Pos[5] = temp.f32_data;
}

void S_Prepare_FUNC(void)//接受主控发来让上层电机开始旋转
{
	//让电机先沿一个方向转动	，打到一侧的接近开关，去中断里读取此时电机的码盘值，并且让电机反方向转动，
	//打到另一侧的接近开关时，记下此时电机的另一码盘值，与第一次的做差后与真实上下距离求比值得出比例因素factor
	if(MOSI_SWITCH[0])
	{
			Moto_Set_GSpeed(W_MOTOR5_OLD_ID,(int16_t)-200);
	}		
}

void Moto_Stop(ID_NUMDEF ID_NUM)
{
 	W_MOTOR_OLD_FUNC(ID_NUM, 0 , 0 , CMD_STOP);
}

float fan_power = 0.0f;

void SET_FAN_FUNC(void)
{
	fan_power = MOSI_FAN.f32_data;
}

void Moto_Clear_NPos(ID_NUMDEF ID_NUM)	
{
	W_MOTOR_OLD_FUNC(ID_NUM, 0 , 0 , 1);
}
	

void W_MOTOR_OLD_FUNC(ID_NUMDEF ID_NUM, float rot, int16_t speed,int8_t cmd)
{
    
    union uPosType
    {
        uint8_t d[4];    /* data */
        float v;      /* value */
    }uRot;          /* union position */
    
    union uSpeedType
    {
        uint8_t d[2];    /* data */
        int16_t v;      /* value */
    }uSpeed;          /* union speed */
        
		uRot.v   = rot ; 
		uSpeed.v = speed ; 
    /* send goal position data by can bus */
	
    *(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+0) = uRot.d[0];
    *(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+1) = uRot.d[1];
    *(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+2) = uRot.d[2];
    *(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+3) = uRot.d[3];
    
		*(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+4) = uSpeed.d[0];
		*(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+5) = uSpeed.d[1];
    
		*(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+6) = 0;
		*(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+7) = cmd;
		
		Write_Database(ID_NUM); 
}
