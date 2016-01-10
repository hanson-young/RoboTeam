/**
  ******************************************************************************
  * @author  wrd_double
  * @date    2013-
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FAN_H
#define __FAN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported macro ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
/* Exported function prototype -----------------------------------------------*/

#define L_CH 1
#define R_CH 2


void Fan_Set_Max(void);
void Fan_Set_Min(void);
int Fan_Duty_Up(double);
int Fan_Duty_Dn(double);
void Fan_Stop(void);
void Fan_Duty(u8, double);
double get_fan_duty(u8 pwm_chx);
/* Exported function prototype -----------------------------------------------*/

#endif 

/**********************************END OF FILE*********************************/

