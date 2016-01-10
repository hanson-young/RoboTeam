
#include "includes.h"
#include "M_Func.h"


ALIGN(RT_ALIGN_SIZE)
static char thread_keyboard_stack[1024*3];
struct rt_thread thread_keyboard;

//double coeff_x,coeff_y,coeff_r;//���ٶ�ʱ�ſ˱Ⱦ����ϵ��

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
 * ������: UserDevices_Init
 * ��  ��: ����ʱ�����ʼ������
 * ��  ��: ��
 * ��  ��: ��
 * ��  ��: �ڲ�����
 */
void UserDevices_Init()
{
	TIM2_Init();
	Fan_Set_Min();//��ʼ����������
	//�������Ҫ��������ʼ��
	Gyro_CheckFloat();
}

/*
 * ������: rt_thread_entry_keyboard
 * ��  ��: �����̵߳���ں���
 * ��  ��: ��
 * ��  ��: ��
 * ��  ��: �ڲ�����
 * ˵  ��: �ڸ��߳����ɲ˵������û�����
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
 * ������: rt_keyboard_init
 * ��  ��: �����̵߳ĳ�ʼ��
 * ��  ��: ��
 * ��  ��: ��
 * ��  ��: �ⲿ����
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
