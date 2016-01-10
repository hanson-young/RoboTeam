
/* Includes ------------------------------------------------------------------*/
#include "includes.h"

struct GPS_State GPS;

/*
 * ������: GPS_Init
 * ��  ��: GPS��ʼ��
 * ��  ��: ��
 * ��  ��: ��
 * ��  ��: �ⲿ����
 */
void GPS_Init(struct Point position,double radian)
{
	MOSI_GPS_X.f64_data = position.x;
	Write_Database(W_GPS_X_ID);

	MOSI_GPS_Y.f64_data = position.y;
	Write_Database(W_GPS_Y_ID);

	MOSI_GPS_ANG.f64_data = radian;
	Write_Database(W_GPS_ANG_ID);
}

/*
 * ������: GPS_Clear
 * ��  ��: GPS����
 * ��  ��: ��
 * ��  ��: ��
 * ��  ��: �ⲿ����
 */
void GPS_Clear(void)
{
	struct Point position={0,0};
	GPS_Init(position,0);
}




/**********************************END OF FILE*********************************/
