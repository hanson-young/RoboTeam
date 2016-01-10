#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define ON  1
#define OFF 0

#define LED1_on GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define LED2_on GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define LED3_on GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define LED4_on GPIO_ResetBits(GPIOC,GPIO_Pin_14)

#define LED1_off GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define LED2_off GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define LED3_off GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define LED4_off GPIO_SetBits(GPIOC,GPIO_Pin_14)				

void LED_GPIO_Config(void);

#endif /* __LED_H */
