
#include "includes.h"

#include "M_List.h"
#include "M_Func.h"

#include "t_keyboard.h"
#include "T_Auto.h"

ALIGN(RT_ALIGN_SIZE)
static char thread_emergency_stack[1024];
struct rt_thread thread_emergency;

/*
 * ������: rt_thread_entry_emergency
 * ��  ��: ���������̵߳���ں���
 * ��  ��: ��
 * ��  ��: ��
 * ��  ��: �ڲ�����
 */

static void rt_thread_entry_emergency(void* parameter)
{
	rt_uint32_t msg;
	rt_uint32_t level;
	
    while(1)
    {
		if(rt_mb_recv(&Mb_Emergency, &msg, RT_WAITING_FOREVER) == RT_EOK)
		{
			level = rt_hw_interrupt_disable();		//���ж�
			rt_mb_detach(&Mb_Auto);
			Mb_Auto_Init();
			
			//Retryʱ������ֱ��̸߳�ɾ�����Ǻ���������ִ�в����ˣ�����Retryʱ����ɾ��
			//ͬ����Retryʱ����Ѱ����߳�Ҳ��ɾ������ôһ���棬��ʱ���������һ������̻߳�һֱ�ȴ���������ô��ĵȴ�������ʧ����
			if(msg == 6)
			{
				rt_thread_detach(&thread_keyboard);		//�����߳�����
				rt_keyboard_init();						//�����߳����³�ʼ��
				rt_thread_detach(&thread_handle);		//�ֱ��߳�����
				rt_thread_handle_init();				//�ֱ��߳����³�ʼ��
			}
			else
			{
				rt_thread_detach(&thread_keyboard);		//�����߳�����
				rt_keyboard_init();						//�����߳����³�ʼ��
				rt_thread_detach(&thread_handle);		//�ֱ��߳�����
				rt_thread_handle_init();				//�ֱ��߳����³�ʼ��
			}
			rt_thread_detach(&thread_auto);
			rt_thread_auto_init();
			
			SPEED_STOP;
// 			Moto_Stop(W_MOTOR1_OLD_ID);
// 			Moto_Stop(W_MOTOR2_OLD_ID);
// 			Moto_Stop(W_MOTOR3_OLD_ID);
// 			
			CurrentMenu = MainMenu;					//���õ�ǰ��ʾҳ��Ϊ���˵�
			ShowMenuList();
			
			rt_hw_interrupt_enable(level);			//���ж�
		}
    }
}

/*
 * ������: rt_thread_emergency_init
 * ��  ��: ���������̵߳ĳ�ʼ��
 * ��  ��: ��
 * ��  ��: ��ʼ�����
 * ��  ��: �ⲿ����
 */
int rt_thread_emergency_init(void)//���������̳߳�ʼ��
{
    rt_err_t result;
    
	result = rt_thread_init(
							&thread_emergency, 
                            "thread_emergency",
                            rt_thread_entry_emergency,
                            RT_NULL,
                            &thread_emergency_stack[0], sizeof(thread_emergency_stack),
							4, 5);
    if (result == RT_EOK)
        rt_thread_startup(&thread_emergency);
    else
        return RT_ERROR;

    return RT_EOK;
}
