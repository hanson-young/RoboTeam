#ifndef _CAN_DATABASE_H
#define _CAN_DATABASE_H
	#include "stm32f10x.h"
	
	#ifndef NULL
	#define NULL ((void *)0)
	#endif
	
	#define fp64 double
	#define fp32  float

	/*Data_Type*/
	#define READ_ONLY  0	//���ض�������д
	#define WRITE_ONLY 1	//����д�������
	
	union fi64_to_u8     //fp64�����ݣ���int64_t������ת��u8�����ݽ���can���߷���
	{
		fp64	 f64_data;
		int64_t  i64_data;
		uint8_t  u8_data[8];
		uint16_t u16_data[4];
	};
	union fi32_to_u8     //fp32�����ݣ���int32_t������ת��u8�����ݽ���can���߷���
	{
		fp32	f32_data;
		int32_t i32_data;
		uint8_t u8_data[4];
	};
	
		union u16_to_u8
	{
		uint16_t u16_data;
		uint8_t u8_data[2];
	};
/*���������Ӧ��ID���б�*/
/*
IDλ��	[	(GS:GPS Slave  MS:Motor Slave)							]
NO.7	[			0--GS			|		1--Others				]
NO.6	[	0--MOSI		|	1--MISO	|	0--MS MOSI	|	1--Others	]
NO.5~0	[	ID numbers				|	ID numbers  &  Others		]
*/
	typedef enum
	{
//B11-000-xxx ����&&ISOЭ����ͨ��ID*/
	//MOSI   B11-0001-xx
		W_PWM_ID			= 0xc4,
	//MISO   B11-0000-xx
		S_SWITCH_ID			= 0xc5,
		BACK_FACTOR_ID	= 0xed,
		W_STATUS_ID     = 0xef,
		
		S_MOTOR5_ANGLE_ID	= 0x81,
		S_MOTOR6_ANGLE_ID	= 0x83,
				//��1�žɿ��ƺ�
		W_MOTOR5_OLD_ID		= 0xa4,
		W_MOTOR6_OLD_ID		= 0xa5,
		W_MOTOR3_OLD_ID		= 0xa2,
		S_MOTOR3_ENC_ID		= 0xaa,
		S_MOTOR5_ENC_ID		= 0xac,
		S_MOTOR6_ENC_ID		= 0xad,
		W_MOTOR1_OLD_ID		= 0xa0,
		S_MOTOR1_ENC_ID		= 0xa8,
		
		W_MOTOR2_OLD_ID		= 0xa1,
		S_MOTOR2_ENC_ID		= 0xa9,
		//W_UP_PWM_ID=0xc7,
			W_FAN_ID		= 0xc7,
		S_Play_ID=0xc8
	
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
	
	extern uint8_t MOSI_SWITCH[4];
	extern uint8_t MOSI_STATUS[4];
	extern union fi64_to_u8 MOSI_PWM;
	extern union fi64_to_u8 MOSI_UP_PWM;
	extern union fi32_to_u8 MOSI_MOTOR_ANGLE;
	extern union fi32_to_u8 MOSI_FACTOR;
	
	extern union fi32_to_u8 MOSI_FAN;
	extern uint8_t MISO_M5_ENC[7];
	extern uint8_t MOSI_MOTOR5_OLD[8];
	extern uint8_t MOSI_MOTOR3_OLD[8];
	extern uint8_t MISO_M6_ENC[7];
	extern uint8_t MISO_M3_ENC[7];
	extern uint8_t MOSI_MOTOR6_OLD[8];
	
	void Hash_Table_init(void);
	void Write_Database(ID_NUMDEF ID_NUM);
	void S_Prepare_FUNC(void);

#endif
