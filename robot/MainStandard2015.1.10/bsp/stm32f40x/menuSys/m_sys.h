/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MENUSYS_H
#define __MENUSYS_H

/* Includes ------------------------------------------------------------------*/
/* variable ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

void sys_Gyro(void);
void sys_Encoder(void);
void sys_GPS(void);
void sys_Mecanum(void);
void sys_SA_ST(void);
void sys_CAN(void);
void sys_Handle(void);
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
void sys_keep(void);


#endif 

/**********************************END OF FILE*********************************/
