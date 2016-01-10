/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GOROUTE_H
#define __GOROUTE_H

/* Includes ------------------------------------------------------------------*/
#include <stm32f4xx.h>
#include <includes.h>

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern u8 Route_Num;
extern u8 RouteFinish;
extern int Route[];
/* Exported function prototype -----------------------------------------------*/
/* Exported function prototype -----------------------------------------------*/

u8 Go_Route(u8 route_num);
#endif 

/**********************************END OF FILE*********************************/

