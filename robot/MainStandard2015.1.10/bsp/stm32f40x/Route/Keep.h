/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEEP_H
#define __KEEP_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "MyTypeDef.h"
#include "includes.h"
/* Exported macro ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/


/* Exported function prototype -----------------------------------------------*/
void SetKeep(struct Point end_point,double aim_radian,double speed_max);
void Keep(void);
#endif 

/**********************************END OF FILE*********************************/
