/*
每加一个CAN总线设备
	1、更改Can_Data_Num定义值
	2、要在Can_Database列表中添加总线信息，特别注意不要搞错
	3、void Hash_Table_init(void)中，添加新加设备在Can_Database列表中的序号
	4、添加ID_NUMDEF枚举定义中加入新添ID代表的命令意义
*/

#include "all.h"

uint8_t Can_Data_Num=0;

uint8_t MOSI_SWITCH[4]={0x0,0x0,0x0,0x0};
uint8_t MOSI_STATUS[4]={0x0,0x0,0x0,0x0};

union fi64_to_u8 MOSI_PWM;
union fi64_to_u8 MOSI_UP_PWM;
union fi32_to_u8 MOSI_MOTOR_ANGLE;
union fi32_to_u8 MOSI_FACTOR;

union u16_to_u8 MOSI_BAT;
uint8_t MOSI_MOTOR5_OLD[8];
uint8_t MOSI_MOTOR3_OLD[8];
uint8_t MOSI_MOTOR6_OLD[8];
uint8_t MISO_M5_ENC[7];
uint8_t MISO_M6_ENC[7];
uint8_t MISO_M3_ENC[7];

uint8_t MOSI_MOTOR1_OLD[8];
uint8_t MOSI_MOTOR2_OLD[8];
uint8_t MOSI_MOTOR3_OLD[8];

//新加ID时，在ID_NUMDEF中定义相应ID的意义
Can_Data Can_Database[]=
{
	//Data_type		Data_ID				*Data_ptr					Data_length		*MenuFunc		Channel		Fifo_num
//主控&&ISO协主控通信ID*/
//	{WRITE_ONLY,	W_PWM_ID,			(uint8_t *)(MOSI_PWM.u8_data)		,8,		W_PWM_FUNC,			1,			CAN_Filter_FIFO0},
	{WRITE_ONLY,		S_SWITCH_ID,		(uint8_t *)(MOSI_SWITCH)			,4,		S_Prepare_FUNC,				1,			CAN_Filter_FIFO1},//标定、结束与紧急标志位（主控发送）
	{READ_ONLY,	BACK_FACTOR_ID,	(uint8_t *)(MOSI_FACTOR.u8_data)		,4,		NULL,		1,			CAN_Filter_FIFO1},//发送因数Factor（协主控发送）
	{READ_ONLY,	W_STATUS_ID,	(uint8_t *)(MOSI_STATUS)		,4,		NULL,		1,			CAN_Filter_FIFO1},//发送状态（协主控发送）

  {READ_ONLY,	S_MOTOR5_ANGLE_ID,			(uint8_t *)(MOSI_MOTOR_ANGLE.u8_data)		,4,		NULL,				1,			CAN_Filter_FIFO0},
  {READ_ONLY,	W_MOTOR5_OLD_ID,	(uint8_t *)(MOSI_MOTOR5_OLD)		,8,		NULL,		1,			CAN_Filter_FIFO1},
	{READ_ONLY,	W_MOTOR3_OLD_ID,	(uint8_t *)(MOSI_MOTOR3_OLD)		,8,		NULL,		1,			CAN_Filter_FIFO1},

  {WRITE_ONLY,		S_MOTOR5_ENC_ID,	(uint8_t *)(MISO_M5_ENC)			,7,		S_MOTOR5_ENC_FUNC,		1,			CAN_Filter_FIFO0},
	{READ_ONLY,		S_MOTOR6_ANGLE_ID,			(uint8_t *)(MOSI_MOTOR_ANGLE.u8_data)		,4,		NULL,				1,			CAN_Filter_FIFO0},
  {READ_ONLY,	W_MOTOR6_OLD_ID,	(uint8_t *)(MOSI_MOTOR6_OLD)		,8,		NULL,		1,			CAN_Filter_FIFO1},
  {WRITE_ONLY,		S_MOTOR6_ENC_ID,	(uint8_t *)(MISO_M6_ENC)			,7,		S_MOTOR6_ENC_FUNC,		1,			CAN_Filter_FIFO0},
{READ_ONLY,	  	W_MOTOR1_OLD_ID,	(uint8_t *)(MOSI_MOTOR1_OLD)					,8,		NULL,									1,			CAN_Filter_FIFO1},
	{READ_ONLY,	  	W_MOTOR2_OLD_ID,	(uint8_t *)(MOSI_MOTOR2_OLD)					,8,		NULL,									1,			CAN_Filter_FIFO0},
	{READ_ONLY,	  	W_MOTOR3_OLD_ID,	(uint8_t *)(MOSI_MOTOR3_OLD)					,8,		NULL,									1,			CAN_Filter_FIFO1},
//	{WRITE_ONLY,	W_UP_PWM_ID,			(uint8_t *)(MOSI_UP_PWM.u8_data)		,8,		W_UP_PWM_FUNC,			1,			CAN_Filter_FIFO1},
//	{READ_ONLY,	S_Play_ID,			(uint8_t *)(MOSI_Play.u8_data)		,8,		NULL,			1,			CAN_Filter_FIFO1},
	{WRITE_ONLY,	  W_BAT_ID,					(uint8_t *)(MOSI_BAT.u8_data)					,2,		SET_BAT_FUNC,					1,			CAN_Filter_FIFO0},
};

//HASH_TABLE初始值为255，认为Can_Data_Num值小于256，即设备数少于256个
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
	if((HASH_TABLE[ID_NUM] >= Can_Data_Num)&&(Can_Database[HASH_TABLE[ID_NUM]].Data_type!=READ_ONLY))
	{
		LED4_on;
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
	Can_SendData(&TxMessage);
}



