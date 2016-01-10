/*
 * File      : SD_srvice.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2014 XJTU Robot Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2014-02-16     dong      first implementation
 */
/* ========= includes ========== */
#include "includes.h"
#include <stdlib.h>
#include <string.h>
#include "finsh.h"
#include <stdarg.h>

/* ========= �궨�� =========== */
/*  ��databack�ļ��д洢���������  */
#define  MAX_STORE  5000
/*  ��init �ļ��д洢��������� */
#define  SD_SAVE_LEN  512


/* ======== private ===========*/
static	FATFS 		fs;
static 	FIL  		FileInit;
static	FIL 		FileDataBack;
static 	UINT 		BW;
static 	DWORD		SDRWNum   ; //��д�ĸ���
static 	uint8_t 	SDBuffer[SD_SAVE_LEN];


/* ========== extern ========= */

struct _SDData SDSave[]  = 	
{
/*	name           	  		pointer of value			type     */
	{ "Gyro_convert1"		, &Gyro.convert1 			, Type_Fp64 	} ,
	{ "Gyro_convert2" 	 	, &Gyro.convert2  			, Type_Fp64 	} ,
	{ "Encoder1_convert1"   , &Encoder[0].convert1  	, Type_Fp64 	} ,
	{ "Encoder1_convert2" 	, &Encoder[0].convert2  	, Type_Fp64 	} ,
	{ "Encoder1_r"			, &Encoder[0].r				, Type_Fp64 	} ,
	{ "Encoder1_radian"  	, &Encoder[0].radian 		, Type_Fp64 	} ,
	{ "Encoder2_convert1" 	, &Encoder[0].convert1		, Type_Fp64 	} ,
	{ "Encoder2_convert2"	, &Encoder[0].convert2		, Type_Fp64		} ,
	{ "Encoder2_r"			, &Encoder[0].r				, Type_Fp64		} ,
	{ "Encoder2_radian"		, &Encoder[0].radian		, Type_Fp64		} ,
	{ "d"  	, &Encoder[0].radian 		, Type_Fp64 	} ,
	{ "Encoder2_convert1" 	, &Encoder[0].convert1		, Type_Fp64 	} ,
	{ "Encoder2_convert2"	, &Encoder[0].convert2		, Type_Fp64		} ,
	{ "Encoder2_r"			, &Encoder[0].r				, Type_Fp64		} ,
	{ "Encoder2_radian"		, &Encoder[0].radian		, Type_Fp64		} ,
};

extern int vsprintf(char *string, char *format, va_list param);
/*******************************************************************************
* Function Name  : SD_Write
* Description    : д�ַ������ļ���
* Input          : 
* Return         : err
*******************************************************************************/
static FRESULT SD_Write(FIL* fp, UINT* bw, char *fmt,...)
{
	int num;
	char __SD_Printf_Buf[128];
	unsigned char *pStr = (unsigned char *)__SD_Printf_Buf;
	FRESULT res;
	va_list ap;
	
	va_start(ap,fmt);
	num = vsprintf(__SD_Printf_Buf,fmt,ap);	
	va_end(ap);

	res = f_write (fp, pStr, num, bw);
	return res;
}



/*******************************************************************************
* Function Name  : Switch_Save
* Description    : ����Ҫ�洢����������ѡ�񱣴���ֽ���
*				 : ����ĸ�ʽ��  name:ԭʼռ�õ��ڴ�:ת����ASCII�����ַ����������û���
*				 : ʵ�ʳ����ȡ����ԭʼռ�õ��ڴ棬���Ծ�����SD���иı��ַ�����ֵ��Ҳû�á�
* Input          : @fp     �ļ���ָ��
*				   @sddata ��Ҫ��������ݵĽṹ�� ��һ��ȫ�ֱ���
* Return         : none
*******************************************************************************/
static void Switch_Save(FIL * fp ,  struct _SDData *sddata  )
{
		static uint32_t BW;
		union udouble udouble_temp 	;
		union uint64  uint64_temp 	;
		union ufloat  ufloat_temp 	;    
		union uint32  uint32_temp 	;
		union uuint32 uuint32_temp	;
		union uint16  uint16_temp	;
		union uuint16 uuint16_temp	;
		//д����������
		SD_Write( fp ,  &BW , "@%s:",sddata->name );
		switch(sddata->value_type){
			case Type_Fp64 :
				udouble_temp.value = *(double*)sddata->value ;
				/* �洢�ڴ��е�ԭʼֵ */
				f_write( fp , udouble_temp.data , 8 , &BW ) ;
				/* ���ַ�������ʽ��ӡ���� ���������� */
				SD_Write( fp ,  &BW , ":%lf;    \n", udouble_temp.value);
			break ;
			case Type_Int64 :
				uint64_temp.value = *(int64_t*)sddata->value ;
				f_write( fp , uint64_temp.data , 8 , &BW ) ; 
				SD_Write( fp ,  &BW , ":%lld;    \n", uint64_temp.value);
			break ;
			case Type_Fp32 :
				ufloat_temp.value = *(float*)sddata->value ;
				f_write( fp , ufloat_temp.data , 4 , &BW ) ; 
				SD_Write( fp ,  &BW , ":%f;     \n", ufloat_temp.value);
			break ;
			case Type_Int32 :
				uint32_temp.value = *(int32_t*)sddata->value ;
				f_write( fp , uint32_temp.data , 4 , &BW ) ; 
				SD_Write( fp ,  &BW , ":%d;     \n", uint32_temp.value);
			break ; 
			case Type_Uint32:
				uuint32_temp.value = *(uint32_t*)sddata->value ;
				f_write( fp , uuint32_temp.data , 4 , &BW ) ; 
				SD_Write( fp ,  &BW , ":%u;     \n", uuint32_temp.value);
			break ;
			case Type_Uint16:
				uuint16_temp.value = *(uint16_t*)sddata->value ;
				f_write( fp , uuint16_temp.data , 2 , &BW ) ; 
				SD_Write( fp ,  &BW , ":%d;     \n", uuint16_temp.value);
			break ;
			case Type_Int16 :
				uint16_temp.value = *(int16_t*)sddata->value ;
				f_write( fp , uint16_temp.data , 2 , &BW ) ; 
				SD_Write( fp ,  &BW , ":%d;     \n", uint16_temp.value);
			break ; 				
			case Type_Int8 :
				f_write( fp , (int8_t*)sddata->value  , 1 , &BW ) ; 
				SD_Write( fp ,  &BW , ":%c;     \n", (int8_t*)sddata->value);
			break ; 
			case Type_Uint8 :
				f_write( fp , (uint8_t*)sddata->value , 1 , &BW ) ; 
				SD_Write( fp ,  &BW , ":%c;     \n", (uint8_t*)sddata->value);
			break ; 
			case Type_Gyro :
				
				f_write( fp , (uint8_t*)sddata->value , 1 , &BW ) ; 
				SD_Write( fp ,  &BW , ":%c;     \n", (uint8_t*)sddata->value);
			break ; 
			case Type_Encoder :
				
				f_write( fp , (uint8_t*)sddata->value , 1 , &BW ) ; 
				SD_Write( fp ,  &BW , ":%c;     \n", (uint8_t*)sddata->value);
			break ; 
			case Type_PID :
				
				f_write( fp , (uint8_t*)sddata->value , 1 , &BW ) ; 
				SD_Write( fp ,  &BW , ":%c;     \n", (uint8_t*)sddata->value);
			break ; 
			case Type_Mecanum :
				
				f_write( fp , (uint8_t*)sddata->value , 1 , &BW ) ; 
				SD_Write( fp ,  &BW , ":%c;     \n", (uint8_t*)sddata->value);
			break ;
		}
		f_sync(fp);
}


/*******************************************************************************
* Function Name  : located
* Description    : ��λĳ��������λ��
* Input          : 	@str    ��SD�����������ַ���
*					@sddata ��Ҫ��������ݵĽṹ�� ��һ��ȫ�ֱ���
* Return         :  û���ҵ���-1 �� ��ȷ�����ڵ���0����
*******************************************************************************/
extern int sscanf( uint8_t *buffer, const char *format, ... );
static int32_t  located(uint8_t *str , struct _SDData *sddata )
{
		int32_t index ;
		char name_str[20]  ;

		for( index = 0 ; index < SD_SAVE_LEN ; index++){
			if(*str == '@'){
				sscanf( str , "%*c%[^:]" , name_str );
				if(sizeof(name_str ) == sizeof(sddata->name) ){
					if(rt_strncmp(name_str , sddata->name ,sizeof(name_str)) == 0){						
						return index ;
					}
				}
			}
			str ++ ;
		}
		return -1 ;
}




/*===========================����Ϊ�ⲿ���õĽӿں���========================*/

/*******************************************************************************
* Function Name  : ValueInit
* Description    : ��ʼ��ĳ������
* Input          : ��Ҫ��������ݵĽṹ��
* Return         : 
*******************************************************************************/
rt_err_t ValueInit( struct _SDData *sddata ) 
{
		FIL *fp = &FileInit ;
		FRESULT res;
		int32_t   location;	
		char * str_ptr = &(sddata->name[0]);
		union udouble udouble_temp 	;
		union uint64  uint64_temp 	;
		union ufloat  ufloat_temp 	;    
		union uint32  uint32_temp 	;
		union uuint32 uuint32_temp	;
		union uint16  uint16_temp		;
		union uuint16 uuint16_temp	;
	
		rt_memset( SDBuffer , 0 ,  SD_SAVE_LEN);
		
		res = f_open (fp, "Init.txt",  FA_READ | FA_WRITE);
		if(res != FR_OK){
				//show message
		}
		
		f_read (fp , SDBuffer , SD_SAVE_LEN , (UINT*)&SDRWNum);

		location = located(SDBuffer , sddata  );
		if(location != -1){		
			switch(sddata->value_type){
				case Type_Fp64 :
						udouble_temp.value  = 0;
						rt_memcpy( udouble_temp.data ,&SDBuffer[location + 2 + strlen(str_ptr)] , 8);
						*(double *)sddata->value = udouble_temp.value ;
					break;
				case Type_Int64 :
						uint64_temp.value = 0 ;
						rt_memcpy( uint64_temp.data ,&SDBuffer[location + 2 + strlen(str_ptr)] , 8);
						*(int64_t *)sddata->value = uint64_temp.value ;
					break;
				case Type_Fp32 :
					ufloat_temp.value = 0;
					  rt_memcpy(ufloat_temp.data , &SDBuffer[location + 2 + strlen(str_ptr)] , 4);
						*(float *)sddata->value = ufloat_temp.value ;
					break ;
				case Type_Int32 :
					uint32_temp.value = 0;
					rt_memcpy( uint32_temp.data ,&SDBuffer[location + 2 + strlen(str_ptr)] , 4);
						*(int32_t *)sddata->value = uint32_temp.value ;
					break;
				case Type_Uint32 :
					uuint32_temp.value = 0;
					  rt_memcpy( uuint32_temp.data , &SDBuffer[location + 2 + strlen(str_ptr)] ,4);
						*( uint32_t*)sddata->value = uuint32_temp.value ;
					break;
				case Type_Int16 :
					  rt_memcpy( uint16_temp.data , &SDBuffer[location + 2 + strlen(str_ptr)] ,2);
						*( int16_t*)sddata->value = uint16_temp.value ;
					break;
				case Type_Uint16 :
					  rt_memcpy(uuint16_temp.data , &SDBuffer[location  + 2 + strlen(str_ptr)] , 2);
						*( uint16_t*)sddata->value = uuint16_temp.value ;
					break;
				case Type_Uint8 :
					  rt_memcpy( ( uint8_t *)sddata->value , &SDBuffer[location + 2 + strlen(str_ptr)] ,1);
					break;
				case Type_Int8 :
					  rt_memcpy( ( int8_t *)sddata->value , &SDBuffer[location  + 2 + strlen(str_ptr)] ,1);
					break;
				case Type_Gyro :
					
					break ; 
				case Type_Encoder :
					
					break ; 
				case Type_PID :
					
					break ; 
				case Type_Mecanum :
					
					break ;
				default :
					break;
			}										
		}
		f_close(fp); 
		return res;		
}


/*******************************************************************************
* Function Name  : ValueSave
* Description    : 
* Input          : @sddata ��Ҫ��������ݵĽṹ�� ��һ��ȫ�ֱ���
* Return         : none
*******************************************************************************/
int32_t ValueSave( struct _SDData * sddata )
{
		FIL *fp = &FileInit ;
		FRESULT res;
		int32_t location ;

		rt_memset( SDBuffer , 0 ,  SD_SAVE_LEN);
		
		res = f_open (fp, "Init.txt",  FA_READ | FA_WRITE);
		if(res != FR_OK){
			//show message
		}
		/* �Ȱ����е����ݶ��������洢��SDBuffer�� */
		f_read(fp , SDBuffer , SD_SAVE_LEN , &BW );
		/* ��λĳ�������ĳ�ʼλ�� */
		location =  located(SDBuffer ,sddata ) ;
		if( location != -1){
			/* ��ָ����ת����λ��λ�� */
			f_lseek(fp , (DWORD)location);
			/* �洢������ */
			Switch_Save( fp , sddata);
		}
		f_close(fp); 
		return res;
}


/*******************************************************************************
* Function Name  : InitAllFromSD
* Description    : ��ʼ�����е�����
* Input          : none
* Return         : none
*******************************************************************************/
rt_err_t InitAllFromSD(void)
{
	int32_t i ;		
	for(i = 0 ; i< (sizeof(SDSave) - sizeof(SDSave[0]) )/sizeof(SDSave[0]) + 1 ; i++ ){
		ValueInit(&SDSave[i]);
	}
    return 1;
}


/*******************************************************************************
* Function Name  : InitNewCard
* Description    : �����С����ɾ�˳�ʼ�����ļ�,���Ե�����������½�Init.txt�ļ�
* Input          : none
* Return         : none
*******************************************************************************/
void InitNewCard(void)
{
		FIL * fp = &FileInit ;
		int32_t  count ;
		FRESULT res;
		res = f_open (fp, "Init.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
		if(res != FR_OK){
				//show message
		}
		for( count = 0 ; count <  (sizeof(SDSave) - sizeof(SDSave[0]) )/sizeof(SDSave[0]) +1;count++){
			/* �½���ʱ���ÿ����������40���ֽڵĿռ� */
			f_lseek(fp , count*50);
			Switch_Save(fp , &SDSave[count]) ;
		}
		f_close(fp);
}


/*******************************************************************************
* Function Name  : FatFsInit
* Description    : �ļ�ϵͳ��ʼ��
* Input          : none
* Return         : none
*******************************************************************************/
void FatFsInit(void)
{
		FRESULT 	res;
		SD_Error 	_status ;
		_status 	= SD_Init();
		if(_status != SD_OK){
			//show messagesd : card init error !
			rt_kprintf("card init error !");
		}
		/* �����ļ�ϵͳ  */
		res = f_mount(&fs ,"", 0);		
		if(res != FR_OK){
			//show message : FS mount error !
			rt_kprintf("FS mount error !");
		}			
		/* ��databack�ļ� */
		res = f_open (&FileDataBack, "DataBack.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
		if(res != FR_OK){
			//show message 
			rt_kprintf("databack open error!");
		}		
}

/*******************************************************************************
* Function Name  : DataStore
* Description    : ���м����ݴ洢��SD����
* Input          : 
* Return         : none
*******************************************************************************/
FRESULT DataStore(char *fmt,...)
{
	uint32_t num , bw;
	FIL *fp = &FileDataBack;
	FRESULT res;
	char __SD_Printf_Buf[128];
	va_list ap;
	unsigned char *pStr = (unsigned char *)__SD_Printf_Buf;
	static int32_t count ;
	
	if( count < MAX_STORE ){
			count ++ ;
			va_start(ap,fmt);
			num = vsprintf(__SD_Printf_Buf,fmt,ap);	
			va_end(ap);
			res = f_write (fp, pStr, num, &bw);
			//f_sync(fp);
			//����������洢������������ļ��ر�
			if(count == MAX_STORE -1){
					f_close(fp);
			}
	}
	return res;
}


/**********************************end of file*************************************/

