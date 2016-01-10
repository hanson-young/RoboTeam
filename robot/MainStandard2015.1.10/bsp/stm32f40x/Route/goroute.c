/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "PointRoute.h"
#include "math.h"

u8 Route_Num=1;
int Route[10]={0,3891,3994,7988,3091,3189,3994,4641,4704,5400};

u8 RouteFinish;//�������㼴��ʹ·��ֹͣ

extern struct Path_Point PointRoute[];

/*
 * ������: goroute1
 * ��  ��: ��·��
 * ��  ��: ��
 * ��  ��: ��
 * ��  ��: �ⲿ����
 */

u8 Go_Route(u8 route_num)
{
	u8 total_route;
	total_route = sizeof(Route)/2;
	if(total_route < route_num)
		return 0;
	SetPointPath(Route[route_num],Route[route_num+1]);
	while(1)
	{
		GoPointPath();
		if(Point_NowNum>Point_EndNum-2)
		{
			RouteFinish=0;
			SPEED_STOP;
			SetSpeed(0,0,0);
			break;
		}
		Delay_ms(5);
	}
	return 1;
}



