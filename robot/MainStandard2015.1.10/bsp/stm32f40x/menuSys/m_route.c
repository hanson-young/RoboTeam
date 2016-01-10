/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "PointRoute.h"
#include "Keep.h"
#include "goroute.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

// uint8_t flag=1;

FIL error_angle_file;
FIL error_dis_file;
FIL speed1_file;
FIL speed2_file;
FIL VoutD_file;
FIL Total_file;
FIL GPS_file;




/* Private functions ---------------------------------------------------------*/
/*
 * 函数名: GoLineTest
 * 描  述: 走直线功能测试
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void GoLineTest()
{
	rt_uint32_t key_value;
	static struct Point end_point={1000,0};
	static double aim_angle;
	static double speed_sta=100;
	static double speed_max=800;//目前不能更改
	static double speed_end=0;
	static double err=20;
	extern double Acc_Rate;
	extern double Dec_Rate;


	
	static struct Input_ListValue Values[]=
	 {
		 {&Pid_List[2].p,	TYPE_FP64,	"AP"},
		 {&Pid_List[2].i,   TYPE_FP64,	"AI"},
		 {&Pid_List[2].d,	TYPE_FP64,	"AD"},
		 {&Pid_List[3].p,	TYPE_FP64,	"DP"},
		 {&Pid_List[3].i,	TYPE_FP64,	"DI"},
		 {&Pid_List[3].d,	TYPE_FP64,	"DD"},
		 {&end_point.x,		TYPE_FP64,	"X"},
		 {&end_point.y,		TYPE_FP64,	"Y"},
		 {&aim_angle,		TYPE_FP64,	"Aim_Angle"},
		 {&speed_sta,		TYPE_FP64,	"Speed_Sta"},
		 {&speed_max,		TYPE_FP64,	"Speed_Max"},
		 {&speed_end,		TYPE_FP64,	"Speed_End"},
		 {&Acc_Rate,		TYPE_FP64,	"Acc_Rate"},
		 {&Dec_Rate,		TYPE_FP64,	"Dec_Rate"},
		 {&err,				TYPE_FP64,	"err"},
	 };

	LCD_Clear();
	GPS_Clear();
	PID_Clear();
	 
	Input_List(Values,15);
	 
	 
	 
	f_open (&error_angle_file, "error_angle.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&error_dis_file, "error_dis.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&speed1_file, "speed1.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&speed2_file,"speed2.txt" , FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&VoutD_file, "VoutD.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&Total_file, "Total.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE); 	
	f_open (&GPS_file, "GPS.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE); 		 
	
	SetLine(end_point,aim_angle*pi/180,speed_sta,speed_max,speed_end);
	
	while(1)
	{
		
		GoLine();//Keep();
		LCD_SetXY(0,7);
		LCD_WriteString("now_distance:");
		LCD_WriteDouble(GetLength(GPS.position,end_point));//输出实时距离
		if((GetLength(GPS.position,end_point) < err)&&((GPS.radian-aim_angle*pi/180<0.02)&&(GPS.radian-aim_angle*pi/180>-0.02))||rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{SPEED_STOP;
			SetSpeed(0,0,0);//此句话去掉之后出错
			break;
		}  
		Delay_ms(5);
	}
	
	
	
	f_close(&error_angle_file);
	f_close(&error_dis_file);
	f_close(&speed1_file);
	f_close(&speed2_file);
	f_close(&VoutD_file);
	f_close(&Total_file);
	f_close(&GPS_file);
	
	

	if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_FOREVER) == RT_EOK);
	
}

/*
 * 函数名: KeepTest
 * 描  述: keep功能测试
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void KeepTest()
{
	rt_uint32_t key_value;
	
	static struct Point end_point={500,500};
	static double aim_angle=0;
  static double speed_max=500;
	struct Input_ListValue Values[]=
	 {
		 {&Pid_List[4].p,	TYPE_FP64,	"AP"},
		 {&Pid_List[4].i, TYPE_FP64,	"AI"},
		 {&Pid_List[4].d,	TYPE_FP64,	"AD"},
		 {&Pid_List[5].p,	TYPE_FP64,	"DP"},
		 {&Pid_List[5].i,	TYPE_FP64,	"DI"},
		 {&Pid_List[5].d,	TYPE_FP64,	"DD"},
		 {&end_point.x,		TYPE_FP64,	"X"},
		 {&end_point.y,		TYPE_FP64,	"Y"},
		 {&aim_angle,		TYPE_FP64,	"Aim_Angle"},
		 {&speed_max,		TYPE_FP64,	"Speed_Max"},
	 };
	
	GPS_Clear();
	LCD_Clear();
	PID_Clear();
	
	Input_List(Values,10);
	
	SetKeep(end_point,aim_angle*pi/180,speed_max);
	 
	 while(1)
	{
		
		Keep();
		LCD_SetXY(0,7);
		LCD_WriteString("now_distance:");
		LCD_WriteDouble(GetLength(GPS.position,end_point));//输出实时距离
		if((GetLength(GPS.position,end_point) < 50)&&((GPS.radian-aim_angle*pi/180<0.02)&&(GPS.radian-aim_angle*pi/180>-0.02))||rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{SPEED_STOP;
			SetSpeed(0,0,0);//此句话去掉之后出错
			break;
		}  
		Delay_ms(5);
	}
	
	if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_FOREVER) == RT_EOK);
	
}

/*
 * 函数名: GoCircleTest
 * 描  述: 跑圆功能测试
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void GoCircleTest(void)
{
	rt_uint32_t key_value;
	
	static struct Point center={500,0};
	static double r=500;
	static double speed=-300;

	struct Input_ListValue Values[]=
	 {
		 {&Pid_List[0].p,	TYPE_FP64,	"AP"},
		 {&Pid_List[0].i,   TYPE_FP64,	"AI"},
		 {&Pid_List[0].d,	TYPE_FP64,	"AD"},
		 {&Pid_List[1].p,	TYPE_FP64,	"DP"},
		 {&Pid_List[1].i,	TYPE_FP64,	"DI"},
		 {&Pid_List[1].d,	TYPE_FP64,	"DD"},
		 {&center.x,		TYPE_FP64,	"Center_X"},
		 {&center.y,		TYPE_FP64,	"Center_Y"},
		 {&r,				TYPE_FP64,	"R"},
		 {&speed,			TYPE_FP64,	"Speed"},
	 };
	
	GPS_Clear();
	LCD_Clear();
	PID_Clear();
	
	Input_List(Values,10);
	
	SetCircle(center,r,speed);
	
	while(1)
	{
		GoCircle();
// 		LCD_SetXY(0,7);
// 		LCD_WriteString("now_distance:");
// 		LCD_WriteDouble(GetLength(GPS.position,end_point));//输出实时距离
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
			break;
		Delay_ms(5);
	}
	
	if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_FOREVER) == RT_EOK);
	
}

/*
 * 函数名: GoLeanTest
 * 描  述: 斜线功能测试
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void GoLeanTest()
{
	rt_uint32_t key_value;
	static struct Point mid_point={1000,1000};
	static struct Point end_point={2000,0};
	static double aim_angle;
	static double speed_sta=100;
	static double speed_max=800;//目前不能更改
	static double speed_end=0;
	static double err=20;
	extern double Acc_Rate;
	extern double Dec_Rate;


	
	static struct Input_ListValue Values[]=
	 {
		 {&Pid_List[2].p,	TYPE_FP64,	"AP"},
		 {&Pid_List[2].i,   TYPE_FP64,	"AI"},
		 {&Pid_List[2].d,	TYPE_FP64,	"AD"},
		 {&Pid_List[3].p,	TYPE_FP64,	"DP"},
		 {&Pid_List[3].i,	TYPE_FP64,	"DI"},
		 {&Pid_List[3].d,	TYPE_FP64,	"DD"},
		 {&mid_point.x,		TYPE_FP64,	"MX"},
		 {&mid_point.y,		TYPE_FP64,	"MY"},
		 {&end_point.x,		TYPE_FP64,	"EX"},
		 {&end_point.y,		TYPE_FP64,	"EY"},
		 {&aim_angle,		TYPE_FP64,	"Aim_Angle"},
		 {&speed_sta,		TYPE_FP64,	"Speed_Sta"},
		 {&speed_max,		TYPE_FP64,	"Speed_Max"},
		 {&speed_end,		TYPE_FP64,	"Speed_End"},
		 {&Acc_Rate,		TYPE_FP64,	"Acc_Rate"},
		 {&Dec_Rate,		TYPE_FP64,	"Dec_Rate"},
		 {&err,				TYPE_FP64,	"err"},
	 };

	LCD_Clear();
	GPS_Clear();
	PID_Clear();
	 
	Input_List(Values,17);
	 
	f_open (&error_angle_file, "error_angle.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&error_dis_file, "error_dis.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&speed1_file, "speed1.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&speed2_file,"speed2.txt" , FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&VoutD_file, "VoutD.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&Total_file, "Total.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE); 	
	f_open (&GPS_file, "GPS.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE); 		 
	
	 //以下走第一段
	SetLine(mid_point,aim_angle*pi/180,speed_sta,speed_max,speed_end);
	
	while(1)
	{
		
		GoLine();
		LCD_SetXY(0,7);
		LCD_WriteString("now_distance1:");
		LCD_WriteDouble(GetLength(GPS.position,mid_point));//输出实时距离
		if((GetLength(GPS.position,mid_point) < err)&&((GPS.radian-aim_angle*pi/180<0.02)&&(GPS.radian-aim_angle*pi/180>-0.02))||rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{SPEED_STOP;
			SetSpeed(0,0,0);//此句话去掉之后出错
			break;
		}  
		Delay_ms(5);
	}
	
	//以下走第二段
   LCD_Clear();
	 SetLine(end_point,aim_angle*pi/180,speed_sta,speed_max,speed_end);
		while(1)
	{
		
		GoLine();
		LCD_SetXY(0,7);
		LCD_WriteString("now_distance2:");
		LCD_WriteDouble(GetLength(GPS.position,end_point));//输出实时距离
		if((GetLength(GPS.position,end_point) < err)&&((GPS.radian-aim_angle*pi/180<0.02)&&(GPS.radian-aim_angle*pi/180>-0.02))||rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{SPEED_STOP;
			SetSpeed(0,0,0);//此句话去掉之后出错
			break;
		}  
		Delay_ms(5);
	}
	
	
	
	f_close(&error_angle_file);
	f_close(&error_dis_file);
	f_close(&speed1_file);
	f_close(&speed2_file);
	f_close(&VoutD_file);
	f_close(&Total_file);
	f_close(&GPS_file);
	
	

	if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_FOREVER) == RT_EOK);
	
}
/*
 * 函数名: STAR
 * 描  述: 周六的演示
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void STAR()
{
	rt_uint32_t key_value;
	
	static struct Point point1={1000*3,0};
	static struct Point point2={190.983*3,-587.785*3};
	static struct Point point3={500*3,363.272*3};
	static struct Point point4={809.017*3,-587.785*3};
	static struct Point point5={0,0};
	static double aim_angle=0;
  static double speed_max=500;
	static double err=20;
	struct Input_ListValue Values[]=
	 {
		 {&Pid_List[4].p,	TYPE_FP64,	"AP"},
		 {&Pid_List[4].i, TYPE_FP64,	"AI"},
		 {&Pid_List[4].d,	TYPE_FP64,	"AD"},
		 {&Pid_List[5].p,	TYPE_FP64,	"DP"},
		 {&Pid_List[5].i,	TYPE_FP64,	"DI"},
		 {&Pid_List[5].d,	TYPE_FP64,	"DD"},
		 {&aim_angle,		TYPE_FP64,	"Aim_Angle"},
		 {&speed_max,		TYPE_FP64,	"Speed_Max"},
		 {&err,				TYPE_FP64,	"err"},
	 };
	
	GPS_Clear();
	LCD_Clear();
	PID_Clear();
	
	Input_List(Values,9);
	 
	f_open (&error_angle_file, "error_angle.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&error_dis_file, "error_dis.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&speed1_file, "speed1.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&speed2_file,"speed2.txt" , FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&VoutD_file, "VoutD.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&Total_file, "Total.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE); 	
	f_open (&GPS_file, "GPS.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	 
	SetKeep(point1,aim_angle*pi/180,speed_max);
	 while(1)
	{
		
		Keep();
		LCD_SetXY(0,7);
		LCD_WriteString("now_distance1:");
		LCD_WriteDouble(GetLength(GPS.position,point1));//输出实时距离
		if((GetLength(GPS.position,point1) < err)&&((GPS.radian-aim_angle*pi/180<0.02)&&(GPS.radian-aim_angle*pi/180>-0.02))||rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{SPEED_STOP;
			//SetSpeed(0,0,0);//此句话去掉之后出错
			break;
		}  
		Delay_ms(5);
	}
	
	
	
	aim_angle=36;
	LCD_Clear();
	SetKeep(point2,aim_angle*pi/180,speed_max);//aim_angle*pi/180
	while(1)
	{
		
		Keep();
		LCD_SetXY(0,7);
		LCD_WriteString("now_distance2:");
		LCD_WriteDouble(GetLength(GPS.position,point2));//输出实时距离
		if((GetLength(GPS.position,point2) < err)&&((GPS.radian-aim_angle*pi/180<0.02)&&(GPS.radian-aim_angle*pi/180>-0.02))||rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{SPEED_STOP;
			//SetSpeed(0,0,0);//此句话去掉之后出错
			break;
		}  
		Delay_ms(5);
	}
	
	aim_angle=72;
	LCD_Clear();
	SetKeep(point3,aim_angle*pi/180,speed_max);
	while(1)
	{
		
		Keep();
		LCD_SetXY(0,7);
		LCD_WriteString("now_distance3:");
		LCD_WriteDouble(GetLength(GPS.position,point3));//输出实时距离
		if((GetLength(GPS.position,point3) < err)&&((GPS.radian-aim_angle*pi/180<0.02)&&(GPS.radian-aim_angle*pi/180>-0.02))||rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{SPEED_STOP;
			//SetSpeed(0,0,0);//此句话去掉之后出错
			break;
		}  
		Delay_ms(5);
	}
	
	
	aim_angle=108;
	LCD_Clear();
	SetKeep(point4,aim_angle*pi/180,speed_max);
	while(1)
	{
		
		Keep();
		LCD_SetXY(0,7);
		LCD_WriteString("now_distance4:");
		LCD_WriteDouble(GetLength(GPS.position,point4));//输出实时距离
		if((GetLength(GPS.position,point4) < err)&&((GPS.radian-aim_angle*pi/180<0.02)&&(GPS.radian-aim_angle*pi/180>-0.02))||rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{SPEED_STOP;
			//SetSpeed(0,0,0);//此句话去掉之后出错
			break;
		}  
		Delay_ms(5);
	}
	
	
	aim_angle=144;
	LCD_Clear();
	SetKeep(point5,aim_angle*pi/180,speed_max);
	while(1)
	{
		
		Keep();
		LCD_SetXY(0,7);
		LCD_WriteString("now_distance5:");
		LCD_WriteDouble(GetLength(GPS.position,point5));//输出实时距离
		if((GetLength(GPS.position,point5) <err)&&((GPS.radian-aim_angle*pi/180<0.02)&&(GPS.radian-aim_angle*pi/180>-0.02))||rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{SPEED_STOP;
			SetSpeed(0,0,0);//此句话去掉之后出错
			break;
		}  
		Delay_ms(5);
	}
	
	aim_angle=0;
	LCD_Clear();
	SetKeep(point5,aim_angle*pi/180,speed_max);
	while(1)
	{
		
		Keep();
		LCD_SetXY(0,7);
		LCD_WriteString("now_distance5:");
		LCD_WriteDouble(GetLength(GPS.position,point5));//输出实时距离
		if((GetLength(GPS.position,point5) <err)&&((GPS.radian-aim_angle*pi/180<0.02)&&(GPS.radian-aim_angle*pi/180>-0.02))||rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{SPEED_STOP;
			SetSpeed(0,0,0);//此句话去掉之后出错
			break;
		}  
		Delay_ms(5);
	}
	
	f_close(&error_angle_file);
	f_close(&error_dis_file);
	f_close(&speed1_file);
	f_close(&speed2_file);
	f_close(&VoutD_file);
	f_close(&Total_file);
	f_close(&GPS_file);
	
	
	if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_FOREVER) == RT_EOK);
	
}
/********************************    END OF FILE    ***************************/

void RouteTest(void)
{
	rt_uint32_t key_value;
	
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========Goroute=========");
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
		LCD_WriteString("5.red_route");
		LCD_SetXY(0,7);
		LCD_WriteString("6.blue_route");
		
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
					Go_Route(0);
					break;
				case key6:
					Go_Route(2);
					break;
				case keyback:
					return;
			}
		}
		Delay_ms(20);
	}
}
