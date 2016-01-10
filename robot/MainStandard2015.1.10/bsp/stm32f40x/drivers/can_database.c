/*
ÿ��һ��CAN�����豸
	1������Can_Data_Num����ֵ
	2��Ҫ��Can_Database�б������������Ϣ���ر�ע�ⲻҪ���
	3��void Hash_Table_init(void)�У�����¼��豸��Can_Database�б��е����
	4�����ID_NUMDEFö�ٶ����м�������ID�������������
*/

#include "includes.h"

uint8_t Can_Data_Num=0;
//SLAVER--GPS
uint8_t MOSI_CMD;
uint8_t MISO_CMD;

#define CAMERA_LENGTH 8

camera_t camera_data1={0};
camera_t camera_data2={0};
camera_t camera_data3={0};
camera_t camera_data4={0};
camera_t camera_data5={0};
camera_t camera_data6={0};

union fi64_to_u8 MOSI_GPS_X;
union fi64_to_u8 MOSI_GPS_Y;
union fi64_to_u8 MOSI_GPS_ANG;
union fi64_to_u8 MOSI_GYRO_C1;
union fi64_to_u8 MOSI_GYRO_C2;
union fi64_to_u8 MOSI_ENC_COEFF;

union fi64_to_u8 MISO_GPS_X;
union fi64_to_u8 MISO_GPS_Y;
union fi64_to_u8 MISO_GPS_ANG;
union fi64_to_u8 MISO_GYRO_C1;
union fi64_to_u8 MISO_GYRO_C2;
union fi64_to_u8 MISO_GYRO_RAD;
union fi64_to_u8 MISO_ENC1_C1;
union fi64_to_u8 MISO_ENC1_C2;
union fi64_to_u8 MISO_ENC1_RAD;
union fi64_to_u8 MISO_ENC1_ANG;
union fi64_to_u8 MISO_ENC1_DIS;
union fi64_to_u8 MISO_ENC2_C1;
union fi64_to_u8 MISO_ENC2_C2;
union fi64_to_u8 MISO_ENC2_RAD;
union fi64_to_u8 MISO_ENC2_ANG;
union fi64_to_u8 MISO_ENC2_DIS;

//SLAVER--MOTOR
// union fi32_to_u8 MOSI_MOTOR1_SPEED;
// union fi32_to_u8 MOSI_MOTOR1_ANGLE;

// union fi32_to_u8 MOSI_MOTOR2_SPEED;
// union fi32_to_u8 MOSI_MOTOR2_ANGLE;

// union fi32_to_u8 MOSI_MOTOR3_SPEED;
// union fi32_to_u8 MOSI_MOTOR3_ANGLE;

// union fi32_to_u8 MOSI_MOTOR4_SPEED;
// union fi32_to_u8 MOSI_MOTOR4_ANGLE;

uint8_t W_MOTOR_ALL_SPEED[8];

uint8_t MOSI_MOTOR1_SPEED[8];
uint8_t MOSI_MOTOR2_SPEED[8];
uint8_t MOSI_MOTOR3_SPEED[8];
uint8_t MOSI_MOTOR4_SPEED[8];

union fi32_to_u8 MOSI_MOTOR5_SPEED;
union fi32_to_u8 MOSI_MOTOR5_ANGLE;

union fi32_to_u8 MOSI_MOTOR6_SPEED;
union fi32_to_u8 MOSI_MOTOR6_ANGLE;

union fi32_to_u8 MOSI_MOTOR7_SPEED;
union fi32_to_u8 MOSI_MOTOR7_ANGLE;

union fi32_to_u8 MOSI_MOTOR8_SPEED;
union fi32_to_u8 MOSI_MOTOR8_ANGLE;

union fi32_to_u8 MOSI_MOTOR9_SPEED;
union fi32_to_u8 MOSI_MOTOR9_ANGLE;

union fi64_to_u8 MOSI_stepper;

union u16_to_u8 MOSI_STEER;
union fi32_to_u8 MOSI_FACTOR;

//SLAVER--OLD MOTOR
uint8_t MOSI_MOTOR1_OLD[8];
uint8_t MOSI_MOTOR2_OLD[8];
uint8_t MOSI_MOTOR3_OLD[8];
uint8_t MOSI_MOTOR4_OLD[8];
uint8_t MOSI_MOTOR5_OLD[8];
uint8_t MOSI_MOTOR6_OLD[8];
uint8_t MOSI_MOTOR7_OLD[8];
uint8_t MOSI_MOTOR8_OLD[8];

uint8_t MISO_M1_ENC[7];
uint8_t MISO_M2_ENC[7];
uint8_t MISO_M3_ENC[7];
uint8_t MISO_M4_ENC[7];
uint8_t MISO_M5_ENC[7];
uint8_t MISO_M6_ENC[7];
uint8_t MISO_M7_ENC[7];
uint8_t MISO_M8_ENC[7];

//SLAVER--ISO
uint8_t MOSI_SWITCH[4]={0x0,0x0,0x0,0x0};
uint8_t MOSI_STATUS[4]={0x0,0x0,0x0,0x0};
union fi64_to_u8 MOSI_PWM;

//SLAVER--CAMERA
uint8_t MOSI_CAMERA1[8];
uint8_t MOSI_CAMERA2[8];
uint8_t MOSI_CAMERA3[8];
uint8_t MOSI_CAMERA4[8];
uint8_t MOSI_CAMERA5[8];

//added by fxh_bool 4-10 for image save
uint8_t MISO_CAM1_TRANS[8];
uint8_t MISO_CAM2_TRANS[8];
uint8_t MISO_CAM3_TRANS[8];
uint8_t MISO_CAM4_TRANS[8];
uint8_t MISO_CAM5_TRANS[8];

//SLAVER--RELAY
uint8_t MOSI_RELAY1_DATA;
uint8_t MOSI_RELAY2_DATA;

//SLAVER--SONIC1
uint8_t MOSI_SONIC1_CMD=0;
uint8_t MISO_SONIC1_DAT[3];
//SLAVER--SONIC2
uint8_t MOSI_SONIC2_CMD=0;
uint8_t MISO_SONIC2_DAT[3];


//�¼�IDʱ����ID_NUMDEF�ж�����ӦID������
/**/
Can_Data Can_Database[]=
{
	//Data_type		Data_ID								*Data_ptr											Data_length		*MenuFunc			Channel		Fifo_num
	//channel  1
	//����&&GPSЭ����ͨ��ID*/
	{WRITE_ONLY,	M_G_CMD_ID,						(uint8_t *)(&MOSI_CMD),									1,	NULL,								1,	CAN_Filter_FIFO0},
	{WRITE_ONLY,	W_GPS_X_ID,						(uint8_t *)(MOSI_GPS_X.u8_data),				8,	NULL,								1,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_GPS_Y_ID,						(uint8_t *)(MOSI_GPS_Y.u8_data),				8,	NULL,								1,	CAN_Filter_FIFO0},
	{WRITE_ONLY,	W_GPS_ANG_ID,					(uint8_t *)(MOSI_GPS_ANG.u8_data),			8,	NULL,								1,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_GYRO_C1_ID,					(uint8_t *)(MOSI_GYRO_C1.u8_data),			8,	NULL,								1,	CAN_Filter_FIFO0},
	{WRITE_ONLY,	W_GYRO_C2_ID,					(uint8_t *)(MOSI_GYRO_C2.u8_data),			8,	NULL,								1,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_ENC_COEFF_ID,				(uint8_t *)(MOSI_ENC_COEFF.u8_data),		8,	NULL,								1,	CAN_Filter_FIFO1},
	
	{READ_ONLY,		G_M_CMD_ID,						(uint8_t *)(&MISO_CMD),									1,	G_M_CMD_FUNC,				1,	CAN_Filter_FIFO0},
	{READ_ONLY,		S_GPS_X_ID,						(uint8_t *)(MISO_GPS_X.u8_data),				8,	S_GPS_X_FUNC,				1,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_GPS_Y_ID,						(uint8_t *)(MISO_GPS_Y.u8_data),				8,	S_GPS_Y_FUNC,				1,	CAN_Filter_FIFO0},
	{READ_ONLY,		S_GPS_ANG_ID,					(uint8_t *)(MISO_GPS_ANG.u8_data),			8,	S_GPS_ANG_FUNC,			1,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_GYRO_C1_ID,					(uint8_t *)(MISO_GYRO_C1.u8_data),			8,	S_GYRO_C1_FUNC,			1,	CAN_Filter_FIFO0},
	{READ_ONLY,		S_GYRO_C2_ID,					(uint8_t *)(MISO_GYRO_C2.u8_data),			8,	S_GYRO_C2_FUNC,			1,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_GYRO_RAD_ID,				(uint8_t *)(MISO_GYRO_RAD.u8_data),			8,	S_GYRO_RAD_FUNC,		1,	CAN_Filter_FIFO0},
	{READ_ONLY,		S_ENC1_C1_ID,					(uint8_t *)(MISO_ENC1_C1.u8_data),			8,	S_ENC1_C1_FUNC,			1,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_ENC1_C2_ID,					(uint8_t *)(MISO_ENC1_C2.u8_data),			8,	S_ENC1_C2_FUNC,			1,	CAN_Filter_FIFO0},
	{READ_ONLY,		S_ENC1_RAD_ID,				(uint8_t *)(MISO_ENC1_RAD.u8_data),			8,	S_ENC1_RAD_FUNC,		1,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_ENC1_ANG_ID,				(uint8_t *)(MISO_ENC1_ANG.u8_data),			8,	S_ENC1_ANG_FUNC,		1,	CAN_Filter_FIFO0},
	{READ_ONLY,		S_ENC1_DIS_ID,				(uint8_t *)(MISO_ENC1_DIS.u8_data),			8,	S_ENC1_DIS_FUNC,		1,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_ENC2_C1_ID,					(uint8_t *)(MISO_ENC2_C1.u8_data),			8,	S_ENC2_C1_FUNC,			1,	CAN_Filter_FIFO0},
	{READ_ONLY,		S_ENC2_C2_ID,					(uint8_t *)(MISO_ENC2_C2.u8_data),			8,	S_ENC2_C2_FUNC,			1,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_ENC2_RAD_ID,				(uint8_t *)(MISO_ENC2_RAD.u8_data),			8,	S_ENC2_RAD_FUNC,		1,	CAN_Filter_FIFO0},
	{READ_ONLY,		S_ENC2_ANG_ID,				(uint8_t *)(MISO_ENC2_ANG.u8_data),			8,	S_ENC2_ANG_FUNC,		1,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_ENC2_DIS_ID,				(uint8_t *)(MISO_ENC2_DIS.u8_data),			8,	S_ENC2_DIS_FUNC,		1,	CAN_Filter_FIFO0},
	
	//����&&ISOЭ����ͨ��ID*/
	{WRITE_ONLY,	W_PWM_ID,							(uint8_t *)(MOSI_PWM.u8_data),					8,	NULL,								2,	CAN_Filter_FIFO0},
	{WRITE_ONLY,	W_STEPPER_ID,					(uint8_t *)(MOSI_stepper.u8_data),			8,	NULL,								1,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	S_SWITCH_ID,					(uint8_t *)(MOSI_SWITCH),								4,	NULL,								2,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_STEER_ID,						(uint8_t *)(MOSI_STEER.u8_data),				2,	NULL,								2,	CAN_Filter_FIFO0},
	{READ_ONLY,		BACK_FACTOR_ID,				(uint8_t *)(MOSI_FACTOR.u8_data),				4,	S_FACTOR_FUNC,			2,	CAN_Filter_FIFO1},
	{READ_ONLY,		W_STATUS_ID,					(uint8_t *)(MOSI_STATUS),								4,	S_STATUS_FUNC,			2,	CAN_Filter_FIFO1},
	//������relayͨ��ID
	{READ_ONLY,		W_RELAY1_ID,					(uint8_t *)(&MOSI_RELAY1_DATA),					1,	NULL,								2,	CAN_Filter_FIFO0},
	{READ_ONLY,		W_RELAY2_ID,					(uint8_t *)(&MOSI_RELAY2_DATA),					1,	NULL,								2,	CAN_Filter_FIFO1},

	//����&&SONIC1Э����ͨ��ID*/
	{WRITE_ONLY,	W_SONIC1_ID,					(uint8_t *)(&MOSI_SONIC1_CMD),					1,	NULL,								2,	CAN_Filter_FIFO0},
	{READ_ONLY,		S_SONIC1_ID,					(uint8_t *)(MISO_SONIC1_DAT),						3,	S_SONIC1_FUNC,			2,	CAN_Filter_FIFO1},
	//����&&SONIC2Э����ͨ��ID*/
	{WRITE_ONLY,	W_SONIC2_ID,					(uint8_t *)(&MOSI_SONIC2_CMD),					1,	NULL,								2,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_SONIC2_ID,					(uint8_t *)(MISO_SONIC2_DAT),						3,	S_SONIC2_FUNC,			2,	CAN_Filter_FIFO0},
	
	//����&&MOTORͨ��ID*/
	//1��
	{WRITE_ONLY,	W_MOTOR_ALL_SPEED_ID,	(uint8_t *)(W_MOTOR_ALL_SPEED),					8,	NULL,								1,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_MOTOR1_SPEED_ID,		(uint8_t *)(MOSI_MOTOR1_SPEED),					8,	NULL,								1,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_MOTOR2_SPEED_ID,		(uint8_t *)(MOSI_MOTOR2_SPEED),					8,	NULL,								1,	CAN_Filter_FIFO0},
	{WRITE_ONLY,	W_MOTOR3_SPEED_ID,		(uint8_t *)(MOSI_MOTOR3_SPEED),					8,	NULL,								1,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_MOTOR4_SPEED_ID,		(uint8_t *)(MOSI_MOTOR4_SPEED),					8,	NULL,								1,	CAN_Filter_FIFO0},
	//5��
	{WRITE_ONLY,	W_MOTOR5_SPEED_ID,		(uint8_t *)(MOSI_MOTOR5_SPEED.u8_data),	4,	NULL,								2,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_MOTOR5_ANGLE_ID,		(uint8_t *)(MOSI_MOTOR5_ANGLE.u8_data),	4,	NULL,								2,	CAN_Filter_FIFO0},
	//6��
	{WRITE_ONLY,	W_MOTOR6_SPEED_ID,		(uint8_t *)(MOSI_MOTOR6_SPEED.u8_data),	4,	NULL,								2,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_MOTOR6_ANGLE_ID,		(uint8_t *)(MOSI_MOTOR6_ANGLE.u8_data),	4,	NULL,								2,	CAN_Filter_FIFO0},
	//7��
	{WRITE_ONLY,	W_MOTOR7_SPEED_ID,		(uint8_t *)(MOSI_MOTOR7_SPEED.u8_data),	4,	NULL,								2,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_MOTOR7_ANGLE_ID,		(uint8_t *)(MOSI_MOTOR7_ANGLE.u8_data),	4,	NULL,								2,	CAN_Filter_FIFO0},
	//8��
	{WRITE_ONLY,	W_MOTOR8_SPEED_ID,		(uint8_t *)(MOSI_MOTOR8_SPEED.u8_data),	4,	NULL,								2,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_MOTOR8_ANGLE_ID,		(uint8_t *)(MOSI_MOTOR8_ANGLE.u8_data),	4,	NULL,								2,	CAN_Filter_FIFO0},
	//9��
	{WRITE_ONLY,	W_MOTOR9_SPEED_ID,		(uint8_t *)(MOSI_MOTOR9_SPEED.u8_data),	4,	NULL,								2,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_MOTOR9_ANGLE_ID,		(uint8_t *)(MOSI_MOTOR9_ANGLE.u8_data),	4,	NULL,								2,	CAN_Filter_FIFO0},
	
	//����&&��MOTORͨ��ID*/
	{WRITE_ONLY,	W_MOTOR1_OLD_ID,			(uint8_t *)(MOSI_MOTOR1_OLD),						8,	NULL,								2,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_MOTOR2_OLD_ID,			(uint8_t *)(MOSI_MOTOR2_OLD),						8,	NULL,								2,	CAN_Filter_FIFO0},
	{WRITE_ONLY,	W_MOTOR3_OLD_ID,			(uint8_t *)(MOSI_MOTOR3_OLD),						8,	NULL,								2,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_MOTOR4_OLD_ID,			(uint8_t *)(MOSI_MOTOR4_OLD),						8,	NULL,								2,	CAN_Filter_FIFO0},
	{WRITE_ONLY,	W_MOTOR5_OLD_ID,			(uint8_t *)(MOSI_MOTOR5_OLD),						8,	NULL,								2,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_MOTOR6_OLD_ID,			(uint8_t *)(MOSI_MOTOR6_OLD),						8,	NULL,								2,	CAN_Filter_FIFO0},
	{WRITE_ONLY,	W_MOTOR7_OLD_ID,			(uint8_t *)(MOSI_MOTOR7_OLD),						8,	NULL,								2,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_MOTOR8_OLD_ID,			(uint8_t *)(MOSI_MOTOR8_OLD),						8,	NULL,								2,	CAN_Filter_FIFO0},
	
	{READ_ONLY,		S_MOTOR1_ENC_ID,			(uint8_t *)(MISO_M1_ENC),								7,	S_MOTOR1_ENC_FUNC,	2,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_MOTOR2_ENC_ID,			(uint8_t *)(MISO_M2_ENC),								7,	S_MOTOR2_ENC_FUNC,	2,	CAN_Filter_FIFO0},
	{READ_ONLY,		S_MOTOR3_ENC_ID,			(uint8_t *)(MISO_M3_ENC),								7,	S_MOTOR3_ENC_FUNC,	2,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_MOTOR4_ENC_ID,			(uint8_t *)(MISO_M4_ENC),								7,	S_MOTOR4_ENC_FUNC,	2,	CAN_Filter_FIFO0},
	{READ_ONLY,		S_MOTOR5_ENC_ID,			(uint8_t *)(MISO_M5_ENC),								7,	S_MOTOR5_ENC_FUNC,	2,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_MOTOR6_ENC_ID,			(uint8_t *)(MISO_M6_ENC),								7,	S_MOTOR6_ENC_FUNC,	2,	CAN_Filter_FIFO0},
	{READ_ONLY,		S_MOTOR7_ENC_ID,			(uint8_t *)(MISO_M7_ENC),								7,	S_MOTOR7_ENC_FUNC,	2,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_MOTOR8_ENC_ID,			(uint8_t *)(MISO_M8_ENC),								7,	S_MOTOR8_ENC_FUNC,	2,	CAN_Filter_FIFO0},
	
	{WRITE_ONLY,	W_MOTOR1_NEW_ID,			(uint8_t *)(MOSI_MOTOR1_OLD),						8,	NULL,								2,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_MOTOR2_NEW_ID,			(uint8_t *)(MOSI_MOTOR2_OLD),						8,	NULL,								2,	CAN_Filter_FIFO0},
	
	//����&&CAMERAͨ��ID*/
	{READ_ONLY,		S_CAMERA1_ID,					(uint8_t *)(&camera_data1),	CAMERA_LENGTH,	S_CAMERA1_FUNC,			2,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_CAMERA2_ID,					(uint8_t *)(&camera_data2),	CAMERA_LENGTH,	S_CAMERA2_FUNC,			2,	CAN_Filter_FIFO0},
	{READ_ONLY,		S_CAMERA3_ID,					(uint8_t *)(&camera_data3),	CAMERA_LENGTH,	S_CAMERA3_FUNC,			2,	CAN_Filter_FIFO1},
	{READ_ONLY,		S_CAMERA4_ID,					(uint8_t *)(&camera_data4),	CAMERA_LENGTH,	S_CAMERA4_FUNC,			2,	CAN_Filter_FIFO0},
	{READ_ONLY,		S_CAMERA5_ID,					(uint8_t *)(&camera_data5),							8,	S_CAMERA5_FUNC,			2,	CAN_Filter_FIFO1},
	
	{WRITE_ONLY,	W_CAMERA1_ID,					(uint8_t *)(MOSI_CAMERA1),							8,	NULL,								1,	CAN_Filter_FIFO0},
	{WRITE_ONLY,	W_CAMERA2_ID,					(uint8_t *)(MOSI_CAMERA2),							8,	NULL,								1,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_CAMERA3_ID,					(uint8_t *)(MOSI_CAMERA3),							8,	NULL,								1,	CAN_Filter_FIFO0},
	{WRITE_ONLY,	W_CAMERA4_ID,					(uint8_t *)(MOSI_CAMERA4),							8,	NULL,								2,	CAN_Filter_FIFO1},
	{WRITE_ONLY,	W_CAMERA5_ID,					(uint8_t *)(MOSI_CAMERA5),							8,	NULL,								2,	CAN_Filter_FIFO0},
	
	//for transmit image --added by fxh_bool 4-10
	{READ_ONLY, 	S_CAM1_TRANS,					(uint8_t *)(&MISO_CAM1_TRANS),					8,	S_CAM1_TRANS_FUNC,	1,	CAN_Filter_FIFO0},
	{READ_ONLY, 	S_CAM2_TRANS,					(uint8_t *)(&MISO_CAM2_TRANS),					8,	S_CAM2_TRANS_FUNC,	1,	CAN_Filter_FIFO1},
	{READ_ONLY, 	S_CAM3_TRANS,					(uint8_t *)(&MISO_CAM3_TRANS),					8,	S_CAM3_TRANS_FUNC,	1,	CAN_Filter_FIFO0},
	{READ_ONLY, 	S_CAM4_TRANS,					(uint8_t *)(&MISO_CAM4_TRANS),					8,	S_CAM4_TRANS_FUNC,	2,	CAN_Filter_FIFO1},
	{READ_ONLY, 	S_CAM5_TRANS,					(uint8_t *)(&MISO_CAM5_TRANS),					8,	S_CAM5_TRANS_FUNC,	2,	CAN_Filter_FIFO0},
};

//HASH_TABLE��ʼֵΪ255����ΪCan_Data_NumֵС��256�����豸������256��
uint8_t HASH_TABLE[256]={255};

void Hash_Table_init(void)
{
	int i;
	
	Can_Data_Num = sizeof(Can_Database) / sizeof(Can_Database[0]);
	for(i=0;i<256;i++)
	{
		HASH_TABLE[i] = 255;
	}
	for(i=0;i<Can_Data_Num;i++)
	{
		HASH_TABLE[Can_Database[i].Data_ID] = i;
	}
}

void Write_Database(ID_NUMDEF ID_NUM)
{
	uint8_t j;
	CanTxMsg TxMessage;
	/* Check the parameters */
	if((HASH_TABLE[ID_NUM] >= Can_Data_Num)&&(Can_Database[HASH_TABLE[ID_NUM]].Data_type!=WRITE_ONLY))
	{
// 		LED4_on;
		return;
	}
	
	TxMessage.StdId=Can_Database[HASH_TABLE[ID_NUM]].Data_ID;
	TxMessage.ExtId=0;
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.DLC=Can_Database[HASH_TABLE[ID_NUM]].Data_length;
	
	for(j=0;j<Can_Database[HASH_TABLE[ID_NUM]].Data_length;j++)
	{
		TxMessage.Data[j]=(*(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+j));
	}
	
	Can_SendData(&TxMessage,Can_Database[HASH_TABLE[ID_NUM]].Channel);
	
}
