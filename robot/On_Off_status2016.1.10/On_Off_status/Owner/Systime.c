#include "all.h"


uint32_t TIME=0;
void SysTimeInit(void)
{
	TIM_TimeBaseInitTypeDef Timeinitstruct;
	NVIC_InitTypeDef NVIC_Initstruct;
	
	if(Sys_TIMER==TIM2||Sys_TIMER==TIM3||Sys_TIMER==TIM4||Sys_TIMER==TIM5||Sys_TIMER==TIM12||\
		Sys_TIMER==TIM13||Sys_TIMER==TIM14)
	RCC_APB1PeriphClockCmd(Sys_Periph,ENABLE);
	if(Sys_TIMER==TIM9||Sys_TIMER==TIM10||Sys_TIMER==TIM11)
		RCC_APB2PeriphClockCmd(Sys_Periph,ENABLE);
	
	Timeinitstruct.TIM_Prescaler=0;
	Timeinitstruct.TIM_CounterMode=TIM_CounterMode_Up;
	Timeinitstruct.TIM_Period=1000;
	Timeinitstruct.TIM_ClockDivision=TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(Sys_TIMER,&Timeinitstruct);
	
	NVIC_Initstruct.NVIC_IRQChannel=TIMx_IRQ;
	NVIC_Initstruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_Initstruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Initstruct.NVIC_IRQChannelCmd=ENABLE;
	 
	NVIC_Init(&NVIC_Initstruct);
	
	TIM_ITConfig(Sys_TIMER,TIM_IT_Update,ENABLE);
	TIM_Cmd(Sys_TIMER,ENABLE);
	
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	TIME++;
}