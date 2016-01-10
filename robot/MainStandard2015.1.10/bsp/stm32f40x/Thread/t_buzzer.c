#include "includes.h"
#include "math.h"


int rt_thread_buzzer1_init(void)
{
    rt_err_t result;
    
	result = rt_thread_init(&thread_buzzer1, 
                            "thread_buzzer1",
                            rt_thread_entry_buzzer1,
                            RT_NULL,
                            &thread_buzzer1_stack[0], sizeof(thread_buzzer1_stack),
							7, 5);
    if (result == RT_EOK) 
        rt_thread_startup(&thread_buzzer1);
    else
        return RT_ERROR;

    return RT_EOK;
}

int rt_thread_buzzer2_init(void)
{
    rt_err_t result;
    
	result = rt_thread_init(&thread_buzzer2, 
                            "thread_buzzer2",
                            rt_thread_entry_buzzer2,
                            RT_NULL,
                            &thread_buzzer2_stack[0], sizeof(thread_buzzer2_stack),
							7, 5);
    if (result == RT_EOK) 
        rt_thread_startup(&thread_buzzer2);
    else
        return RT_ERROR;

    return RT_EOK;
}
	
static void rt_thread_entry_buzzer1(void* parameter)
{
	
	int value;
	Buz_On();
	LCD_SetXY(0, 6);
	LCD_WriteString("111");
	Delay_ms(2000);
	rt_mb_send(&Mb_Buzzer, (rt_uint32_t)&value);
			
}

static void rt_thread_entry_buzzer2(void* parameter)
{
	
	int value;
	while(rt_mb_send_wait(&Mb_Buzzer, (rt_uint32_t)&value, 0)!=RT_EOK)
	{

	}
	LCD_SetXY(0, 7);
	LCD_WriteString("222");
	rt_mb_delete(&Mb_Buzzer);
			
}
/********************************    END OF FILE    ***************************/