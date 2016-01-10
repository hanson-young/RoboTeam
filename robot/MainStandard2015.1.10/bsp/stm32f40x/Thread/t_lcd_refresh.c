
#include "includes.h"
#include "t_lcd_refresh.h"


ALIGN(RT_ALIGN_SIZE)
static char thread_lcd_refresh_stack[1024];
struct rt_thread thread_lcd_refresh;

u8 LCD_Table[LCD_LEN];
u8 LCD_Index;
/*
 * ������: rt_thread_entry_lcd_refresh
 * ��  ��: lcdˢ���̵߳���ں���
 * ��  ��: ��
 * ��  ��: ��
 * ��  ��: �ڲ�����
 * ˵  ��: Һ����д20�У�8��;��0�п�ʼ����0�п�ʼ
 */
static void rt_thread_entry_lcd_refresh(void* parameter)
{
    int i=0;
    
    while(1)
    {
        Com_SendByte (1, 0xff);//������ͷ
        Delay_ms(5);
		
        Com_SendByte (1, 30*(i/2)+(i%2)*10);//��ʾλ�ã�������ʾ
        Delay_ms(5);
		
		Dma_Out(1, &LCD_Table[10*i],10);
        
        if(i==15)//һ��16�飬��8x(2x10)������
            i=0;
        else
            i++;
        
        Delay_ms(5);
    }
} 

/*
 * ������: rt_thread_lcd_refresh_init
 * ��  ��: lcdˢ���̵߳ĳ�ʼ��
 * ��  ��: ��
 * ��  ��: ��ʼ�����
 * ��  ��: �ⲿ����
 */
int rt_thread_lcd_refresh_init(void)
{
    rt_err_t result;
    
	result = rt_thread_init(&thread_lcd_refresh, 
                            "thread_lcd_refresh",
                            rt_thread_entry_lcd_refresh,
                            RT_NULL,
                            &thread_lcd_refresh_stack[0], sizeof(thread_lcd_refresh_stack),
							15, 5);
    if (result == RT_EOK) 
        rt_thread_startup(&thread_lcd_refresh);
    else
        return RT_ERROR;

    return RT_EOK;
}

/********************************    END OF FILE    ***************************/
