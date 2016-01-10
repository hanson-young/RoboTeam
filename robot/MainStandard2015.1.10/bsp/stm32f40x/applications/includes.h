#ifndef _INCLUDES_H
#define _INCLUDES_H

#include <rthw.h>
#include <rtthread.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/* Application */
#include "mb.h"
#include "mq.h"

/* thread */
#include "t_handle.h"
#include "t_fatfs.h"

/* Menu */

/* UserDevices */
#include "Lcd_Keyboard.h"
#include "Mecanum.h"
#include "Gyro.h"
#include "Encoder.h"
#include "GPS.h"
#include "Relay.h"
#include "Motor.h"
#include "FanCtrl.h"

/* Route */
#include "GoLine.h"
#include "EasyLine.h"
#include "GoCircle.h"
#include "Keep.h"
#include "PointPath.h"

/*Auto*/

/* GeneralFunction */
#include "MyTypeDef.h"
#include "MyString.h"
#include "MyMath.h"
#include "PID.h"
#include "DataBack.h"

/*SDIO*/
#include "stm324xg_eval_sdio_sd.h"
#include "ff.h"

/* Driver */
#include "board.h"
#include "io.h"
#include "usart.h"
#include "dma.h"
#include "dma_rx_isr.h"

#include "pwm.h"
#include "can.h"
#include "SD_Serve.h"
#include "can_database.h"
#include "Command.h"
#include "fan.h"
#include "sd_save.h"

/*------------------------------------------------------------------*/
 #define LCD_DEBUG 1                                                //                    
                                                                    //
 #if LCD_DEBUG                                                      //
 void LCD_Assert_Fail(const char *func, const int line);            //
	 #define LCD_ERROR() LCD_Assert_Fail(__FUNCTION__, __LINE__)    //
#else                                                               //
    #define LCD_ERROR() (void)0;                                    //
 #endif                                                             //
/*------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
 #define SD_SHOWRES 1                                                       //
                                                                            //
 #if SD_SHOWRES                                                             //
	 #define SD_RES(res) \
                sd_res(res, __FUNCTION__, __LINE__)                         //
 #else                                                                      //
    #define SD_RES(FRESULT res) (void)0                                     //
 #endif                                                                     //
/*--------------------------------------------------------------------------*/
	

#endif
