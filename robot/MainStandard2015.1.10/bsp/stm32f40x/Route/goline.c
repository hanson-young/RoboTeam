
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "math.h"

struct Point Sta_Point;
struct Point End_Point;
double Aim_Radian;
double Speed_Sta;
double Speed_Max;
double Line_Angle;
double S;
double Acc_Dis,Dec_Dis;
double Acc,Dec;
double Acc_Rate=0.3;
double Dec_Rate=0.3;



extern FIL error_angle_file;
extern FIL error_dis_file;
extern FIL speed1_file;
extern FIL speed2_file;
extern FIL VoutD_file;
extern FIL Total_file;
extern FIL GPS_file;

/*
 * 函数名: SetLine
 * 描  述: 初始化走直线功能
 * 输  入: -end_point  目标位置
 *		   -aim_radian 目标角度
 *		   -speed_sta  初始速度
 *		   -speed_max  最大速度
 *		   -speed_end  结束速度
 * 输  出: 无
 * 调  用: 外部调用
 */
void SetLine(struct Point end_point,double aim_radian,double speed_sta,double speed_max,double speed_end)
{
	Sta_Point = GPS.position;		//记录初始位置
	End_Point = end_point;
	Aim_Radian = aim_radian;
	
	Speed_Sta = speed_sta;
	Speed_Max = speed_max;
	
	Line_Angle = GetLineAngle(Sta_Point,end_point);	//求出该路径线段的倾角
	
	S = GetLength(Sta_Point,end_point);				//求出该路径线段的长度
	
	Acc_Dis = Acc_Rate*S;//加速距离和减速距离
	Dec_Dis = Dec_Rate*S;
	
	Acc = (speed_max - speed_sta)/Acc_Dis;//加速度与减速度（与距离相关）
	Dec = (speed_end - speed_max)/Dec_Dis;
	
	PID_Clear();
}

void GoLine(void)
{
	struct Point pos;							//理论坐标

	double k;
	float speed_x;						//世界坐标里的速度与分速度
	float speed=0;
	float speed_y;
	float speed_robot_x;
	double phi;
	float speed_robot_y;			//转换到机器人坐标的分速度
	
	double error_angle,error_dis;				//角度误差与距离误差
	double Vout_A=0;							//PID调整后角度输出值
	double Vout_D=0;							//PID调整后距离输出值
	double v_err_x,v_err_y;						//PID调整速度的分量
	
	pos = GetFoot_P2L_PP(GPS.position,Sta_Point,End_Point);	//得到理论坐标
	
	//根据理论坐标赋速度
	if(GetLength(Sta_Point,pos) < Acc_Dis)
	{
		if(speed < Speed_Max)
			speed = Speed_Sta+  Acc*GetLength(Sta_Point,pos); //speed_sta
		else
			speed = Speed_Max;
	}
	else 
		if(GetLength(Sta_Point,pos) > (S-Dec_Dis))
	{
		speed = Speed_Max + Dec*(GetLength(Sta_Point,pos)-(S-Dec_Dis));
	}
		else
		speed = Speed_Max;
	
	//求速度在x,y方向上的分量
	speed_x = speed*cos(Line_Angle);
	speed_y = speed*sin(Line_Angle);
	
	//角度误差和距离误差
	error_angle = Aim_Radian-GPS.radian;
	

	error_dis = ( GPS.position.x - Sta_Point.x ) * sin( Line_Angle ) -( GPS.position.y - Sta_Point.y ) * cos( Line_Angle );
		//error_dis为垂直路径方向上偏离路径的距离，无误！
	
	if(error_angle>pi)
		error_angle-=2*pi;
	if(error_angle<-pi)
		error_angle+=2*pi;
	
	//PID调整
	//if(GetLength(GPS.position,Sta_Point) >= Acc_Dis)
	Vout_A = 1000*AnglePID(error_angle,Pid_List[2]);
	Vout_D = DistancePID(error_dis,Pid_List[3]);
	
	//PID调整速度的分量
	v_err_x = -Vout_D*sin(Line_Angle);
	v_err_y =  Vout_D*cos(Line_Angle);
	
	//加上调整增量
	speed_x += v_err_x;
	speed_y += v_err_y;
	
	k=sqrt(speed_x*speed_x+speed_y*speed_y);
	
		if(speed_x>Speed_Max||speed_y>Speed_Max)//这四行是我加的
		{speed_x=Speed_Max*speed_x/k;
	
		speed_y=Speed_Max*speed_y/k;}
	
	
	if(speed_x<-Speed_Max||speed_y<-Speed_Max)//这四行是我加的
	{speed_x=-Speed_Max*speed_x/k;

		speed_y=-Speed_Max*speed_y/k;}
		
		phi = GPS.radian;
	
		speed_robot_x =  speed_x*cos(phi) + speed_y*sin(phi);
		speed_robot_y = -speed_x*sin(phi) + speed_y*cos(phi);
		//将世界坐标的速度转换为机器人局部坐标的速度
	
	
	//为四轮赋速度
	Speed_X = speed_robot_x;
	Speed_Y = speed_robot_y;
	Speed_Rotation = Vout_A;
		if(Speed_Rotation>=100)
			Speed_Rotation=100;
		if(Speed_Rotation<=-100)
			Speed_Rotation=-100;
		
		
	
// 	if(Speed_X>Speed_Max)//这四行是我加的
// 		Speed_X=Speed_Max;
// 	if(Speed_Y>Speed_Max)
// 		Speed_Y=Speed_Max;
		
		
	  //fprintf(&error_angle_file,"%-12.4f\r\n",120.00);
		SD_Printf_MQ(&error_angle_file,"%-12.4f\r\n",error_angle);
		SD_Printf_MQ(&error_dis_file,"%-12.4f\r\n",error_dis);
		SD_Printf_MQ(&speed1_file,"%-12.4f	%-12.4f\r\n",speed_x,speed_y);
		SD_Printf_MQ(&speed2_file,"%-12.4f	%-12.4f\r\n",Speed_X,Speed_Y);
		SD_Printf_MQ(&VoutD_file,"%-12.4f\r\n",Vout_D);
	  SD_Printf_MQ(&GPS_file,"%-12.4f	%-12.4f\r\n",GPS.position.x,GPS.position.y);
		SD_Printf_MQ(&Total_file,"%-12.4f	%-12.4f	%-12.4f	%-12.4f	%-12.4f	%-12.4f\r\n",error_angle,error_dis,Vout_A,Vout_D,speed_x,speed_y);
	  
	
	  fflush(&error_angle_file);
	  fflush(&error_dis_file);
	  fflush(&speed1_file);
	  fflush(&speed2_file);
	  fflush(&VoutD_file);
	  fflush(&Total_file);

	SetSpeed(Speed_X,Speed_Y,Speed_Rotation);
	//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！LCD显示！！！！！！！！！！！！！！！！！！
	//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！LCD显示！！！！！！！！！！！！！！！！！！
	//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！LCD显示！！！！！！！！！！！！！！！！！！
	//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！LCD显示！！！！！！！！！！！！！！！！！！
	//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！LCD显示！！！！！！！！！！！！！！！！！！
	//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！LCD显示！！！！！！！！！！！！！！！！！！
	//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！LCD显示！！！！！！！！！！！！！！！！！！
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("A:");
	LCD_WriteDouble(GPS.radian);
	
	LCD_SetXY(0,1);
	LCD_WriteString("X:");
	LCD_WriteDouble(GPS.position.x);
	
	LCD_SetXY(9,1);
	LCD_WriteString(" Y:");
	LCD_WriteDouble(GPS.position.y);
	
	LCD_SetXY(0,2);
	LCD_WriteString("error_angle:");
	LCD_WriteDouble(error_angle);
	
	LCD_SetXY(0,3);
	LCD_WriteString("error_dis:");
	LCD_WriteDouble(error_dis);
	
	LCD_SetXY(0,4);
	LCD_WriteString("Vout_A:");//Vout_A
	LCD_WriteDouble(Vout_A);
	
	LCD_SetXY(0,5);
	LCD_WriteString("Vout_D:");//Vout_D
	LCD_WriteDouble(Vout_D);
	
	LCD_SetXY(0,6);
	LCD_WriteDouble(speed_x);
	LCD_SetXY(10,6);
	LCD_WriteDouble(speed_y);
	
	LCD_SetXY(0,7);
	LCD_WriteDouble(Line_Angle);
}


/**********************************END OF FILE*********************************/
