#include "includes.h"

extern u8 Stop_Flag[3];

void CheckFloat(void)
{
	MISO_CMD = NO_COMMAND;
	MOSI_CMD = CHECK_FLOAT;
	Write_Database(M_G_CMD_ID);
}


void G_M_CMD_FUNC(void)
{
	if(*(Can_Database[G_M_CMD_ID].Data_ptr) == CF_NORMAL)
	{
		
	}
	else if(*(Can_Database[G_M_CMD_ID].Data_ptr) == CF_ERROR)
	{
		
	}
}

void S_GPS_X_FUNC(void)
{
	GPS.position.x = MISO_GPS_X.f64_data;
}

void S_GPS_Y_FUNC(void)
{
	GPS.position.y = MISO_GPS_Y.f64_data;
}

void S_GPS_ANG_FUNC(void)
{
	GPS.radian = MISO_GPS_ANG.f64_data;
}

void S_GYRO_C1_FUNC(void)
{
	Gyro.convert1 = MISO_GYRO_C1.f64_data;
}

void S_GYRO_C2_FUNC(void)
{
	Gyro.convert2 = MISO_GYRO_C2.f64_data;
}

void S_GYRO_RAD_FUNC(void)
{
	Gyro.angle = MISO_GYRO_RAD.f64_data;
}

void S_ENC1_C1_FUNC(void)
{
	Encoder[0].convert1 = MISO_ENC1_C1.f64_data;
}

void S_ENC1_C2_FUNC(void)
{
	Encoder[0].convert2 = MISO_ENC1_C2.f64_data;
}

void S_ENC1_RAD_FUNC(void)
{
	Encoder[0].r = MISO_ENC1_RAD.f64_data;
}

void S_ENC1_ANG_FUNC(void)
{
	Encoder[0].radian = MISO_ENC1_ANG.f64_data;
}

void S_ENC1_DIS_FUNC(void)
{
	Encoder[0].dis = MISO_ENC1_DIS.f64_data;
}

void S_ENC2_C1_FUNC(void)
{
	Encoder[1].convert1 = MISO_ENC2_C1.f64_data;
}

void S_ENC2_C2_FUNC(void)
{
	Encoder[1].convert2 = MISO_ENC2_C2.f64_data;
}

void S_ENC2_RAD_FUNC(void)
{
	Encoder[1].r = MISO_ENC2_RAD.f64_data;
}

void S_ENC2_ANG_FUNC(void)
{
	Encoder[1].radian = MISO_ENC2_ANG.f64_data;
}

void S_ENC2_DIS_FUNC(void)
{
	Encoder[1].dis = MISO_ENC2_DIS.f64_data;
}

void S_FACTOR_FUNC(void)
{                                                                                                                                                    
	Factor = MOSI_FACTOR.f32_data;
}
void S_STATUS_FUNC(void)
{
	
}

void W_MOTOR_SPEED_ANGLE_FUNC(ID_NUMDEF ID_NUM , union fi32_to_u8 SPEED_ANGLE)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		*(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+i) = SPEED_ANGLE.u8_data[i];
	}
	Write_Database(ID_NUM);
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
        
//     if(ID_NUM>W_MOTOR8_OLD_ID || ID_NUM<W_MOTOR1_OLD_ID)
//     {
//         /* add code here to report error */
//         return;
//     }
	
	uRot.v   = rot ; 
	uSpeed.v = speed ; 
    /* send goal position data by can bus */
	
    *(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+0) = uRot.d[0];
    *(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+1) = uRot.d[1];
    *(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+2) = uRot.d[2];
    *(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+3) = uRot.d[3];
    
	*(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+4) = uSpeed.d[0];
	*(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+5) = uSpeed.d[1];
    
	*(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+6) = CURRENT_ROBOT;
	*(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+7) = cmd;
	
	
	Write_Database(ID_NUM); 
}

void W_MOTOR_ALL_FUNC(int16_t *speed_for_all)
{
	int counter = 0;
	
    union uSpeedType
    {
        uint8_t d[2];    /* data */
        int16_t v;      /* value */
    }uSpeed;          /* union speed */
	
	for(; counter <= 3; speed_for_all++, counter++)
	{
		uSpeed.v = *speed_for_all; 
		
		*(Can_Database[HASH_TABLE[W_MOTOR_ALL_SPEED_ID]].Data_ptr+counter*2) = uSpeed.d[0];
		*(Can_Database[HASH_TABLE[W_MOTOR_ALL_SPEED_ID]].Data_ptr+counter*2+1) = uSpeed.d[1];
	}
	
	Write_Database(W_MOTOR_ALL_SPEED_ID); 
}

void S_MOTOR1_ENC_FUNC(void)
{
	union  fi32_to_u8 temp;
	
	IO_State[0] = MISO_M1_ENC[0];
	
	temp.u8_data[0] = MISO_M1_ENC[1];
	temp.u8_data[1] = MISO_M1_ENC[2];
	temp.u8_data[2] = MISO_M1_ENC[3];
	temp.u8_data[3] = MISO_M1_ENC[4];

	
	Motor_RealAim[0] = MISO_M1_ENC[6]*256+MISO_M1_ENC[5];
	Motor_Pos[0] = temp.f32_data;
}

void S_MOTOR2_ENC_FUNC(void)
{
	extern u8 Motor2_Time_Flag;
	union fi32_to_u8 temp;
	
	IO_State[1] = MISO_M2_ENC[0];
	
	temp.u8_data[0] = MISO_M2_ENC[1];
	temp.u8_data[1] = MISO_M2_ENC[2];
	temp.u8_data[2] = MISO_M2_ENC[3];
	temp.u8_data[3] = MISO_M2_ENC[4];		
	
	Motor_RealAim[1] = MISO_M2_ENC[6]*256+MISO_M2_ENC[5];
	Motor_Pos[1] = temp.f32_data;
	
	Motor2_Time_Flag = 0;
}

void S_MOTOR3_ENC_FUNC(void)
{
	union fi32_to_u8 temp;
	
	IO_State[2] = MISO_M3_ENC[0];
	
	temp.u8_data[0] = MISO_M3_ENC[1];
	temp.u8_data[1] = MISO_M3_ENC[2];
	temp.u8_data[2] = MISO_M3_ENC[3];
	temp.u8_data[3] = MISO_M3_ENC[4];		
	
	Motor_RealAim[2] = MISO_M3_ENC[6]*256+MISO_M3_ENC[5];
	Motor_Pos[2] = temp.f32_data;
	
}

void S_MOTOR4_ENC_FUNC(void)
{
	union fi32_to_u8 temp;
	
	IO_State[3] = MISO_M4_ENC[0];
	
	temp.u8_data[0] = MISO_M4_ENC[1];
	temp.u8_data[1] = MISO_M4_ENC[2];
	temp.u8_data[2] = MISO_M4_ENC[3];
	temp.u8_data[3] = MISO_M4_ENC[4];
	
	Motor_RealAim[3] = MISO_M4_ENC[6]*256+MISO_M4_ENC[5];
	Motor_Pos[3] = temp.f32_data;
	
}

void S_MOTOR5_ENC_FUNC(void)
{
	union fi32_to_u8 temp;
	
	IO_State[4] = MISO_M5_ENC[0];
	
	temp.u8_data[0] = MISO_M5_ENC[1];
	temp.u8_data[1] = MISO_M5_ENC[2];
	temp.u8_data[2] = MISO_M5_ENC[3];
	temp.u8_data[3] = MISO_M5_ENC[4];		
	
	Motor_RealAim[4] = MISO_M5_ENC[6]*256+MISO_M5_ENC[5];
	Motor_Pos[4] = temp.f32_data;
	
}

void S_MOTOR6_ENC_FUNC(void)
{
	union fi32_to_u8 temp;
	
	IO_State[5] = MISO_M6_ENC[0];
	
	temp.u8_data[0] = MISO_M6_ENC[1];
	temp.u8_data[1] = MISO_M6_ENC[2];
	temp.u8_data[2] = MISO_M6_ENC[3];
	temp.u8_data[3] = MISO_M6_ENC[4];		
	
	Motor_RealAim[5] = MISO_M6_ENC[6]*256+MISO_M6_ENC[5];
	Motor_Pos[5] = temp.f32_data;
	
}

void S_MOTOR7_ENC_FUNC(void)
{
	union fi32_to_u8 temp;
	
	IO_State[6] = MISO_M7_ENC[0];
	
	temp.u8_data[0] = MISO_M7_ENC[1];
	temp.u8_data[1] = MISO_M7_ENC[2];
	temp.u8_data[2] = MISO_M7_ENC[3];
	temp.u8_data[3] = MISO_M7_ENC[4];		
	
	Motor_RealAim[6] = MISO_M7_ENC[6]*256+MISO_M7_ENC[5];
	Motor_Pos[6] = temp.f32_data;
	
}

void S_MOTOR8_ENC_FUNC(void)
{
	union fi32_to_u8 temp;
	
	IO_State[7] = MISO_M8_ENC[0];
	
	temp.u8_data[0] = MISO_M8_ENC[1];
	temp.u8_data[1] = MISO_M8_ENC[2];
	temp.u8_data[2] = MISO_M8_ENC[3];
	temp.u8_data[3] = MISO_M8_ENC[4];		
	
	Motor_RealAim[7] = MISO_M8_ENC[6]*256+MISO_M8_ENC[5];
	Motor_Pos[7] = temp.f32_data;
	
}


void W_PWM_FUNC(double pwm0, double pwm1, double pwm2, double pwm3)
{
	union fi64_to_u8 temp;
	temp.u16_data[0] = (uint16_t)(pwm0*10);
	temp.u16_data[1] = (uint16_t)(pwm1*10);
	temp.u16_data[2] = (uint16_t)(pwm2*10);
	temp.u16_data[3] = (uint16_t)(pwm3*10);
	
	MOSI_PWM.f64_data = temp.f64_data;
	Write_Database(W_PWM_ID);
}
	


extern u8 FilterFlag[6];
void S_CAMERA1_FUNC(void)
{

}

void S_CAMERA2_FUNC(void)
{

}

void S_CAMERA3_FUNC(void)
{

}

void S_CAMERA4_FUNC(void)
{

}

void S_CAMERA5_FUNC(void)
{

}

/**-------------------------------------------*
  | @attention:for transmiting image          |
  *-------------------------------------------*/
void img_save_isr(u8 camx);
void S_CAM1_TRANS_FUNC(void)
{
    img_save_isr(1);
}
void S_CAM2_TRANS_FUNC(void)
{
    img_save_isr(2);
}
void S_CAM3_TRANS_FUNC(void)
{
    img_save_isr(3);
}
void S_CAM4_TRANS_FUNC(void)
{
    img_save_isr(4);
}
void S_CAM5_TRANS_FUNC(void)
{
    img_save_isr(5);
}



void W_RELAY64_FUNC(void)
{
	
}

void W_RELAY8_FUNC(void)
{
	
}

void W_SONIC1_FUNC(void)
{
	
}

void S_SONIC1_FUNC(void)
{

}

void W_SONIC2_FUNC(void)
{
	
}

void S_SONIC2_FUNC(void)
{

}
