/**
  ******************************************************************************
  * @author  wrd_double
  * @date    2013-
  * @brief   
  * @note

  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#define __AR_TEST_
/* Includes ------------------------------------------------------------------*/
#include "pwm.h"
#include "fan.h"
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/**@attention :存储最后一次发给文林的光隔板的pwm波的占空比的值
 */
static double pwm_duty[4] = {20.0, 20.0, 20.0, 20.0};



/**@attention :给涵道开到最大速度
 *
 */
void Fan_Set_Max(void)
{
    pwm_duty[L_CH-1] = 43.0;
    pwm_duty[R_CH-1] = 43.0;
    W_PWM_FUNC(pwm_duty[0], pwm_duty[1], pwm_duty[2], pwm_duty[3]);
}

/**@attention :停止涵道
 */
void Fan_Set_Min(void)
{
    pwm_duty[L_CH-1] = 20.0;
    pwm_duty[R_CH-1] = 20.0;
    W_PWM_FUNC(pwm_duty[0], pwm_duty[1], pwm_duty[2], pwm_duty[3]);
}

/**@attention :步进增加涵道的占空比 
 */
int Fan_Duty_Up(double delt)
{
    //left channel
    if(pwm_duty[L_CH-1]+delt>43.0)    
    {
        pwm_duty[L_CH-1] = 43.0;
    }
    else
    {
        pwm_duty[L_CH-1] += delt;
    }
    
    //right channel
    if(pwm_duty[R_CH-1]+delt>43.0)    
    {
        pwm_duty[R_CH-1] = 43.0;
    }
    else
    {
        pwm_duty[R_CH-1] += delt;
    }
    
    W_PWM_FUNC(pwm_duty[0], pwm_duty[1], pwm_duty[2], pwm_duty[3]);
    return 1;
}

int Fan_Duty_Dn(double delt)
{
        //left channel
    if(pwm_duty[L_CH-1]-delt<20.0)    
    {
        pwm_duty[L_CH-1] = 20.0;
    }
    else
    {
        pwm_duty[L_CH-1] -= delt;
    }
    
    //right channel
    if(pwm_duty[R_CH-1]-delt<20.0)    
    {
        pwm_duty[R_CH-1] = 20.0;
    }
    else
    {
        pwm_duty[R_CH-1] -= delt;
    }
    
    
    W_PWM_FUNC(pwm_duty[0], pwm_duty[1], pwm_duty[2], pwm_duty[3]);
    return 1;
}

void Fan_Stop(void)
{
    Fan_Set_Min();
}

/* 涵道的比率计算 100.0时是涵道最大的风速 */
/**@attention :pwm_chx range  1~4
 */
void Fan_Duty(u8 pwm_chx, double duty)
{
    assert_param(duty<=100.0);
    
    /* 限幅 */
    if(duty>100.0)
    {
        pwm_duty[pwm_chx-1] = 43.0;
        W_PWM_FUNC(pwm_duty[0], pwm_duty[1], pwm_duty[2], pwm_duty[3]);
        return;
    }
    else if(duty<0.0)
    {
        pwm_duty[pwm_chx-1] = 20.0;
        W_PWM_FUNC(pwm_duty[0], pwm_duty[1], pwm_duty[2], pwm_duty[3]);
        return;
    }
    
    pwm_duty[pwm_chx-1] = 20.0 + duty*(43.0-20.0)/100.0;
	W_PWM_FUNC(pwm_duty[0], pwm_duty[1], pwm_duty[2], pwm_duty[3]);
}

/**@attention: 返回某路涵道的值
 */
double get_fan_duty(u8 pwm_chx)
{
    if(pwm_chx!=L_CH && pwm_chx!=R_CH)
    {
        LCD_Clear();
        
        LCD_Printf("err in [%s]", __FUNCTION__);
    }
    
    return pwm_duty[pwm_chx-1];
}



