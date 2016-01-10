#ifndef _CAN_DATABASE_H
#define _CAN_DATABASE_H
	#include "stm32f4xx.h"
	
	#ifndef NULL
	#define NULL ((void *)0)
	#endif
	
	#define fp64 double
	#define fp32  float
	
;	union fi64_to_u8     //fp64�����ݣ���int64_t������ת��u8�����ݽ���can���߷���
	{
		fp64	f64_data;
		int64_t i64_data;
		uint64_t u64_data;
		uint16_t u16_data[4];
		int16_t i16_data[4];
		uint8_t u8_data[8];
	};
	
	union fi32_to_u8     //fp32�����ݣ���int32_t������ת��u8�����ݽ���can���߷���
	{
		fp32	f32_data;
		int32_t i32_data;
		uint32_t u32_data;
		int16_t i16_data[2];
		uint8_t u8_data[4];
	};
	
	union u16_to_u8
	{
		uint16_t u16_data;
		uint8_t u8_data[2];
	};

	/*Data_Type*/
	#define READ_ONLY  0	//���ض�������д
	#define WRITE_ONLY 1	//����д�������
	
	
/*���������Ӧ��ID���б�*/
/*
IDλ��	[	(GS:GPS Slave  MS:Motor Slave)							]
NO.7	[			0--GS			|		1--Others				]
NO.6	[	0--MOSI		|	1--MISO	|	0--MS MOSI	|	1--Others	]
NO.5~0	[	ID numbers				|	ID numbers  &  Others		]
	
	Others��
		B11-000-xxx ����&&ISOЭ����ͨ��ID
		B11-001-xxx ����&&cameraͨ��ID--MISO
		B11-011-xxx ����&&cameraͨ��ID--MOSI
		B11-010-xxx ������relayͨ��ID
		B11-100-xxx �����벿��Э����ͨ��ID   ��������������
*/
	typedef enum
	{
//B0x-xxx-xxx ����&&GPSЭ����ͨ��ID*/
	//MOSI   B00-xxx-xxx
		M_G_CMD_ID		= 0x00,		//������GPS�巢�����ID
		//д��������
		W_STEPPER_ID  = 0X01,
			//B00-001-xxx  дGPS����
		W_GPS_X_ID		= 0x08,
		W_GPS_Y_ID		= 0x09,
		W_GPS_ANG_ID	= 0x0A,
			//B00-010-xxx  дGYRO����
		W_GYRO_C1_ID	= 0x10,
		W_GYRO_C2_ID	= 0x11,
// 			//B00-011-xxx  дENC1ϵ��
// 		W_ENC1_C1_ID	= 0x18,
// 		W_ENC1_C2_ID	= 0x19,
// 		W_ENC1_RAD_ID	= 0x1A,
// 		W_ENC1_ANG_ID	= 0x1B,
// 			//B00-100-xxx  дENC2ϵ��
// 		W_ENC2_C1_ID	= 0x20,
// 		W_ENC2_C2_ID	= 0x21,
// 		W_ENC2_RAD_ID	= 0x22,
// 		W_ENC2_ANG_ID	= 0x23,
		W_ENC_COEFF_ID= 0x30,
	//MISO   B01-xxx-xxx
		G_M_CMD_ID		= 0x40,		//GPS�������ط������ID
		//����������
			//B01-001-xxx  ��GPS����
		S_GPS_X_ID		= 0x48,
		S_GPS_Y_ID		= 0x49,
		S_GPS_ANG_ID	= 0x4A,
			//B01-010-xxx  ��GYRO����
		S_GYRO_C1_ID	= 0x50,
		S_GYRO_C2_ID	= 0x51,
		S_GYRO_RAD_ID	= 0x52,
			//B01-011-xxx  ��ENC1����
		S_ENC1_C1_ID	= 0x58,
		S_ENC1_C2_ID	= 0x59,
		S_ENC1_RAD_ID	= 0x5A,
		S_ENC1_ANG_ID	= 0x5B,
		S_ENC1_DIS_ID	= 0x5C,
			//B01-100-xxx  ��ENC2����
		S_ENC2_C1_ID	= 0x60,
		S_ENC2_C2_ID	= 0x61,
		S_ENC2_RAD_ID	= 0x62,
		S_ENC2_ANG_ID	= 0x63,
		S_ENC2_DIS_ID	= 0x64,
		
		W_MOTOR_ALL_SPEED_ID = 0x70,
		
//B10-xxxxx-x ����&&MOTORͨ��ID*/
	//MOSI   B10-0-0000-x     ��1�ŵ�����ƺ�
		W_MOTOR1_SPEED_ID	= 0x80,
		W_MOTOR1_ANGLE_ID	= 0x81,
	//MOSI   B10-0-0001-x     ��2�ŵ�����ƺ�
		W_MOTOR2_SPEED_ID	= 0x82,
		W_MOTOR2_ANGLE_ID	= 0x83,
	//MOSI   B10-0-0010-x     ��3�ŵ�����ƺ�
		W_MOTOR3_SPEED_ID	= 0x84,
		W_MOTOR3_ANGLE_ID	= 0x85,
	//MOSI   B10-0-0011-x     ��4�ŵ�����ƺ�
		W_MOTOR4_SPEED_ID	= 0x86,
		W_MOTOR4_ANGLE_ID	= 0x87,
	//MOSI   B10-0-0100-x     ��5�ŵ�����ƺ�
		W_MOTOR5_SPEED_ID	= 0x88,
		W_MOTOR5_ANGLE_ID	= 0x89,
	//MOSI   B10-0-0101-x     ��6�ŵ�����ƺ�
		W_MOTOR6_SPEED_ID	= 0x8a,
		W_MOTOR6_ANGLE_ID	= 0x8b,
	//MOSI   B10-0-0110-x     ��7�ŵ�����ƺ�
		W_MOTOR7_SPEED_ID	= 0x8c,
		W_MOTOR7_ANGLE_ID	= 0x8d,
	//MOSI   B10-0-0111-x     ��8�ŵ�����ƺ�
		W_MOTOR8_SPEED_ID	= 0x8e,
		W_MOTOR8_ANGLE_ID	= 0x8f,
	//MOSI   B10-0-1000-x     ��9�ŵ�����ƺ�
		W_MOTOR9_SPEED_ID	= 0x90,
		W_MOTOR9_ANGLE_ID	= 0x91,
		
	//MOSI   B10-1-0d-xxx     ��xxx�žɵ�����ƺУ�d	1:�������̵���Ϣ    0�����Ƶ��
		//��1�žɿ��ƺ�
		W_MOTOR1_OLD_ID		= 0xa0,
		S_MOTOR1_ENC_ID		= 0xa8,
		//��1�žɿ��ƺ�
		W_MOTOR2_OLD_ID		= 0xa1,
		S_MOTOR2_ENC_ID		= 0xa9,
		//��1�žɿ��ƺ�
		W_MOTOR3_OLD_ID		= 0xa2,
		S_MOTOR3_ENC_ID		= 0xaa,
		//��1�žɿ��ƺ�
		W_MOTOR4_OLD_ID		= 0xa3,
		S_MOTOR4_ENC_ID		= 0xab,
		//��1�žɿ��ƺ�
		W_MOTOR5_OLD_ID		= 0xa4,
		S_MOTOR5_ENC_ID		= 0xac,
		//��1�žɿ��ƺ�
		W_MOTOR6_OLD_ID		= 0xa5,
		S_MOTOR6_ENC_ID		= 0xad,
		//��1�žɿ��ƺ�
		W_MOTOR7_OLD_ID		= 0xa6,
		S_MOTOR7_ENC_ID		= 0xae,
		//��1�žɿ��ƺ�
		W_MOTOR8_OLD_ID		= 0xa7,
		S_MOTOR8_ENC_ID		= 0xaf,
		
		W_MOTOR1_NEW_ID  =0xb0,
		W_MOTOR2_NEW_ID  =0xb1,
		
//B11-000-xxx ����&&ISOЭ����ͨ��ID*/
	//MOSI   B11-0001-xx
		W_PWM_ID		= 0xc4,
	//MISO   B11-0000-xx
		S_SWITCH_ID		= 0xc5,
		W_STEER_ID		= 0xc6,
		W_BAT_ID		= 0xc7,
		
//B11-0x1-xxx ����&&cameraͨ��ID*/
	//MISO   B11-001-xxx     ��xxx��camera
		S_CAMERA1_ID	= 0xc8,
		S_CAMERA2_ID	= 0xc9,
		S_CAMERA3_ID	= 0xca,
		S_CAMERA4_ID	= 0xcb,
		S_CAMERA5_ID	= 0xcc,
		
	//MOSI   B11-011-xxx     ��xxx��camera
		W_CAMERA1_ID	= 0xd8,
		W_CAMERA2_ID	= 0xd9,
		W_CAMERA3_ID	= 0xda,
		W_CAMERA4_ID	= 0xdb,
		W_CAMERA5_ID	= 0xdc,
//B11-010-xxx ������relayͨ��ID
		W_RELAY64_ID	= 0xD0,
		W_RELAY1_ID		= 0xD1,
		W_RELAY2_ID		= 0xD2,
//B11-100-xxx �����벿��Э����ͨ��ID
	//B11-100-00x ������1�ų�����Э����ͨ��ID
		//MOSI B11-100-000  ����ʹ�ܺ�ʧ��ĳ����������
		W_SONIC1_ID		= 0xE0,
		//MISO B11-100-001   �����ط��س���������
		S_SONIC1_ID		= 0xE1,
    //B11-100-01x ������2�ų�����Э����ͨ��ID
		//MOSI B11-100-010  ����ʹ�ܺ�ʧ��ĳ����������
		W_SONIC2_ID		= 0xE2,
		//MISO B11-100-011   �����ط��س���������
		S_SONIC2_ID		= 0xE3,
		
		//added by fxh_bool 4-10 for image save
		S_CAM1_TRANS     = 0xe8,
		S_CAM2_TRANS     = 0xe9,
		S_CAM3_TRANS     = 0xea,
		S_CAM4_TRANS     = 0xeb,
		S_CAM5_TRANS     = 0xec,
		BACK_FACTOR_ID   = 0xed,
		W_STATUS_ID      = 0xef,
	}ID_NUMDEF;	
	
	/*���غ�GPSЭ����֮��������ʽ*/
	typedef enum
	{
		NO_COMMAND			= 0x00,
		//M_G_CMD_ID�µ�����
		GPS_READ			= 0x01,		//��ȡGPS��Ϣ����
		CHECK_FLOAT			= 0x02,		//֪ͨGPSЭ����У׼��������Ư����
		GYRO_INIT			= 0x03,		//֪ͨGPSЭ���ر궨����������
		ENC_L_INIT			= 0x04,		//֪ͨGPSЭ���ر궨��������תϵ������
		ENC_R_INIT			= 0x05,		//֪ͨGPSЭ���ر궨������ת�뾶����
		//G_M_CMD_ID�µ�����
		CF_NORMAL			= 0x06,
		CF_CHANGED			= 0x07,
		CF_ERROR			= 0x08
	}COM_TYPE;
	
	typedef struct
	{
		uint8_t  Data_type;
		ID_NUMDEF  Data_ID;
		uint8_t* Data_ptr;
		uint8_t  Data_length;
		
		//��can���߽ӵ����ݺ󣬵��õĺ���
		void (*MenuFunc)(void);			//��ں���		
		uint8_t  Channel;
		uint8_t  Fifo_num;			//�ڽ��շ�����ID���õ�fifo��
	}Can_Data;

	
	
	extern uint8_t Can_Data_Num;
	extern Can_Data Can_Database[];
	extern uint8_t HASH_TABLE[256];
	
	extern uint8_t MOSI_CMD;
	extern uint8_t MISO_CMD;

	extern union fi64_to_u8 MOSI_GPS_X;
	extern union fi64_to_u8 MOSI_GPS_Y;
	extern union fi64_to_u8 MOSI_GPS_ANG;
	extern union fi64_to_u8 MOSI_GYRO_C1;
	extern union fi64_to_u8 MOSI_GYRO_C2;
	extern union fi64_to_u8 MOSI_ENC_COEFF;

	extern union fi64_to_u8 MISO_GPS_X;
	extern union fi64_to_u8 MISO_GPS_Y;
	extern union fi64_to_u8 MISO_GPS_ANG;
	extern union fi64_to_u8 MISO_GYRO_C1;
	extern union fi64_to_u8 MISO_GYRO_C2;
	extern union fi64_to_u8 MISO_GYRO_RAD;
	extern union fi64_to_u8 MISO_ENC1_C1;
	extern union fi64_to_u8 MISO_ENC1_C2;
	extern union fi64_to_u8 MISO_ENC1_RAD;
	extern union fi64_to_u8 MISO_ENC1_ANG;
	extern union fi64_to_u8 MISO_ENC1_DIS;
	extern union fi64_to_u8 MISO_ENC2_C1;
	extern union fi64_to_u8 MISO_ENC2_C2;
	extern union fi64_to_u8 MISO_ENC2_RAD;
	extern union fi64_to_u8 MISO_ENC2_ANG;
	extern union fi64_to_u8 MISO_ENC2_DIS;
		
// 	extern union fi32_to_u8 MOSI_MOTOR1_SPEED;
// 	extern union fi32_to_u8 MOSI_MOTOR1_ANGLE;

// 	extern union fi32_to_u8 MOSI_MOTOR2_SPEED;
// 	extern union fi32_to_u8 MOSI_MOTOR2_ANGLE;

// 	extern union fi32_to_u8 MOSI_MOTOR3_SPEED;
// 	extern union fi32_to_u8 MOSI_MOTOR3_ANGLE;

// 	extern union fi32_to_u8 MOSI_MOTOR4_SPEED;
// 	extern union fi32_to_u8 MOSI_MOTOR4_ANGLE;


	extern uint8_t MOSI_MOTOR1_SPEED[8];
	extern uint8_t MOSI_MOTOR2_SPEED[8];
	extern uint8_t MOSI_MOTOR3_SPEED[8];
	extern uint8_t MOSI_MOTOR4_SPEED[8];


	extern union fi32_to_u8 MOSI_MOTOR5_SPEED;
	extern union fi32_to_u8 MOSI_MOTOR5_ANGLE;

	extern union fi32_to_u8 MOSI_MOTOR6_SPEED;
	extern union fi32_to_u8 MOSI_MOTOR6_ANGLE;

	extern union fi32_to_u8 MOSI_MOTOR7_SPEED;
	extern union fi32_to_u8 MOSI_MOTOR7_ANGLE;

	extern union fi32_to_u8 MOSI_MOTOR8_SPEED;
	extern union fi32_to_u8 MOSI_MOTOR8_ANGLE;

	extern union fi32_to_u8 MOSI_MOTOR9_SPEED;
	extern union fi32_to_u8 MOSI_MOTOR9_ANGLE;

  extern union fi64_to_u8 MOSI_stepper;
  
 extern union u16_to_u8 MOSI_STEER;
 
	extern union u16_to_u8 MOSI_BAT;
	
	extern uint8_t MOSI_MOTOR1_OLD[8];
	extern uint8_t MOSI_MOTOR2_OLD[8];
	extern uint8_t MOSI_MOTOR3_OLD[8];
	extern uint8_t MOSI_MOTOR4_OLD[8];
	extern uint8_t MOSI_MOTOR5_OLD[8];
	extern uint8_t MOSI_MOTOR6_OLD[8];
	extern uint8_t MOSI_MOTOR7_OLD[8];
	extern uint8_t MOSI_MOTOR8_OLD[8];
	extern uint8_t MOSI_MOTOR9_OLD[8];
	
	extern uint8_t MISO_M1_ENC[7];
	extern uint8_t MISO_M2_ENC[7];
	extern uint8_t MISO_M3_ENC[7];
	extern uint8_t MISO_M4_ENC[7];
	extern uint8_t MISO_M5_ENC[7];
	extern uint8_t MISO_M6_ENC[7];
	extern uint8_t MISO_M7_ENC[7];
	extern uint8_t MISO_M8_ENC[7];

	
	extern uint8_t MOSI_SWITCH[4];
	extern uint8_t MOSI_STATUS[4];
	extern union fi64_to_u8 MOSI_PWM;
	extern union fi32_to_u8 MOSI_FACTOR;
	
	//SLAVER--CAMERA
	extern uint8_t MOSI_CAMERA1[8];
	extern uint8_t MOSI_CAMERA2[8];
	extern uint8_t MOSI_CAMERA3[8];
	extern uint8_t MOSI_CAMERA4[8];
	extern uint8_t MOSI_CAMERA5[8];
	
	//added by fxh_bool 4-10 for image save
    extern uint8_t MISO_CAM1_TRANS[8];
    extern uint8_t MISO_CAM2_TRANS[8];
    extern uint8_t MISO_CAM3_TRANS[8];
    extern uint8_t MISO_CAM4_TRANS[8];
    extern uint8_t MISO_CAM5_TRANS[8];
	
	extern uint8_t MOSI_RELAY1_DATA;
	extern uint8_t MOSI_RELAY2_DATA;
	
	//SLAVER--SONIC1
	extern uint8_t MOSI_SONIC1_CMD;
	extern uint8_t MISO_SONIC1_DAT[3];
    //SLAVER--SONIC2
	extern uint8_t MOSI_SONIC2_CMD;
	extern uint8_t MISO_SONIC2_DAT[3];
	
	void Hash_Table_init(void);
	void Write_Database(ID_NUMDEF ID_NUM);
typedef union 
{
    u8 d[2];
    s16 v;
}u8tos16_t;

typedef struct
{
		u8tos16_t	x_or_angle;
		u8tos16_t	y_or_dis;
		u8tos16_t	z;
	u8tos16_t	turn;  
		int time_flag;
}camera_t;

extern camera_t camera_data1;
extern camera_t camera_data2;
extern camera_t camera_data3;
extern camera_t camera_data4;
extern camera_t camera_data5;
extern camera_t camera_data6;
#endif
