/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "math.h"

struct Point Aim_Keep_Point;
double Aim_Keep_Radian;
double Speed_Keep_Max;


extern FIL error_angle_file;
extern FIL error_dis_file;
extern FIL speed1_file;
extern FIL speed2_file;
extern FIL VoutD_file;
extern FIL Total_file;
extern FIL GPS_file;


/*
 * ������: SetKeep
 * ��  ��: ���ٵ���ָ��λ����Ƕ�
 * ��  ��: -end_point  Ŀ��λ��
 *		   -aim_radian Ŀ��Ƕ�
 * ��  ��: ��
 * ��  ��: �ⲿ����
 */
void SetKeep(struct Point end_point,double aim_radian,double speed_max)
{
	Aim_Keep_Point = end_point;
	Aim_Keep_Radian = aim_radian;
	Speed_Keep_Max = speed_max;
}

void Keep(void)
{
		double lineangle;
	
		double speed = 0;
		double speed_x,speed_y;						//������������ٶ�����ٶ�
		double speed_robot_x,speed_robot_y;			//ת��������������ķ��ٶ�
		double phi;
	
		double error_angle,error_dis;				//�Ƕ������������
		double Vout_A = 0;							//PID������Ƕ����ֵ
		double Vout_D = 0;							//PID������������ֵ
		lineangle = GetLineAngle(GPS.position,Aim_Keep_Point);	//�����·���߶ε����
		
		//��Ƕ������������
		error_angle = Aim_Keep_Radian-GPS.radian;
		error_dis = GetLength(GPS.position,Aim_Keep_Point);
		
		if(error_angle>pi)
			error_angle-=2*pi;
		if(error_angle<-pi)
			error_angle+=2*pi;
		
		//PID����
		Vout_A = 1000*AnglePID(error_angle,Pid_List[4]);
		Vout_D = DistancePID(error_dis,Pid_List[5]);
		
		//�����ת��
		if(Vout_A > 100) Vout_A = 100;
		else if(Vout_A < -100) Vout_A = -100;
		
		speed = Vout_D;
		
		//�������
		if(speed > Speed_Keep_Max) speed = Speed_Keep_Max;
		else if(speed < -Speed_Keep_Max) speed = -Speed_Keep_Max;
		
		//���ٶ���x,y�����ϵķ���
		speed_x = speed*cos(lineangle);
		speed_y = speed*sin(lineangle);
		
		//������������ٶ�ת��Ϊ�����˾ֲ�������ٶ�
		phi = GPS.radian;
		speed_robot_x =  speed_x*cos(phi) + speed_y*sin(phi);
		speed_robot_y = -speed_x*sin(phi) + speed_y*cos(phi);
		
		//Ϊ���ָ��ٶ�
		
		Speed_X = speed_robot_x;
		Speed_Y = speed_robot_y;
		Speed_Rotation = Vout_A;
		
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
		
		
		SetSpeed(Speed_X,Speed_Y,Speed_Rotation);//ԭ��û�����
		
		//LCD��ʾ
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
		LCD_WriteString("Vout_A:");
		LCD_WriteDouble(Vout_A);
		
		LCD_SetXY(0,5);
		LCD_WriteString("Vout_D:");
		LCD_WriteDouble(Vout_D);
		
		LCD_SetXY(0,6);
		LCD_WriteDouble(speed_x);
		LCD_SetXY(10,6);
		LCD_WriteDouble(speed_y);
		
		LCD_SetXY(0,7);
		LCD_WriteDouble(lineangle);	

}
/**********************************END OF FILE*********************************/
