#ifndef _ALL
#define _ALL

#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "on_off_status.h"
#include "delay.h"
#include "led.h"
#include "pwm.h"
#include "Command.h"
#include "can.h"
#include "can_database.h"
#include "Systick.h"
#include "Uart.h"
#include "system_stm32f10x.h"
#include "misc.h"
#define SPEED_CTRL 30000
extern long int time_test;
extern long int time1;
extern long int time_H_hit;
extern long int time_L_hit;
extern u8 L_hit_flag;
extern u8 H_hit_flag;
extern u16 H_HIT;
extern u16 L_HIT;
extern u8 F_hit_flag;
extern long int f_hit_time;

#endif
