/* Includes ------------------------------------------------------------------*/
#include "includes.h"
double aim_angle=90*pi/180;//目标角度直角
struct PID keep_angle_pid=
{
	0,1000000,0

	
	
};

int64_t i;
void MY_Input_List(struct Input_ListValue *list,int num);
	double speed_max;
	int x_f,y_f,a_f;
	double error_angle,Vout_A;
	double zhuan;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

void pid_set_A(void)
{
	rt_uint32_t key_value;

    static struct Input_ListValue Values[]=
    {
        {&keep_angle_pid.p,	TYPE_FP64,	"angle.p"},
        {&keep_angle_pid.i,	TYPE_FP64,	"angle.i"},
				{&keep_angle_pid.d,	TYPE_FP64,	"angle.d"},
				{&aim_angle,TYPE_FP64,"aim_angle"},
    };

    LCD_Clear();
    GPS_Clear();
    PID_Clear();

    MY_Input_List(Values,4);

    if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_FOREVER) == RT_EOK);
}

void MY_Input_List(struct Input_ListValue *list,int num)
{
	rt_uint32_t key_value;
	int i,index;
	int allpage=(num-1)/3;
	int nowpage=0;
	fp64 out;

	while(1)
	{
		LCD_Clear();	
		for(i=0;i<4;i++)
		{
			index=nowpage*3+i;
			if(index<num)
			{
				LCD_SetXY(0,i);
				LCD_Printf("%d.",i+1);
				LCD_WriteString(list[index].name);
				LCD_WriteString(":");
				switch(list[index].type)
				{
					case TYPE_INT8:
						LCD_WriteNum(*(int8_t*)(list[index].value));
					break;
					case TYPE_UINT8:
						LCD_WriteNum(*(uint8_t*)(list[index].value));
					break;
					case TYPE_INT16:
						LCD_WriteNum(*(int16_t*)(list[index].value));
					break;
					case TYPE_UINT16:
						LCD_WriteNum(*(uint16_t*)(list[index].value));
					break;
					case TYPE_INT32:
						LCD_WriteNum(*(int32_t*)(list[index].value));
					break;
					case TYPE_UINT32:
						LCD_WriteNum(*(uint32_t*)(list[index].value));
					break;
					case TYPE_INT64:
						LCD_WriteNum(*(int64_t*)(list[index].value));
					break;
					case TYPE_UINT64:
						LCD_WriteNum(*(uint64_t*)(list[index].value));
					break;
					case TYPE_FP32:
					  LCD_WriteNum(*(fp32*)(list[index].value));
					break;
					case TYPE_FP64:
						LCD_WriteNum(*(fp64*)(list[index].value));
					break;
				}
			}
		  else
			{
				LCD_SetXY(0,i);
				LCD_WriteString("                    ");
			}
			LCD_SetXY(0,4);
			LCD_Printf("5.go 6.return %d/%d ",nowpage+1,allpage+1);
		}
		
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{
			switch(key_value)
			{
				case pageup:
					if(nowpage>0)
						nowpage--;
					else
						nowpage=allpage;
				break;
				case pagedown:
					if(nowpage<allpage)
						nowpage++;
					else
						nowpage=0;
				break;
				case 1:
				case 2:
				case 3:
					index=key_value+nowpage*3-1;
					switch(list[index].type)
					{
						case TYPE_INT8:
							out=(*(int8_t*)(list[index].value));
						break;
						case TYPE_UINT8:
							out=(*(uint8_t*)(list[index].value));
						break;
						case TYPE_INT16:
							out=(*(int16_t*)(list[index].value));
						break;
						case TYPE_UINT16:
							out=(*(uint16_t*)(list[index].value));
						break;
						case TYPE_INT32:
							out=(*(int32_t*)(list[index].value));
						break;
						case TYPE_UINT32:
							out=(*(uint32_t*)(list[index].value));
						break;
						case TYPE_INT64:
							out=(*(int64_t*)(list[index].value));
						break;
						case TYPE_UINT64:
							out=(*(uint64_t*)(list[index].value));
						break;
						case TYPE_FP32:
							out=(*(fp32*)(list[index].value));
						break;
						case TYPE_FP64:
							out=(*(fp64*)(list[index].value));
						break;
					}
					Input_DoubleValue(&out,list[index].name);
					switch(list[index].type)
					{
						case TYPE_INT8:
							(*(int8_t*)(list[index].value))=out;
						break;
						case TYPE_UINT8:
							(*(uint8_t*)(list[index].value))=out;
						break;
						case TYPE_INT16:
							(*(int16_t*)(list[index].value))=out;
						break;
						case TYPE_UINT16:
							(*(uint16_t*)(list[index].value))=out;
						break;
						case TYPE_INT32:
							(*(int32_t*)(list[index].value))=out;
						break;
						case TYPE_UINT32:
							(*(uint32_t*)(list[index].value))=out;
						break;
						case TYPE_INT64:
							(*(int64_t*)(list[index].value))=out;
						break;
						case TYPE_UINT64:
							(*(uint64_t*)(list[index].value))=out;
						break;
						case TYPE_FP32:
							(*(fp32*)(list[index].value))=out;
						break;
						case TYPE_FP64:
							(*(fp64*)(list[index].value))=out;
						break;
					}
				break;
				case 4:
					Input_DoubleValue(&aim_angle,list[3].name);
				break;
				case key5:
					while(1)
					{
						error_angle = aim_angle-GPS.radian;
	
						if(error_angle>pi)
							error_angle-=2*pi;
						if(error_angle<-pi)
							error_angle+=2*pi;
	
						Vout_A = 1000*AnglePID(error_angle,keep_angle_pid);			//角度纠正值
						zhuan=Vout_A;    //赋给zhuan

						//LCD_Clear();
						LCD_SetXY(0,7);
						LCD_WriteString("error_angle:");
						LCD_WriteDouble(error_angle);//输出角度

						if(zhuan>200)
							zhuan= 200;		
						if(zhuan<-200)
							zhuan= -200;

						SetSpeed(0,0,zhuan);
						
						Delay_ms(5);
						if(fabs(error_angle)<=4*pi/180)
							break;
					}	
					break;					
		
				case key6:
					return;
			}

// 			if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
// 		{
// 			switch(key_value)
// 			{
// 					case key6:
// 							goto label;
// 			}
		}
		Delay_ms(5);
	}
}



/* Private functions ---------------------------------------------------------*/
/*
 * 函数名: sys_Gyro
 * 描  述: 陀螺仪相关功能
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void sys_Gyro()
{
	rt_uint32_t key_value;
	
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========Gyro========");
		LCD_SetXY(0,1);
		LCD_WriteString("angle:");
		LCD_WriteDouble(Gyro.angle);
		LCD_SetXY(0,2);
		LCD_WriteString("Con1:");
		LCD_WriteDouble(Gyro.convert1);
		LCD_SetXY(0,3);
		LCD_WriteString("Con2:");
		LCD_WriteDouble(Gyro.convert2);
		LCD_SetXY(0,4);
		LCD_WriteString("1.Init 2.Set 3.Float");  
		
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{
			switch(key_value)
			{
				case key1:
					Gyro_Init();
					break;
				case key2:
					Gyro_Set();
					break;
				case key3:
					Gyro_CheckFloat();
					break;
				case keyback:
					return;
			}
		}
		Delay_ms(10);
	}
}

/*
 * 函数名: sys_Encoder
 * 描  述: 码盘相关功能
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
int x,y;

void sys_Encoder()
{
	rt_uint32_t key_value;
	
	while(1)
	{
		x=GPS.position.x;
		y=GPS.position.y;
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======Encoder=======");
		LCD_SetXY(0,1);
		LCD_WriteString("Dis1:");
		LCD_WriteDouble(Encoder[0].dis);
		LCD_SetXY(0,2);
		LCD_WriteString("Dis2:");
		LCD_WriteDouble(Encoder[1].dis);
		LCD_SetXY(0,3);
		LCD_WriteString("1.Init_Con 2.Init_R");
		LCD_SetXY(0,4);
		LCD_WriteString("3.View 4.Set");
		LCD_SetXY(0,5);
		LCD_WriteString("X:");
 		LCD_WriteDouble(x);
		LCD_WriteInt(x);//写整数部分
		LCD_SetXY(0,6);
		LCD_WriteString("Y:");
 		LCD_WriteDouble(y);
		LCD_WriteInt(y);//写整数部分
		
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{
			switch(key_value)
			{
				case key1:
					Encoder_Init_Con();
					break;
				case key2:
					Encoder_Init_R();
					break;
				case key3:
					Encoder_View();
					break;
				case key4:
					Encoder_Set();
					break;
				case keyback:
					return;
			}
		}
		Delay_ms(10);
	}
}

/*
 * 函数名: sys_GPS
 * 描  述: GPS显示与修改
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */

double StartAngle;
void sys_GPS()
{
	rt_uint32_t key_value;
	
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========GPS=========");
		LCD_SetXY(0,1);
		LCD_WriteString("Angle:");
		LCD_WriteDouble(GPS.radian*180/pi);
		LCD_SetXY(0,2);
		LCD_WriteString("X:");
		LCD_WriteDouble(GPS.position.x);
		LCD_SetXY(0,3);
		LCD_WriteString("Y:");
		LCD_WriteDouble(GPS.position.y);
		LCD_SetXY(0,4);
		LCD_WriteString("1.SetA 2.SetX 3.SetY");
		LCD_SetXY(0,5);
		LCD_WriteString("4.Clear");
		LCD_SetXY(0,6);
		LCD_WriteString("5.Record 6.Set");
		LCD_SetXY(0,7);
		LCD_Printf("StartA:%f",StartAngle);
		
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{
			switch(key_value)
			{
				case key1:
					Input_DoubleValue(&MOSI_GPS_ANG.f64_data,"angle");
					MOSI_GPS_ANG.f64_data=MOSI_GPS_ANG.f64_data*pi/180;
					Write_Database(W_GPS_ANG_ID);
					break;
				case key2:
					Input_DoubleValue(&MOSI_GPS_X.f64_data,"X");
					Write_Database(W_GPS_X_ID);
					break;
				case key3:
					Input_DoubleValue(&MOSI_GPS_Y.f64_data,"Y");
					Write_Database(W_GPS_Y_ID);
					break;
				case key4:
					GPS_Clear();
					break;
				case key5:
					StartAngle = GPS.radian*180/pi;
					break;
				case key6:
					MOSI_GPS_ANG.f64_data=StartAngle*pi/180;
					Write_Database(W_GPS_ANG_ID);
					break;
				case keyback:
					return;
			}
		}
		Delay_ms(20);
	}
}

/*
 * 函数名: sys_Mecanum
 * 描  述: 全向轮相关功能
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void sys_Mecanum()
{
	rt_uint32_t key_value;
	
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======Mecanum=======");
		LCD_SetXY(0,1);
		LCD_WriteString("1.FL port:");
		LCD_WriteInt(Mec_FL.port);
		LCD_SetXY(0,2);
		LCD_WriteString("2.FR port:");
		LCD_WriteInt(Mec_FR.port);
		LCD_SetXY(0,3);
		LCD_WriteString("3.BL port:");
		LCD_WriteInt(Mec_BL.port);
		LCD_SetXY(0,4);
		LCD_WriteString("4.BR port:");
		LCD_WriteInt(Mec_BR.port);
		
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{
			switch(key_value)
			{
				case key1:
					Mecanum_test(&Mec_FL);
					break;
				case key2:
					Mecanum_test(&Mec_FR);
					break;
				case key3:
					Mecanum_test(&Mec_BL);
					break;
				case key4:
					Mecanum_test(&Mec_BR);
					break;
				case keyback:
					return;
			}
		}
		Delay_ms(10);
	}
}

/*
 * 函数名: sys_SA_ST
 * 描  述: 接近开关、接触开关
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void sys_SA_ST()
{
	rt_uint32_t key_value;
	uint8_t st_num=8;
	uint8_t sa_num=5;
	int i,j;
	
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("=======SA-ST========");
		
		for(i=1;i<=st_num;i++)
		{
			LCD_SetXY(((i-1)%3)*7,(i-1)/3+1);
// 			LCD_Printf("ST%d:%d",i,ST_READ(i) );
		}
		
		j=(i-1)/3+2;
		
		for(i=1;i<=sa_num;i++)
		{
			LCD_SetXY(((i-1)%3)*7,j+(i-1)/3+1);
// 			LCD_Printf("SA%d:%d",i,SA_READ(i) );
		}
		
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{
			switch(key_value)
			{
				case keyback:
					return;
			}
		}
		Delay_ms(10);
	}
}

/*
 * 函数名: sys_CAN
 * 描  述: CAN总线
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void sys_CAN()
{
	rt_uint32_t key_value;
	
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========CAN=========");
		LCD_SetXY(0,1);
		LCD_WriteString("1.Relay");
		LCD_SetXY(0,2);
		LCD_WriteString("2.Motor");
		
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{
			switch(key_value)
			{
				case key1:
					Relay_View();
					break;
				case key2:
					Motor_View();
					break;
				case keyback:
					return;
			}
		}
		Delay_ms(10);
	}
}

/*
 * 函数名: sys_Handle
 * 描  述: 手柄数据显示
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void sys_Handle()
{
	rt_uint32_t key_value;
	
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("=======Handle=======");
		LCD_SetXY(0,1);
		LCD_WriteString("UD:");
		LCD_WriteInt(handle_speed_x);
		LCD_WriteString("LR:");
		LCD_WriteInt(handle_speed_y);
		LCD_SetXY(6,2);
		LCD_WriteString("RT:");
		LCD_WriteInt(handle_speed_turn);
		LCD_SetXY(0,3);
		LCD_WriteInt(*handle_count_y);
		LCD_WriteString("  ");
		LCD_WriteInt(*handle_count);
		LCD_SetXY(0,4);		
		LCD_Printf("X Key:");
		if(Button1_On)
			LCD_Printf("%d",1);
		if(Button2_On)
			LCD_Printf("%d",2);
		if(Button3_On)
			LCD_Printf("%d",3);
		if(Button4_On)
			LCD_Printf("%d",4);
		if(Button5_Up)
			LCD_Printf("%d",5);
		if(Button5_Down)
			LCD_Printf("%d",-5);		
		if(Button6_Up)
			LCD_Printf("%d",6);
		if(Button6_Down)
			LCD_Printf("%d",-6);
		if(Button7_On)
			LCD_Printf("%d",7);
		if(Button8_On)
		{
			LCD_Printf(" ");
			LCD_Printf("%d",8);
		}
		if(Button9_On)
			LCD_Printf("%d",9);
		if(Button10_On)
		{
			LCD_Printf(" ");
			LCD_Printf("%d",10);
		}
		LCD_SetXY(0,5);
		LCD_Printf("Y Key:");
		if(Y_Button1_On)
			LCD_Printf("%d",1);
		if(Y_Button2_On)
			LCD_Printf("%d",2);
		if(Y_Button3_On)
			LCD_Printf("%d",3);
		if(Y_Button4_On)
			LCD_Printf("%d",4);
		if(Y_Button5_Up)
			LCD_Printf("%d",5);
		if(Y_Button5_Down)
			LCD_Printf("%d",-5);
		if(Y_Button6_Up)
			LCD_Printf("%d",6);
		if(Y_Button6_Down)
			LCD_Printf("%d",-6);
		if(Y_Button7_On)
			LCD_Printf("%d",7);
		if(Y_Button8_On)
		{
			LCD_Printf(" ");
			LCD_Printf("%d",8);
		}
		if(Y_Button9_On)
			LCD_Printf("%d",9);
		if(Y_Button10_On)
		{
			LCD_Printf(" ");
			LCD_Printf("%d",10);
		}
		LCD_SetXY(0,6);
		LCD_WriteString("6.t_f:");
		LCD_WriteInt(time_flag);
		LCD_SetXY(0,7);
		LCD_WriteString("7.t_fy");
		LCD_WriteInt(time_flag_y);
		Delay_ms(10);
	}
}

/*
 * 函数名: sys_Fan
 * 描  述: 控制涵道的菜单
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
 
#include "fan.h"
void sys_keep(void)
{
	rt_uint32_t key_value;
	struct Point keep_test_point;
    static double delta = 1.0;
	Fan_Set_Min();
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========keep_test=========");
		LCD_SetXY(0,1);
		LCD_Printf("1:angle_only");
		LCD_SetXY(0,2);
		LCD_Printf("3:posit_only");

		keep_test_point.x=0;
		keep_test_point.y=0;
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{
			switch(key_value)
			{
				case key1:
				{
					SetKeep(keep_test_point,20*3.1415926/180,300);
					Keep();
				}
					break;
				case key2:
				{
					keep_test_point.x=1000;
					keep_test_point.y=1000;
					SetKeep(keep_test_point,0,300);
					Keep();
				}
					break;
				default:
					break;
			}
		}
		Delay_ms(10);
	}
}

/********************************    END OF FILE    ***************************/
