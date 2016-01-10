#ifndef _SYSTIME_H
#define _SYSTIME_H

#include "stdint.h"

extern uint32_t TIME;
#define Sys_TIMER TIM9  //系统定时器选择，TIMx，x=2,3,4,5,9,10,11,12,13,14
#define TIMx_IRQ  TIM1_BRK_TIM9_IRQn  //相应中断
                         /*
												   TIM1_BRK_TIM9_IRQn          = 24,     !< TIM1 Break interrupt and TIM9 global interrupt                    
                           TIM1_UP_TIM10_IRQn          = 25,     !< TIM1 Update Interrupt and TIM10 global interrupt                  
                           TIM1_TRG_COM_TIM11_IRQn     = 26       !< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt 
                           TIM1_CC_IRQn                = 27      !< TIM1 Capture Compare Interrupt                                    
                           TIM2_IRQn                   = 28,     !< TIM2 global Interrupt                                             
                           TIM3_IRQn                   = 29,    !< TIM3 global Interrupt                                             
                           TIM4_IRQn                   = 30,     !< TIM4 global Interrupt    
                           TIM8_BRK_TIM12_IRQn         = 43      !< TIM8 Break Interrupt and TIM12 global interrupt                   
                           TIM8_UP_TIM13_IRQn          = 44      !< TIM8 Update Interrupt and TIM13 global 
                           TIM8_TRG_COM_TIM14_IRQn     = 45      !< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt 
                           TIM8_CC_IRQn                = 46,     !< TIM8 Capture Compare Interrupt 													 
												 */
#define Sys_Periph  RCC_APB2Periph_TIM9     /*   RCC_APB2Periph_TIM9:   TIM9 clock
                                                 RCC_APB2Periph_TIM10:  TIM10 clock
                                                 RCC_APB2Periph_TIM11:  TIM11 clock
				 															           RCC_APB1Periph_TIM2:   TIM2 clock
                                                 RCC_APB1Periph_TIM3:   TIM3 clock
                                                 RCC_APB1Periph_TIM4:   TIM4 clock
                                                 RCC_APB1Periph_TIM5:   TIM5 clock
                                                 RCC_APB1Periph_TIM12:  TIM12 clock
                                                 RCC_APB1Periph_TIM13:  TIM13 clock
                                                 RCC_APB1Periph_TIM14:  TIM14 clock
																		     	 */
void SysTimeInit(void);
void TIM1_BRK_TIM9_IRQHandler(void);

#endif