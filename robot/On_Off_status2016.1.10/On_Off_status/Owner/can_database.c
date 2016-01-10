/*
ÿ��һ��CAN�����豸
	1������Can_Data_Num����ֵ
	2��Ҫ��Can_Database�б������������Ϣ���ر�ע�ⲻҪ���
	3��void Hash_Table_init(void)�У�����¼��豸��Can_Database�б��е����
	4�����ID_NUMDEFö�ٶ����м�������ID�������������
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

//�¼�IDʱ����ID_NUMDEF�ж�����ӦID������
Can_Data Can_Database[]=
{
	//Data_type		Data_ID				*Data_ptr					Data_length		*MenuFunc		Channel		Fifo_num
//����&&ISOЭ����ͨ��ID*/
//	{WRITE_ONLY,	W_PWM_ID,			(uint8_t *)(MOSI_PWM.u8_data)		,8,		W_PWM_FUNC,			1,			CAN_Filter_FIFO0},
	{WRITE_ONLY,		S_SWITCH_ID,		(uint8_t *)(MOSI_SWITCH)			,4,		S_Prepare_FUNC,				1,			CAN_Filter_FIFO1},//�궨�������������־λ�����ط��ͣ�
	{READ_ONLY,	BACK_FACTOR_ID,	(uint8_t *)(MOSI_FACTOR.u8_data)		,4,		NULL,		1,			CAN_Filter_FIFO1},//��������Factor��Э���ط��ͣ�
	{READ_ONLY,	W_STATUS_ID,	(uint8_t *)(MOSI_STATUS)		,4,		NULL,		1,			CAN_Filter_FIFO1},//����״̬��Э���ط��ͣ�

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



