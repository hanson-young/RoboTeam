#ifndef __T_HANDLE_H_
#define __T_HANDLE_H_

#include "stm32f4xx.h"

#define Button1_On	((HandleData_X.rest[1]&0x04)==0x04)
#define Button1_Off	((HandleData_X.rest[1]&0x04)==0x00)
#define Button2_On	((HandleData_X.rest[1]&0x08)==0x08)
#define Button2_Off	((HandleData_X.rest[1]&0x08)==0x00)
#define Button3_On	((HandleData_X.rest[1]&0x10)==0x10)
#define Button3_Off	((HandleData_X.rest[1]&0x10)==0x00)
#define Button4_On	((HandleData_X.rest[1]&0x20)==0x20)
#define Button4_Off	((HandleData_X.rest[1]&0x20)==0x00)

#define Button5_Up	((HandleData_X.rest[0]&0x20)==0x20)
#define Button5_Down	((HandleData_X.rest[0]&0x40)==0x40)
#define Button6_Up	((HandleData_X.rest[0]&0x80)==0x80)
#define Button6_Down	((HandleData_X.rest[1]&0x02)==0x02)

#define Button7_On	((HandleData_X.rest[0]&0x02)==0x02)
#define Button7_Off	((HandleData_X.rest[0]&0x02)==0x00)
#define Button8_On	((HandleData_X.rest[0]&0x04)==0x04)
#define Button8_Off	((HandleData_X.rest[0]&0x04)==0x00)
#define Button9_On	((HandleData_X.rest[0]&0x08)==0x08)
#define Button9_Off	((HandleData_X.rest[0]&0x08)==0x00)
#define Button10_On	((HandleData_X.rest[0]&0x10)==0x10)
#define Button10_Off	((HandleData_X.rest[0]&0x10)==0x00)


#define Y_Button1_On	((HandleData_Y.rest[1]&0x04)==0x04)
#define Y_Button1_Off	((HandleData_Y.rest[1]&0x04)==0x00)
#define Y_Button2_On	((HandleData_Y.rest[1]&0x08)==0x08)
#define Y_Button2_Off	((HandleData_Y.rest[1]&0x08)==0x00)
#define Y_Button3_On	((HandleData_Y.rest[1]&0x10)==0x10)
#define Y_Button3_Off	((HandleData_Y.rest[1]&0x10)==0x00)
#define Y_Button4_On	((HandleData_Y.rest[1]&0x20)==0x20)
#define Y_Button4_Off	((HandleData_Y.rest[1]&0x20)==0x00)

#define Y_Button5_Up	((HandleData_Y.rest[0]&0x20)==0x20)
#define Y_Button5_Down	((HandleData_Y.rest[0]&0x40)==0x40)
#define Y_Button6_Up	((HandleData_Y.rest[0]&0x80)==0x80)
#define Y_Button6_Down	((HandleData_Y.rest[1]&0x02)==0x02)

#define Y_Button7_On	((HandleData_Y.rest[0]&0x02)==0x02)
#define Y_Button7_Off	((HandleData_Y.rest[0]&0x02)==0x00)
#define Y_Button8_On	((HandleData_Y.rest[0]&0x04)==0x04)
#define Y_Button8_Off	((HandleData_Y.rest[0]&0x04)==0x00)
#define Y_Button9_On	((HandleData_Y.rest[0]&0x08)==0x08)
#define Y_Button9_Off	((HandleData_Y.rest[0]&0x08)==0x00)
#define Y_Button10_On	((HandleData_Y.rest[0]&0x10)==0x10)
#define Y_Button10_Off	((HandleData_Y.rest[0]&0x10)==0x00)

int rt_thread_handle_init(void);

extern struct rt_thread thread_handle;
extern double handle_speed_y;
extern double handle_speed_x;
extern double handle_speed_turn;
extern u32 time_flag_y;
extern u32 time_flag;
void pid_set_A(void);


#endif

