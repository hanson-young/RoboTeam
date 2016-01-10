/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include "includes.h"
#include <stdio.h>
#include <board.h>

#include "t_keyboard.h"
#include "t_lcd_refresh.h"
#include "t_emergency.h"
#include "T_Auto.h"

#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "stm32_eth.h"
#endif

#ifdef RT_USING_FINSH
#include <finsh.h>
#endif
void rt_init_thread_entry(void* parameter)
{
    /* LwIP Initialization */
#ifdef RT_USING_LWIP
    {
        extern void lwip_sys_init(void);

        /* register ethernetif device */
        eth_system_device_init();

        rt_hw_stm32_eth_init();
        /* re-init device driver */
        rt_device_init_all();

        /* init lwip system */
        lwip_sys_init();
        rt_kprintf("TCP/IP initialized!\n");
    }
#endif

//FS
		{
				
		}
//GUI
}

int rt_application_init()
{
    rt_thread_t init_thread;

#if (RT_THREAD_PRIORITY_MAX == 32)
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   2048, 8, 20);
#else
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   2048, 80, 20);
#endif	

    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);

/**********************����Ϊ�û��߳�**********************/
	
	rt_keyboard_init();						//�����̵߳ĳ�ʼ��			���ȼ�:6
	rt_thread_lcd_refresh_init();	        //lcdˢ���̳߳�ʼ��			���ȼ�:15
	rt_thread_emergency_init();		        //���������̳߳�ʼ��		    ���ȼ�:4
	rt_thread_handle_init();			    //�ֱ��̳߳�ʼ��				���ȼ�:7
	rt_thread_auto_init();				    //�Զ������̳߳�ʼ��		    ���ȼ�:9
	rt_thread_FATfs_init();				    //�ļ�ϵͳ�ĳ�ʼ��			���ȼ�:32-5=27
    /*-------------------------- ���� ---------------------------------------*/
	
	Mb_Key_Init();			//����������
	Mb_Emergency_Init();	//�������������
	Mb_Auto_Init();			//�Զ���������
	/*------------------------ ��Ϣ����--------------------------------------*/
 	Mq_Sd_Init();   //SD����Ϣ����
  
    /*------------------------------------------------------------------------*/
    rt_scheduler_sethook(ttime_hook);
    return 0;
}

/*
 *�����ļ��򿪵Ŀ���̨����
*/

void  Creat_NewFloder(const char * str)
{
		f_mkdir(str);
}

FINSH_FUNCTION_EXPORT(Creat_NewFloder , Creat A New Folder) ; 

/*@}*/

