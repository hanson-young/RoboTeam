/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FanCtrl_H
#define __FanCtrl_H
#include "stm32f4xx.h"
/* Includes ------------------------------------------------------------------*/
/* variable ------------------------------------------------------------------*/
extern float  Factor;
/* Exported types ------------------------------------------------------------*/
void InitElevator(void);
void InitStepper(void);
void SetElevator(float elevator_hight,int16_t elevator_speed);
void SetStepper(float stepper_angle);
void SetFan(float fan_power);
void test_elevator(void);
void stepper_fan(void);
#endif 

/**********************************END OF FILE*********************************/
