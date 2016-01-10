
#include "includes.h"
#include "string.h"
ALIGN(RT_ALIGN_SIZE)
static char thread_FATfs_stack[4096];
struct rt_thread thread_FATfs;
extern	SD_Error status;
static FATFS fs;

extern FIL fil_walkground;
extern double StartAngle;
extern struct GPS_State GPS_Record[];
#define USING_TTIME 1
#define TTIME   2   /* thread timer */

typedef struct
{
    int prio1;
    int prio2;
    int us;
}ttim_t;   /* thread timer type */

static ttim_t tims[25]; /*  */
const static int nsch=sizeof(tims)/sizeof(tims[0]);
static int sch_index = 0;

void ttime_hook(struct rt_thread *from, struct rt_thread *to)
{
    tims[nsch].prio1 = from->init_priority;
    tims[nsch].prio2 =   to->init_priority;
    tims[nsch].us    = get_tim(TTIME);
    sch_index ++;
}

void ttime_reset(void)
{
    reset_tim(TTIME);
    sch_index = 0;
}

void ttime_prt(void)    /* thread time print */
{   
//     int i=0;
//     for(i=0; i<nsch; i++)
//       SD_Printf_MQ();
}

void sys_ttime(void)
{
    int i=0;
    static rt_uint32_t key_value;
    
    while( 1 )
    {
        LCD_Clear();
        for(i=0; i<8; i++)
        {
            LCD_SetXY(0, i);    
            LCD_Printf("prio:%d time:%d", tims[i].prio1, tims[i].us);
        }
        
        if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
        {
            switch(key_value)
            {
                case key1:                  break;
                case key2:                  break;
                case keyback:               return;
            }
        }
        Delay_ms(15);
    }    

}

/* end of file */

void SD_ReInit(void)
{
	rt_thread_detach(&thread_FATfs);
	rt_thread_FATfs_init();
}
void SD_Walkground(void)
{
    LCD_Clear();
		LCD_SetXY(0,2);
		LCD_Printf("Done");
    Delay_ms(200);
}

char *f_nameDeal(char *p_char)
{
	char *save = p_char;
 	while(*p_char++!=0)
 	{
 		if(*p_char==' ' || *p_char==':' || *p_char=='-' || *p_char=='(' || *p_char==')')
 			*p_char = '_'	;
 	}
	return save;
}

/*
 * ������: rt_thread_entry_FATfs
 * ��  ��: �ļ�ϵͳ�̵߳���ں���
 * ��  ��: ��
 * ��  ��: ��
 * ��  ��: �ڲ�����
 * ˵  ��: 
 */

FIL fil_pid;
FIL Mortor_info;

static void Mortor_filcrt(void)
{

    FRESULT res;
    
    res = f_open (&Mortor_info, "Mortor_info.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);    //FA_OPEN_ALWAYS��������ļ������ڣ��ʹ���
    //res = f_lseek(&fil_pid, fil_pid.fsize);   // ��������д�����Ǵ�ͷд
    //res = f_printf(&fil_pid, "f.txt");
    res = f_close(&Mortor_info);                   //ֻ�����˹رղ������ļ��Ż�����
    res = f_timestamp("Mortor_info.txt");            //���ļ�����ʱ��������ļ��رյ�ǰ���£�
		res = f_open (&Mortor_info, "Mortor_info.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
    if(res!=FR_OK)
		SD_RES(res);
}


static void route_filcrt(void)
{

    FRESULT res;
    
    res = f_open (&fil_pid, "f_pid.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);    //FA_OPEN_ALWAYS��������ļ������ڣ��ʹ���
    //res = f_lseek(&fil_pid, fil_pid.fsize);   // ��������д�����Ǵ�ͷд
    //res = f_printf(&fil_pid, "f.txt");
    res = f_close(&fil_pid);                   //ֻ�����˹رղ������ļ��Ż�����
    res = f_timestamp("f_pid.txt");            //���ļ�����ʱ��������ļ��رյ�ǰ���£�
		res = f_open (&fil_pid, "f_pid.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);    //FA_OPEN_ALWAYS��������ļ������ڣ��ʹ���
    SD_RES(res);
}

static void set_dir(void)
{
    FRESULT res;
    FILINFO fno;
    int i=0;
    DIR dir;
    static char buf[50];
    sprintf(buf, "%s_%2d", __DATE__, 3);
    f_nameDeal(buf);
    
    while(1)
    {
        sprintf(buf, "%s_%3d", __DATE__, i);
        f_nameDeal(buf);
        res = f_stat(buf, &fno);
        if (res!=FR_OK)
            break;
        i++;
    }
    sprintf(buf, "%s_%3d", __DATE__, i);
    f_nameDeal(buf);
    res = f_mkdir((const char *)buf);
    res = f_opendir(&dir, (const char *)buf);
    res = f_closedir(&dir);
    res = f_timestamp((char *)buf);
    res = f_chdir((const char *)buf);
}
 
static void rt_thread_entry_FATfs(void* parameter)
{
	char buf[128];
	uint32_t write_count = 0;
	FIL *file_tmp;
	uint32_t BytesWritten = 0;
	union {
		uint32_t temp;
		char     arr[4];
	}trs;
		
    FRESULT res;

    Delay_ms(50);
    status = SD_Init();
    if(status != SD_OK)
    {
        rt_kprintf("sd card init error.\n\r");
    }
    
    res = f_mount(&fs ,"", 0);
    SD_RES(res);
    rt_kprintf(" Capacity :%d MB\r\n", SDCardInfo.CardCapacity>>20);
    
    /** @attention : ������ʱ����Ϊ�ļ��� */
    set_dir();
	
	//��������
		route_filcrt();

    while(1)
	{  
//        rt_thread_delay(2);
		/*
		*
		*		�벻Ҫ�ڸ��߳�����Ӵ��룡����
		*
		*/
		rt_memset(buf, 0, sizeof(buf));
		if(rt_mq_recv(&mq_sd, &buf[0], sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
		{
			/*����ļ�ָ��*/
			trs.arr[0] = buf[0];
			trs.arr[1] = buf[1];
			trs.arr[2] = buf[2];
			trs.arr[3] = buf[3];
			file_tmp = (FIL *)(trs.temp);
			/*�ƶ����ϴ���д�ֽ�֮��*/
//			f_lseek(file_tmp, file_tmp->fptr);
			/*����ļ���д�ֽ���*/
			trs.arr[0] = buf[4];
			trs.arr[1] = buf[5];
			trs.arr[2] = buf[6];
			trs.arr[3] = buf[7];
			write_count = trs.temp;
			
			f_write(file_tmp, &buf[8], write_count, &BytesWritten);
            if(!strcmp(&buf[8], "THE END"))
            {
                rt_thread_delay(1);
                f_close(file_tmp);
            }
		}
    }	
}                      

void sd_reInit(void)
{
		
    FRESULT res;

    Delay_ms(50);
    status = SD_Init();
    if(status != SD_OK)
    {
        rt_kprintf("sd card init error.\n\r");
    }
    
    res = f_mount(&fs ,"", 0);
    rt_kprintf(" Capacity :%d MB\r\n", SDCardInfo.CardCapacity>>20);
    
    /** @attention : ������ʱ����Ϊ�ļ��� */
    set_dir();
	
	//��������
	route_filcrt();
    
    //��������ͷ�ļ�
    //cam_filcrt();
	
	Mortor_filcrt();
	
    res = f_open (&fil_pid, "f_pid.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);    //FA_OPEN_ALWAYS��������ļ������ڣ��ʹ���

    if(res) ((void) 0);
}


/*
 * ������: rt_thread_FATfs_init
 * ��  ��: �ļ�ϵͳ�̵߳ĳ�ʼ��
 * ��  ��: ��
 * ��  ��: ��ʼ�����
 * ��  ��: �ⲿ����
 */
int rt_thread_FATfs_init(void)
{
    rt_err_t result;
    
	result = rt_thread_init(&thread_FATfs,
						 "FatFs",
						 rt_thread_entry_FATfs,//
						 RT_NULL,
						 &thread_FATfs_stack[0],
						 4096,RT_THREAD_PRIORITY_MAX - 5,5);
    
    if (result == RT_EOK) 
        rt_thread_startup( &thread_FATfs );
    else
        return RT_ERROR;

    return RT_EOK;
}

/********************************    END OF FILE    ***************************/
