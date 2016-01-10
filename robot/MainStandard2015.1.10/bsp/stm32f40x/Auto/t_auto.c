
#include "includes.h"

ALIGN(RT_ALIGN_SIZE)
static char thread_auto_stack[1024];
struct rt_thread thread_auto;

/*
 * ������: rt_thread_entry_auto
 * ��  ��: �Զ��������ں���
 * ��  ��: ��
 * ��  ��: ��
 * ��  ��: �ڲ�����
 */
static void rt_thread_entry_auto(void* parameter)
{
    rt_uint32_t msg;
	
    while(1)
    {
			if(rt_mb_recv(&Mb_Auto, &msg, RT_WAITING_FOREVER) == RT_EOK)
			{
				switch(msg)
				{
					case 1:

						break;
					case 2:

						break;
					case 3:

						break;

					case 4:

						break;
					
					case 5:

						break;
					
					case 6:

						break;
					
					case 7:

						break;
					
					default :
					break;
				}
			}
    }
}

/*sy
 * ������: rt_thread_auto_init
 * ��  ��: �Զ������̵߳ĳ�ʼ��
 * ��  ��: ��
 * ��  ��: ��ʼ�����
 * ��  ��: �ⲿ����
 */
int rt_thread_auto_init(void)
{
    rt_err_t result;
    
	result = rt_thread_init(&thread_auto, 
                            "thread_auto",
                            rt_thread_entry_auto,
                            RT_NULL,
                            &thread_auto_stack[0], sizeof(thread_auto_stack),
							9, 5);
    if (result == RT_EOK)
        rt_thread_startup(&thread_auto);
    else
        return RT_ERROR;

    return RT_EOK;
}

/********************************    END OF FILE    ***************************/
