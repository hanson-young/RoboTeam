#include "all.h"
float Init_Rot1=0.805,Init_Rot2=-0.12;
u16 pwm1_2_Freqz;//pwm波1，2口输出频率
u16 pwm3_4_Freqz;//pwm波3，4口输出频率
u8 H_hit_flag=0;
u8 L_hit_flag=0;
u8 F_hit_flag=0;
int main(void)
{	
	pwm1_2_Freqz=1000;//37194,42575,35817
	pwm3_4_Freqz=1000;
	RCC_Configuration();
	GPIO_Configuration();
	IO_IT_config();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Can1_Init();
	delay_init(72);
	USART1_Configuration();
	SysTick_init();
	TIM_PWM_Init();
	LED_GPIO_Config();
//PWM1_SetDuty(200);
//	PWM2_SetDuty(52);
  LED3_on;
	TIM_Config();
	MOSI_SWITCH[2]=0;
	
	
	while (1)
	{
			delay_ms(4);
	}
}
