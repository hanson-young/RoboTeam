#include "all.h"

/*
SS_H进入高级模式时的状态标志，在State_Tran()中置位，在相应状态函数中被复位
	0--空闲，在非空闲状态下所有状态转移的命令都无效，并且LED3_on
	1--陀螺仪校准零漂状态
	2--标定陀螺仪系数状态
	3--标定码盘正反转系数状态
	4--标定码盘旋转半径系数状态

SS_W进入写数据模式标志，在State_Tran()中置位，在相应状态函数中被复位
	0--空闲
	1--忙碌，在此期间所有写数据命令都无效

SS_R进入读数据模式标志，在State_Tran()中置位，在相应状态函数中被复位
	0--空闲
	1--忙碌，在此期间所有读数据命令都无效
*/
uint8_t SS_H = 0,SS_W = 0,SS_R = 0;
uint8_t SS_CF = 0;

uint8_t SEND_CONVER_FLAG=0;

void GPS_TO_MASTER(void)
{
	MISO_GPS_X.f64_data = Gps_List[0].position.x;
	Write_Database(S_GPS_X_ID);
	
	MISO_GPS_Y.f64_data = Gps_List[0].position.y;
	Write_Database(S_GPS_Y_ID);
	
	MISO_GPS_ANG.f64_data = Gps_List[0].radian;
	Write_Database(S_GPS_ANG_ID);
}

void CHECK_FLOAT_STATE(void)
{
	SS_CF = 1;
	SS_H = 0;							//复位状态标志
}

void GYRO_INIT_STATE(void)
{
	/*
	s_gi 陀螺仪初始化状态时的状态标志
		0--初始化还未开始
		1--校准正传系数
		2--校准反正系数
		3--告知主控陀螺仪标定完成
	*/
	static uint8_t s_gi = 0;
	
	switch(s_gi)
	{
		case 0	:		s_gi = 1;
			Gyro_Clear();
			break;
		case 1	:		s_gi = 2;
			Gyro_Init_P();
			Gyro_Clear();
			S_GYRO_C1_FUNC();
			break;
		case 2	:		s_gi = 0;
			Gyro_Init_N();
			S_GYRO_C2_FUNC();
			Data_Save();
			SS_H = 0;//复位状态标志
			break;
		default	:		s_gi = 1;
			Gyro_Clear();
	}
}

void ENC_L_INIT_STATE(void)
{
	/*
	s_el 码盘正反转初始化状态时的状态标志
		0--初始化还未开始
		1--校准正传系数
		2--校准反正系数
		3--告知主控陀螺仪标定完成
	*/
	static uint8_t s_el = 0;
	
	switch(s_el)
	{
		case 0:
			s_el = 1;
			Encoder_Clear(0);
			Encoder_Clear(1);
			break;
		case 1:
			s_el = 2;
			Encoder_InitXY(0);
			break;
		case 2:
			s_el = 3;
		  Encoder_Clear(0);
			Encoder_Clear(1);
			break;
		case 3:
			s_el = 0;
			Encoder_InitXY(1);
			Encoder_Init();
			Data_Save();
			SS_H = 0;	//复位状态标志
			break;
		default	:		s_el = 1;
			Encoder_Clear(0);
			Encoder_Clear(1);
			break;
	}
}

void ENC_R_INIT_STATE(void)
{
	/*
	s_er 码盘旋转半径初始化状态时的状态标志
		0--初始化还未开始
		1--校准正传系数
		2--校准反正系数
		3--告知主控陀螺仪标定完成
	*/
	static uint8_t s_er = 0;
	
	switch(s_er)
	{
		case 0	:		s_er = 1;
			Encoder_Clear(0);
			Encoder_Clear(1);
		  Gyro_Clear();
			break;
		case 1	:		s_er = 0;
			Encoder_InitR();
			Data_Save();
			SS_H = 0;					//复位状态标志
			break;
		default	:		s_er = 1;
			Encoder_Clear(0);
			Encoder_Clear(1);
	}
}


void M_G_CMD_FUNC(void)
{
	uint8_t CMD_NOW;
	
	CMD_NOW = MOSI_CMD;
	
	if(CMD_NOW == GPS_READ)
	{
		GPS_TO_MASTER();
	}else
	{
		/*进入高级模式时的状态标志SS_H：
			0--空闲	
			1--陀螺仪校准零漂状态
			2--标定陀螺仪系数状态
			3--标定码盘正反转系数状态
			4--标定码盘旋转半径系数状态
		*/
		switch(SS_H)
		{
			case 0 :
			{
				switch(CMD_NOW)
				{
					case CHECK_FLOAT	:	SS_H = 1;
						CHECK_FLOAT_STATE();
						break;
					case GYRO_INIT		:	SS_H = 2;
						GYRO_INIT_STATE();
						break;
					case ENC_L_INIT		:	SS_H = 3;
						ENC_L_INIT_STATE();
						break;
					case ENC_R_INIT		:	SS_H = 4;
						ENC_R_INIT_STATE();
						break;
					default :
						LED3_on;
				}
				break;
			}
			case 1 : 
			{
				if(CMD_NOW == CHECK_FLOAT)
				{
					CHECK_FLOAT_STATE();
				}
				else
					LED3_on;
				
				break;
			}
			case 2 :
			{
				if(CMD_NOW == GYRO_INIT)
				{
					GYRO_INIT_STATE();
				}
				else
					LED3_on;
				
				break;
			}
			case 3 :
			{
				if(CMD_NOW == ENC_L_INIT)
				{
					ENC_L_INIT_STATE();
				}
				else
					LED3_on;
				
				break;
			}
			case 4 :
			{
				if(CMD_NOW == ENC_R_INIT)
				{
					ENC_R_INIT_STATE();
				}
				else
					LED3_on;
				
				break;
			}
			default :
				SS_H = 0;
		}
	}
}

void W_GPS_X_FUNC(void)
{
	Pointfp64 position;
	position.x=MOSI_GPS_X.f64_data;
	position.y=Gps_List[0].position.y;
	GPS_Init(position,Gps_List[0].radian);
}

void W_GPS_Y_FUNC(void)
{
	Pointfp64 position;
	position.y=MOSI_GPS_Y.f64_data;
	position.x=Gps_List[0].position.x;
	GPS_Init(position,Gps_List[0].radian);
}

void W_GPS_ANG_FUNC(void)
{
  GPS_Init(Gps_List[0].position,MOSI_GPS_ANG.f64_data);
}

void W_GYRO_C1_FUNC(void)
{
	Gyro_Convert1=MOSI_GYRO_C1.f64_data;
	Data_Save();
}

void W_GYRO_C2_FUNC(void)
{
	Gyro_Convert2=MOSI_GYRO_C2.f64_data;
	Data_Save();
}

void W_ENC_COEFF_FUNC(void)
{
	uint8_t	EncID;
	uint8_t CoeffID;
	
	EncID = MOSI_ENC_COEFF.u8_data[0]/4;
	CoeffID = MOSI_ENC_COEFF.u8_data[0]-4*EncID;
	if(CoeffID == 0)
		Encoders[EncID].Convert1 = MOSI_ENC_COEFF.f64_data;
	if(CoeffID == 1)
		Encoders[EncID].Convert2 = MOSI_ENC_COEFF.f64_data;
	if(CoeffID == 2)
		Encoders[EncID].Radius = MOSI_ENC_COEFF.f64_data;
	if(CoeffID == 3)
		Encoders[EncID].radian = MOSI_ENC_COEFF.f64_data;
	Data_Save();
}

void S_GPS_X_FUNC(void)
{
	MISO_GPS_X.f64_data = Gps_List[0].position.x;
	Write_Database(S_GPS_X_ID);
}

void S_GPS_Y_FUNC(void)
{
	MISO_GPS_Y.f64_data = Gps_List[0].position.y;
	Write_Database(S_GPS_Y_ID);
}

void S_GPS_ANG_FUNC(void)
{
	MISO_GPS_ANG.f64_data = Gps_List[0].radian;
	Write_Database(S_GPS_ANG_ID);
}

void S_GYRO_C1_FUNC(void)
{
	MISO_GYRO_C1.f64_data = Gyro_Convert1;
	Write_Database(S_GYRO_C1_ID);
}

void S_GYRO_C2_FUNC(void)
{
	MISO_GYRO_C2.f64_data = Gyro_Convert2;
	Write_Database(S_GYRO_C2_ID);
}

void S_GYRO_RAD_FUNC(void)
{
	MISO_GYRO_RAD.f64_data = Gyro_Angle_Total;
	Write_Database(S_GYRO_RAD_ID);
}

void S_ENC1_C1_FUNC(void)
{
	MISO_ENC1_C1.f64_data = Encoders[0].Convert1;
	Write_Database(S_ENC1_C1_ID);
}

void S_ENC1_C2_FUNC(void)
{
	MISO_ENC1_C2.f64_data = Encoders[0].Convert2;
	Write_Database(S_ENC1_C2_ID);}

void S_ENC1_RAD_FUNC(void)
{
	MISO_ENC1_RAD.f64_data = Encoders[0].Radius;
	Write_Database(S_ENC1_RAD_ID);
}

void S_ENC1_ANG_FUNC(void)
{
	MISO_ENC1_ANG.f64_data = Encoders[0].radian;
	Write_Database(S_ENC1_ANG_ID);
}

void S_ENC1_DIS_FUNC(void)
{
	MISO_ENC1_DIS.f64_data = Encoders[0].Distance;
	Write_Database(S_ENC1_DIS_ID);
}

void S_ENC2_C1_FUNC(void)
{
 	MISO_ENC2_C1.f64_data = Encoders[1].Convert1;
	Write_Database(S_ENC2_C1_ID);
}

void S_ENC2_C2_FUNC(void)
{
 	MISO_ENC2_C2.f64_data = Encoders[1].Convert2;
	Write_Database(S_ENC2_C2_ID);}

void S_ENC2_RAD_FUNC(void)
{
 	MISO_ENC2_RAD.f64_data = Encoders[1].Radius;
	Write_Database(S_ENC2_RAD_ID);
}

void S_ENC2_ANG_FUNC(void)
{
 	MISO_ENC2_ANG.f64_data = Encoders[1].radian;
	Write_Database(S_ENC2_ANG_ID);
}

void S_ENC2_DIS_FUNC(void)
{
 	MISO_ENC2_DIS.f64_data = Encoders[1].Distance;
	Write_Database(S_ENC2_DIS_ID);
}

void SEND_CONVERS(void)
{
	switch(SEND_CONVER_FLAG)
	{
		case 1 : S_ENC1_C1_FUNC();
			break;
		case 2 : S_ENC1_C2_FUNC();
			break;
		case 3 : S_ENC1_RAD_FUNC();
			break;
		case 4 : S_ENC1_ANG_FUNC();
			break;
		case 5 : S_ENC2_C1_FUNC();
			break;
		case 6 : S_ENC2_C2_FUNC();
			break;
		case 7 : S_ENC2_RAD_FUNC();
			break;
		case 8 : S_ENC2_ANG_FUNC();
			break;
		case 9 : S_GYRO_C1_FUNC();
			break;
		case 10 : S_GYRO_C2_FUNC();
			SEND_CONVER_FLAG = 0;
			break;
		default : S_ENC1_C1_FUNC();
			SEND_CONVER_FLAG = 1;
			break;
	}
	SEND_CONVER_FLAG++;
	
}
void W_PWM_FUNC(void)
{
	PWM1_SetDuty(MOSI_PWM.u16_data[0]);
	PWM2_SetDuty(MOSI_PWM.u16_data[1]);

}
