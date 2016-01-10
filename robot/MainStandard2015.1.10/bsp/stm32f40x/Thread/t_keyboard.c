
#include "includes.h"
#include "M_Func.h"


ALIGN(RT_ALIGN_SIZE)
static char thread_keyboard_stack[1024*3];
struct rt_thread thread_keyboard;

//double coeff_x,coeff_y,coeff_r;//赋速度时雅克比矩阵的系数

void TIM2_Init(void)
{  
	//GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef  TIM_OCInitStructure;
	u32 timerperiod;
	
	timerperiod = 0XFFFFFFFF;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = timerperiod; 
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
	
	
	TIM_Cmd(TIM2, ENABLE);  	
}



/*
 * 函数名: UserDevices_Init
 * 描  述: 开机时外设初始化函数
 * 输  入: 无
 * 输  出: 无
 * 调  用: 内部调用
 */
void UserDevices_Init()
{
	TIM2_Init();
	Fan_Set_Min();//初始化涵道风扇
	//这个函数要放在最后初始化
	Gyro_CheckFloat();
}

/*
 * 函数名: rt_thread_entry_keyboard
 * 描  述: 按键线程的入口函数
 * 输  入: 无
 * 输  出: 无
 * 调  用: 内部调用
 * 说  明: 在该线程内由菜单进入用户函数
 */
static void rt_thread_entry_keyboard(void* parameter)
{
	static u8 pow_on_flag=1;
	
  rt_uint32_t key_value;
	
	if(pow_on_flag)
	{
		pow_on_flag = 0;
		UserDevices_Init();
	}
	
	ShowMenuList();
	
    while (1)
    {
        if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_FOREVER) == RT_EOK)
            KeyNumOperate(key_value);
    }
}

/*
 * 函数名: rt_keyboard_init
 * 描  述: 按键线程的初始化
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void rt_keyboard_init( void )
{
	rt_thread_init(&thread_keyboard,
                    "keyboard",
                    rt_thread_entry_keyboard,
                    RT_NULL,
					&thread_keyboard_stack[0], sizeof(thread_keyboard_stack),
					6,5);
    rt_thread_startup(&thread_keyboard);
}
