#ifndef _COMMAND_H
#define _COMMAND_H
	#include "stm32f10x.h"
	#include "can_database.h"
	
	#define CMD_SET_PSG     5   //set goal speed and goal position
	#define CMD_STOP        0
#define CMD_DEFAULT     1   // all value to default value
#define CMD_SET_PN      2   // now position to xxx
#define CMD_SET_PG      3   // goal positon to xxx
#define CMD_SET_SG      4   // goal speed to xxx
#define CMD_INIT_CAN    7   //Init can
	void W_PWM_FUNC(void);
	void W_UP_PWM_FUNC(void);
	void S_SWITCH_FUNC(void);
	void S_MOTOR_ANGLE_FUNC(void);
	void S_MOTOR5_ENC_FUNC(void);
	void Moto_Stop(ID_NUMDEF ID_NUM);
		void S_MOTOR6_ENC_FUNC(void);
	void W_MOTOR_OLD_FUNC(ID_NUMDEF ID_NUM, float rot, int16_t speed,int8_t cmd);
	void Moto_Clear_NPos(ID_NUMDEF ID_NUM);
	void Moto_Set_GSpeed(ID_NUMDEF ID_NUM, int16_t speed ); 
	
	void SET_BAT_FUNC(void);
	
extern 	u8 IO_State[8];//电机控制盒上的ST的状态，比如:电机1对应 IO_State[0]
extern float	Motor_Pos[8];//电机的绝对位置，比如:电机1对应 Motor_Pos[0]
extern int16_t Motor_RealAim[8];//电机控制盒认为的目标位置*1000后得到的整数
	
#endif
