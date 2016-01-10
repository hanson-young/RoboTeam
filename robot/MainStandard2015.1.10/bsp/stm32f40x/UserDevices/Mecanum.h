/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MECANUM_H
#define __MECANUM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "MyTypeDef.h"
/* Exported macro ------------------------------------------------------------*/

#define SPEED_STOP Speed_X = Speed_Y = Speed_Rotation = 0

/* Exported variables --------------------------------------------------------*/

extern double Speed_X;
extern double Speed_Y;
extern double Speed_Rotation;
extern double speed_rate;
extern double ACC_X_1,ACC_Y_1;
extern struct Mecanum_State Mec_FL;		//四轮的状态参数
extern struct Mecanum_State Mec_FR;
extern struct Mecanum_State Mec_BL;
extern struct Mecanum_State Mec_BR;

/* Exported function prototype -----------------------------------------------*/
void Mecanum_test(struct Mecanum_State *mec);
//void SetSpeed(double fl_speed,double fr_speed,double bl_speed,double br_speed);
//void SetSpeed(void);
void SetSpeed(double speed_x,double speed_y,double speed_rotation);
void SetSpeed2(double speed_x,double speed_y,double speed_rotation);
void acc_2(void);
void keep_test(void);
void acc_1(void);
extern double speed_x_zx,speed_y_zx;
extern struct Point GPS_mes[6];
extern int x_sign,y_sign;
void collect_GPS(void);
#endif 

/**********************************END OF FILE*********************************/
