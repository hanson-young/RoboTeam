

/**@author fxh_bool 2014-4-2
  ----------------------------------------------------------------------------------------------*
  |  ������ͷ�彫ͼ������ݷ��͵����أ��������н����ݴ浽 SD ����                       |
  -----------------------------------------------------------------------------------------------
  |     ����ͨ�Ÿ�ʽ:     ֡ͷ(cam_trsimg_fh)                                                 |
  |     1�����ط��� cam_trsimg_fh ������ͷ��                                                  	|
  |     2������ͷ��ط� cam_trsimg_fh  ������                                                 	|
  |     3�����俪ʼ�����ط������Ŵ��죩                                                        |
  |     4�����ݴ���֮������ͷ���ٴη��͸����� cam_trsimg_fh                               	|
  |     5����������յ�cam_trsimg_fh ��ȷ����(�������ͣ)                                 |
  ---------------------------------------------------------------------------------------------*/
 
 
#include "sd_save.h"
#include "includes.h"
#include "string.h"
#include "stdio.h"


u8 cam_trsimg_fh[] =            /* camera transmit image frame head */
    {0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa};

int Cam_Sta[7];
int Cam_Mode[7];
int Cam_ErrFlg[7];
int Time_Stamp;     /* check timeout */
    
static const u8 camID_hash[] =
    { W_CAMERA1_ID, W_CAMERA2_ID, W_CAMERA3_ID, W_CAMERA4_ID, W_CAMERA5_ID };
static const u8 *camBuf_hash[] = 
    {&MISO_CAM1_TRANS[0], &MISO_CAM2_TRANS[0], &MISO_CAM3_TRANS[0], &MISO_CAM4_TRANS[0], &MISO_CAM5_TRANS[0]};

img_cbuf_t Img_Cbuf;/* image circular buffer (1k byte) */

static FIL img_file[5];

// static char *_name(char *p_char)
// {
// 	char *save = p_char;
//  	while(*p_char++!=0)
//  	{
//  		if(*p_char==' ' || *p_char==':')
//  			*p_char = '_'	;
//  	}
// 	return save;
// }
/**---------------------------------------------------------*
 |  ����ͼ����ܺ������ڲ˵��е���                       |
 |  @attention: camx: 1~6                                   |
 *----------------------------------------------------------*/
#include "string.h"
u8 Save_Img(        /* return 0:OK , 1:wrong parameter �� 2:camera response timeout */
    u8 *f_name, 
    u8 camx         /* range:1~6 */
)
{
    FRESULT res;
    int i=0;
    CanTxMsg txmsg;
    u32 can_cnt = 0;
    char fn_buf[50];  /* file name buf */
		CAN_TypeDef * canx;
		CAN_TypeDef * _cans[] = {CAN1, CAN2};
    
    if(camx==0 || camx>6 || f_name==0 || f_name[0]==0)
    {
        LCD_ERROR();
        return camErr_Param;
    }
    
		canx =_cans[ Can_Database[ HASH_TABLE[ camID_hash[ camx-1 ] ] ].Channel - 1];
		
    /* --init-- */
    Cam_ErrFlg[camx-1]  = camErr_OK;
    Cam_Sta[camx-1]     = camSta_NoReady;
    Cam_Mode[camx-1]    = camMod_tran;
    Img_Cbuf.r_index = 0;
    Img_Cbuf.w_index = 0;
    
    /* create file */
    sprintf(fn_buf, "%s__%d", f_name, rt_tick_get()/200);
    res = f_open (&img_file[camx-1], fn_buf, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
    if(res != FR_OK)
    {
        LCD_ERROR();
        return camErr_file;
    }
    
    /* send transmit image cammand to camera */
    txmsg.StdId=camID_hash[ camx-1 ];
    txmsg.ExtId=0;
    txmsg.IDE=CAN_ID_STD;
    txmsg.RTR=CAN_RTR_DATA;
    txmsg.DLC=8;
    for(i=0; i<8; i++)
        txmsg.Data[i] = cam_trsimg_fh[i];
    while(CAN_Transmit(canx, &txmsg)==CAN_NO_MB && can_cnt<200)
    {
        can_cnt ++;
        rt_thread_delay(1);
    }
    
    /* send cammand  timeout */
    if(can_cnt>=200)
    {
        LCD_ERROR();
        
        /* revmove file */
        f_unlink((const char *)f_name);
        
        return camErr_can | camErr_Timeout;
    }
    
    /* wait reply from camera */
    while(Cam_Sta[camx-1]==camSta_NoReady)
    {
        static u32 wait_time=0;
        wait_time ++;
        
        /* wait 2000ms at most */
        if(wait_time>400)
            break;
        
        Delay_ms(5);
        
        /* it err */
        if(Cam_ErrFlg[camx-1]!=camErr_OK)
        {
            LCD_ERROR();
            return Cam_ErrFlg[camx-1];
        }
    }
    
    /* time out ? */
    if(Cam_Sta[camx-1]==camSta_NoReady)
    {
        LCD_ERROR();
        
        /* remove file */
        //f_unlink((const char *)f_name);
        f_close(&img_file[camx-1]);
        f_timestamp(fn_buf);
        return camErr_Timeout;
    }
    
    while(Cam_Sta[camx-1]==camSta_Ready) 
        rt_thread_delay(1);
    
    /* begin transmit */
    while(Cam_Sta[camx-1]==camSta_Transmit && Cam_ErrFlg[camx-1]==camErr_OK)
    {
        LCD_Clear();
        LCD_SetXY(0, 3);
        LCD_Printf("data transmiting ^_^ ");
        while(Img_Cbuf.r_index!=Img_Cbuf.w_index)
        {
            /* save image data in SD card */
            u32 byte_length;
            
            res = f_write (&img_file[camx-1], &Img_Cbuf.buf[Img_Cbuf.r_index], 8, &byte_length);
            if(res!=FR_OK)
            {
                f_unlink((const char *)f_name);
                LCD_ERROR();
                return camErr_file;
            }
            
            Img_Cbuf.r_index += 8;
            if(Img_Cbuf.r_index==sizeof(Img_Cbuf.buf))
            {
                Img_Cbuf.r_index = 0;
            }
        }
        Delay_ms(5);
        
        Time_Stamp ++;
        if(Time_Stamp>20)
        {
            LCD_ERROR();
            ///* remove file */
            //f_unlink((const char *)f_name);
            f_close(&img_file[camx-1]);
            f_timestamp(fn_buf);
            return camErr_Timeout;
        }
    }
    
    if(Cam_Sta[camx-1]==camSta_Finish)
    {
        Cam_Mode[camx-1] = camMod_norm;

        /* close file */
        f_close(&img_file[camx-1]);
        f_timestamp(fn_buf);
    }
    else
    {/* err */
        LCD_ERROR();
        
        //modified by fxh 2014 4-27
        ///* remove file */ 
        //f_unlink((const char *)f_name);
        
        f_close(&img_file[camx-1]);
        f_timestamp(fn_buf);
        
        return Cam_ErrFlg[camx-1];
    }
    return camErr_OK;
}


/*-------------------------------------------------------*
 |  can receive isr (to save image data in Img_Cbuf)     |
 *-------------------------------------------------------*/
extern uint8_t MISO_CAM1_TRANS[8];
void img_save_isr(u8 camx)
{  
    if(Cam_Sta[camx-1] == camSta_NoReady)
    {/* this frame should be cam_trsimg_fh from camra */
        if( !strncmp( (const char *)camBuf_hash[camx-1], (const char *)cam_trsimg_fh, sizeof(cam_trsimg_fh) ) )
        {
            Cam_Sta[camx-1] = camSta_Ready;
        }
        else
        {
            /* Err: it and frame */
            Cam_ErrFlg[camx-1] = camErr_it | camErr_frame;
        }
    }
    else
    {
        static int img_fcnt=0;

        if(img_fcnt==0)     
            Cam_Sta[camx-1] = camSta_Transmit;
            
        if(Cam_Sta[camx-1] != camSta_Finish)
        {
            Time_Stamp = 0;
            if(img_fcnt<IMG_SIZE/8)
            {/* save image data in Img_Cbuf */
                int i;
                for(i=0; i<8; i++)
                    *(uint8_t *)&Img_Cbuf.buf[Img_Cbuf.w_index ++] =  *((u8 *)(camBuf_hash[camx-1]) + i);

                if(Img_Cbuf.w_index==sizeof(Img_Cbuf.buf))
                    Img_Cbuf.w_index = 0;
                
                if(Img_Cbuf.w_index == Img_Cbuf.r_index)
                {
                    /* Err: it and buffer overflow*/
                    Cam_ErrFlg[camx-1] = camErr_bufof | camErr_it;
                }
                img_fcnt ++;
            }
            else
            {/* receive end frame */
                if( !strncmp( (const char *)camBuf_hash[camx-1], (const char *)cam_trsimg_fh, sizeof(cam_trsimg_fh) ) )
                {
                    /* restore normal state */
                    Cam_Sta[camx-1] = camSta_Finish;
                    img_fcnt = 0;
                }
                else
                {
                    /* Err: it and frame */
                    Cam_ErrFlg[camx-1] = camErr_it|camErr_frame;
                    img_fcnt = 0;
                }
            }
        }
    }
}

int buz_debug = 0;
static int buz_delay = 150;
static int onCnt[3] = {0};
void buz_msg(int on1, int on2, int on3, int dly)
{
    onCnt[0] = on1;
    onCnt[1] = on2;
    onCnt[2] = on3;
    buz_delay = dly;
}



 extern int vsprintf(char *string, const char *format, va_list param);

/**
  * @brief  ֱ����SD��д���ݣ�
  * @note   1������һ���ļ����д����ʱ����Ҫ���е���f_lseek()�ƶ���д����λ�ã�
  *			2. ���軻�У���������Ϣ�����\r\n���ɣ���ػ��з��ͻس��������ϣ�������ܻ���ʧ�ܣ�
  * @param  FIL* fp����д�ļ�ָ�룻 
  * @retval None
  */
FRESULT SD_Printf(FIL* fp, char *fmt,...)
{
	uint32_t num;
	FRESULT res;
    UINT temp;
    UINT *bw = &temp;
	char __SD_Printf_Buf[128];
    va_list ap;
	unsigned char *pStr = (unsigned char *)__SD_Printf_Buf;
    va_start(ap,fmt);
    num = vsprintf(__SD_Printf_Buf,fmt,ap);	
	va_end(ap);
	res = f_write (fp, pStr, num, bw);
	return res;
}

/**
  * @brief  ͨ����FATFS�̷߳���message queue��SD��д���ݣ�
  * @note   1������һ���ļ����д����ʱ��ֻ��ֱ�ӵ��øú������ɣ������ٵ���f_lseek()��
  *			2. ���軻�У���������Ϣ�����\r\n���ɣ���ػ��з��ͻس��������ϣ�������ܻ���ʧ�ܣ�
  *	           ��ȷ��ʽ��SD_Printf_MQ(&Fp, "robocon test_number %d\r\n", i);
  * @param  FIL* fp����д�ļ�ָ�룻 
  * @retval None
  */
FRESULT SD_Printf_MQ(FIL* fp, char *fmt,...)
{
	FRESULT res;
	int result;
	union {
		uint32_t temp;
		char     arr[4];
		}trs;
	char SD_Printf_Buf[128];

    va_list ap;
	unsigned char *pStr = (unsigned char *)SD_Printf_Buf;
	/*�洢�ļ�ָ�뵽SD_Printf_Buf*/
	trs.temp = (uint32_t)fp;
	SD_Printf_Buf[0] = trs.arr[0];
	SD_Printf_Buf[1] = trs.arr[1];
	SD_Printf_Buf[2] = trs.arr[2];
	SD_Printf_Buf[3] = trs.arr[3];       
    va_start(ap,fmt);
    trs.temp = vsprintf(&SD_Printf_Buf[8],fmt,ap);	
	va_end(ap);
	/*�洢�����ֽ�����SD_Printf_Buf*/
	SD_Printf_Buf[4] = trs.arr[0];
	SD_Printf_Buf[5] = trs.arr[1];
	SD_Printf_Buf[6] = trs.arr[2];
	SD_Printf_Buf[7] = trs.arr[3];		
	/*���͵���Ϣ����*/
	result = rt_mq_send(&mq_sd, pStr, sizeof(SD_Printf_Buf));
	if(result == -RT_EFULL)
	{
		buz_msg(2,2,2,200);
		rt_kprintf("message queue full, delay 1s\r\n");
		rt_thread_delay(100);
	}
	else 
		buz_msg(0,0,0,200);
	return res;
}

/** д��һ���ַ���֮���Զ����� */


FRESULT SD_Printf2(FIL* fp, char *fmt,...)
{
	uint32_t num;
	FRESULT res;
    UINT temp;
    UINT *bw = &temp;
	char __SD_Printf_Buf[128];
    
    va_list ap;
	unsigned char *pStr = (unsigned char *)__SD_Printf_Buf;
    va_start(ap,fmt);
    num = vsprintf(__SD_Printf_Buf,fmt,ap);	
	va_end(ap);
	
    __SD_Printf_Buf[num++] = 0x0d;
    __SD_Printf_Buf[num++] = 0x0a;
    
    res = f_write (fp, pStr, num, bw);
    
	return res;
}


/**-----------------------------------------------+
  |  ���ļ���ʱ���                              |
  +-----------------------------------------------*/

 
#include "stdio.h"
#include "string.h"
// static const char wdat_name[7][3] = {
// 	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
// };

static const char mon_name[12][3] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static int get_mon(char *date)
{
	int i;
	for(i=0; i<12; i++)
		if(strncmp(date, mon_name[i], 3)==0)
			return ++i;
	return 0;
}
static int get_mday(char date[])
	{return (date[4]-'0')*10 + (date[5]-'0');}
static int get_year(char date[])
	{return (date[7]-'0')*1000 + (date[8]-'0')*100 + (date[9]-'0')*10 + (date[10]-'0');}
static int get_hour(char time[])
	{return (time[0]-'0')*10 + (time[1]-'0');}
static int get_min(char time[])
	{return (time[3]-'0')*10 + (time[4]-'0');}
static int get_sec(char time[])
	{return (time[6]-'0')*10 + (time[7]-'0');}
 
FRESULT set_timestamp (
    char *obj,     /* Pointer to the file name */
    int year,
    int month,
    int mday,
    int hour,
    int min,
    int sec
)
{
    FILINFO fno;

    fno.fdate = (WORD)(((year - 1980) * 512U) | month * 32U | mday);
    fno.ftime = (WORD)(hour * 2048U | min * 32U | sec / 2U);

    return f_utime(obj, &fno);
}

FRESULT f_timestamp ( char *obj )    /* Pointer to the file name */
{
    int year  = get_year(__DATE__);
    int month = get_mon(__DATE__);
    int mday  = get_mday(__DATE__); 
    int hour  = get_hour(__TIME__);
    int min   = get_min(__TIME__);
    int sec   = get_sec(__TIME__);
    FILINFO fno;

    fno.fdate = (WORD)(((year - 1980) * 512U) | month * 32U | mday);
    fno.ftime = (WORD)(hour * 2048U | min * 32U | sec / 2U);

    return f_utime(obj, &fno);
}

/**----------------------------------------+
  | ��ʾ�ļ������Ľ��                  |
  +----------------------------------------*/
static char *sd_resStr[] =
{
	"FR_OK",				    /* (0) Succeeded */
	"FR_DISK_ERR",			    /* (1) A hard error occurred in the low level disk I/O layer */
	"FR_INT_ERR",				/* (2) Assertion failed */
	"FR_NOT_READY",			    /* (3) The physical drive cannot work */
	"FR_NO_FILE",				/* (4) Could not find the file */
	"FR_NO_PATH",				/* (5) Could not find the path */
	"FR_INVALID_NAME",		    /* (6) The path name format is invalid */
	"FR_DENIED",				/* (7) Access denied due to prohibited access or directory full */
	"FR_EXIST",				    /* (8) Access denied due to prohibited access */
	"FR_INVALID_OBJECT",		/* (9) The file/directory object is invalid */
	"FR_WRITE_PROTECTED",		/* (10) The physical drive is write protected */
	"FR_INVALID_DRIVE",		    /* (11) The logical drive number is invalid */
	"FR_NOT_ENABLED",			/* (12) The volume has no work area */
	"FR_NO_FILESYSTEM",		    /* (13) There is no valid FAT volume */
	"FR_MKFS_ABORTED",		    /* (14) The f_mkfs() aborted due to any parameter error */
	"FR_TIMEOUT",				/* (15) Could not get a grant to access the volume within defined period */
	"FR_LOCKED",				/* (16) The operation is rejected according to the file sharing policy */
	"FR_NOT_ENOUGH_CORE",		/* (17) LFN working buffer could not be allocated */
	"FR_TOO_MANY_OPEN_FILES",	/* (18) Number of open files > _FS_SHARE */
	"FR_INVALID_PARAMETER"	    /* (19) Given parameter is invalid */   
};

void sd_res(FRESULT res, const char *func, int line)
{
    //LCD_Clear();
    LCD_SetXY(0, 6);
    LCD_WriteString("func:");
    LCD_WriteString((char *)func);
    LCD_SetXY(0, 7);
    LCD_WriteString("line:");
    LCD_WriteInt(line);
    LCD_SetXY(0, 5);
    LCD_WriteString(sd_resStr[res]);
    LCD_WriteInt(res);
}
