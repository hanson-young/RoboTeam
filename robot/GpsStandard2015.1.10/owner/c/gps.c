#include "all.h"
//Gps_State结构体个数

Gps_State Gps_List[GPS_STATCOUNT];
Pointfp64 Gps_Zero={0,0};
Pointfp64 Gps_assume;//利用摄像头或者激光等校正时试验用的变量

	fp64 localx;
	fp64 localy;
	fp64 radian;

void GPS_Update(void)
{
	fp64 radianstep = 0.0;						//弧度步进值

    	
	Gps_List[1]=Gps_List[0];
	
	Gps_List[0].angle=Gyro_Angle_Total;//角度范围为-180~180

	//转换为弧度值并限制在-PI~PI
	Gps_List[0].radian=Gps_List[0].angle*PI/180.0;

	radian=Gps_List[0].radian;
	
	//获得弧度步进值
	radianstep=Gps_List[0].radian-Gps_List[1].radian;
	if(radianstep>PI)
		radianstep-=2*PI;
	if(radianstep<-PI)
		radianstep+=2*PI;
	
 	Gps_List[0].distance1 = Encoders[0].Distance;
 	Gps_List[0].distance2 = Encoders[1].Distance;
	
 	Gps_List[0].distancestep1 = Gps_List[0].distance1 - Gps_List[1].distance1 - Encoders[0].Radius*radianstep;
 	Gps_List[0].distancestep2 = Gps_List[0].distance2 - Gps_List[1].distance2 - Encoders[1].Radius*radianstep;

	localx = (Gps_List[0].distancestep1*cos(Encoders[1].radian)-Gps_List[0].distancestep2*cos(Encoders[0].radian))/sin(Encoders[0].radian-Encoders[1].radian);
	localy = (Gps_List[0].distancestep1*sin(Encoders[1].radian)-Gps_List[0].distancestep2*sin(Encoders[0].radian))/sin(Encoders[1].radian-Encoders[0].radian);
	
	Gps_List[0].position.x+=localx*cos(radian)-localy*sin(radian) ;
	Gps_List[0].position.y+=localx*sin(radian)+localy*cos(radian) ;

	
}
void GPS_Clear(void)
{
	Gyro_Total=0;
// 	Gyro_Last=Gyro_Now;
	Encoder_Clear(0);
	Encoder_Clear(1);
	GPS_Init(Gps_Zero,0);
}
void GPS_Init(Pointfp64 position,fp64 radian)
{
	int i;
	Encoder_Clear(0);
	Encoder_Clear(1);
	for(i=0;i<GPS_STATCOUNT;i++)
	{
		Gps_List[i].distance1=0.0;
		Gps_List[i].distance2=0.0;
		Gps_List[i].distancestep1=0.0;
		Gps_List[i].distancestep2=0.0;
		Gps_List[i].position=position;
		Gps_List[i].angle=radian/PI*180.0;
		Gps_List[i].radian=radian;
		Gps_List[i].speed=0.0;
		Gps_List[i].acceleration=0.0;
		Gyro_Total=Gps_List[i].angle/0.0000001/Gyro_Convert1;
		Gyro_Now.count=0;
	}
}
