
#include "includes.h"
#include "math.h"

#define SPEED_DEAD_ZONE 600
#define ROTA_DEAD_ZONE 100


struct GPS_State OP_List[] = 
{
	{{-1303,2973},(-28.60463*pi/180)},
	{{228,5355},(-28.60463*pi/180)},
	{{0,0},0},
	{{0,0},0},
};
// double aim_angle=90*pi/180;//目标角度直角
// struct PID keep_angle_pid=
// {
// 	0,1000000,0

// 	
// 	
// };



// 	double speed_max;
// 	int x_f,y_f,a_f;
// 	double error_angle,Vout_A;
// 	double zhuan;




ALIGN(RT_ALIGN_SIZE)
static char thread_handle_stack[1024];
struct rt_thread thread_handle;
//void MY_Input_List(struct Input_ListValue *list,int num);

typedef enum Status
{
	FALSE = 0,
	TRUE
}e_Status;

double handle_speed_y;
double handle_speed_x;
double handle_speed_turn;

u8 handleoff_flag_y = 1;
u8 handleoff_flag = 1;
u32 time_flag_y = 1;
u32 time_flag = 1;

u8 handle_timeout_flag_y = 1;
u8 handle_timeout_flag = 1;

int Handle_Off_Count_y=0;
int Handle_Off_Count=0;

u8 fan_flag;

static void rt_thread_entry_handle(void* parameter)
{
	double Handle_Speed_X;
	double Handle_Speed_Y;
	double Handle_Speed_Rotation;
	
	
	
// 	double speed_max;
// 	int x_f,y_f,a_f;
// 	double error_angle,Vout_A;
// 	double zhuan;
	

	//Speed_Data handle_speed_mes;
	u32 temp = 0;
	u32 temp_y = 0;


	TIM2->CNT = 0;
	
	while(1)
	{ 
		temp_y = *handle_count_y;
		while(*handle_count_y == temp_y)
		{
			time_flag_y++;
			Delay_ms(5);
					
			if(time_flag_y > 60)
			{
				time_flag_y = 0;
				if(handle_timeout_flag_y)
				{	
				}
				break;
			}
		}  

		if(time_flag_y)
		{
				time_flag_y = 0;
				handle_timeout_flag_y = 1;

				Handle_Speed_Y = HandleData_Y.ud-12288;
				Handle_Speed_X = HandleData_Y.lr-12288;
				Handle_Speed_Rotation = (HandleData_Y.turn-12288)/10.0;
			
				if (Abs(Handle_Speed_Y) <= SPEED_DEAD_ZONE) Handle_Speed_Y = 0;
				if (Abs(Handle_Speed_X)<=SPEED_DEAD_ZONE) Handle_Speed_X = 0;
				if (Abs(Handle_Speed_Rotation)<=ROTA_DEAD_ZONE) Handle_Speed_Rotation = 0;
				if(Handle_Speed_Y > SPEED_DEAD_ZONE) Handle_Speed_Y-=SPEED_DEAD_ZONE;
				if(Handle_Speed_X > SPEED_DEAD_ZONE) Handle_Speed_X-=SPEED_DEAD_ZONE;
				if(Handle_Speed_Rotation > ROTA_DEAD_ZONE) Handle_Speed_Rotation-=ROTA_DEAD_ZONE;
				if(Handle_Speed_Y < -SPEED_DEAD_ZONE) Handle_Speed_Y += SPEED_DEAD_ZONE;
				if(Handle_Speed_X < -SPEED_DEAD_ZONE) Handle_Speed_X += SPEED_DEAD_ZONE;
				if(Handle_Speed_Rotation < -ROTA_DEAD_ZONE) Handle_Speed_Rotation += ROTA_DEAD_ZONE;
				if(Y_Button1_On)
				{
					Handle_Speed_X /= 10.0;
					Handle_Speed_Y /= 10.0;
					Handle_Speed_Rotation /= 10.0;
				}
				if(Y_Button3_On)
				{

				}
				if(Y_Button4_On)
				{

				}
				if(Y_Button6_Up)
				{

				}
				if(Y_Button6_Down)
				{

				}
				if(Y_Button5_Down)
				{
					
				}
				handle_speed_y = Handle_Speed_Y;
				handle_speed_x = Handle_Speed_X;
				handle_speed_turn = Handle_Speed_Rotation;
				
				

		
		
				SetSpeed(Speed_X+Handle_Speed_X,Speed_Y+Handle_Speed_Y,Speed_Rotation+Handle_Speed_Rotation);	
			}

			else
			{
				//SetSpeed(0,0,0);
			}
			Delay_ms(15);
		}
}
// void pid_set_A(void)
// {
// 	rt_uint32_t key_value;

//     static struct Input_ListValue Values[]=
//     {
//         {&keep_angle_pid.p,	TYPE_FP64,	"angle.p"},
//         {&keep_angle_pid.i,	TYPE_FP64,	"angle.i"},
// 				{&keep_angle_pid.d,	TYPE_FP64,	"angle.d"},
// 				{&aim_angle,TYPE_FP64,"aim_angle"},
//     };

//     LCD_Clear();
//     GPS_Clear();
//     PID_Clear();

//     MY_Input_List(Values,4);

//     if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_FOREVER) == RT_EOK);
// }
/*
 * ???: rt_thread_handle_init
 * ?  ?: ????????
 * ?  ?: ?
 * ?  ?: ?????
 * ?  ?: ????
 */
int rt_thread_handle_init(void)
{
    rt_err_t result;
    
	result = rt_thread_init(&thread_handle, 
                            "thread_handle",
                            rt_thread_entry_handle,
                            RT_NULL,
                            &thread_handle_stack[0], sizeof(thread_handle_stack),
							7, 5);
    if (result == RT_EOK) 
        rt_thread_startup(&thread_handle);
    else
        return RT_ERROR;

    return RT_EOK;
}
// void MY_Input_List(struct Input_ListValue *list,int num)
// {
// 	rt_uint32_t key_value;
// 	int i,index;
// 	int allpage=(num-1)/3;
// 	int nowpage=0;
// 	fp64 out;

// 	while(1)
// 	{
// 		LCD_Clear();	
// 		for(i=0;i<4;i++)
// 		{
// 			index=nowpage*3+i;
// 			if(index<num)
// 			{
// 				LCD_SetXY(0,i);
// 				LCD_Printf("%d.",i+1);
// 				LCD_WriteString(list[index].name);
// 				LCD_WriteString(":");
// 				switch(list[index].type)
// 				{
// 					case TYPE_INT8:
// 						LCD_WriteNum(*(int8_t*)(list[index].value));
// 					break;
// 					case TYPE_UINT8:
// 						LCD_WriteNum(*(uint8_t*)(list[index].value));
// 					break;
// 					case TYPE_INT16:
// 						LCD_WriteNum(*(int16_t*)(list[index].value));
// 					break;
// 					case TYPE_UINT16:
// 						LCD_WriteNum(*(uint16_t*)(list[index].value));
// 					break;
// 					case TYPE_INT32:
// 						LCD_WriteNum(*(int32_t*)(list[index].value));
// 					break;
// 					case TYPE_UINT32:
// 						LCD_WriteNum(*(uint32_t*)(list[index].value));
// 					break;
// 					case TYPE_INT64:
// 						LCD_WriteNum(*(int64_t*)(list[index].value));
// 					break;
// 					case TYPE_UINT64:
// 						LCD_WriteNum(*(uint64_t*)(list[index].value));
// 					break;
// 					case TYPE_FP32:
// 					  LCD_WriteNum(*(fp32*)(list[index].value));
// 					break;
// 					case TYPE_FP64:
// 						LCD_WriteNum(*(fp64*)(list[index].value));
// 					break;
// 				}
// 			}
// 		  else
// 			{
// 				LCD_SetXY(0,i);
// 				LCD_WriteString("                    ");
// 			}
// 			LCD_SetXY(0,4);
// 			LCD_Printf("5.save 6.go %d/%d ",nowpage+1,allpage+1);
// 		}
// 		
// 		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
// 		{
// 			switch(key_value)
// 			{
// 				case pageup:
// 					if(nowpage>0)
// 						nowpage--;
// 					else
// 						nowpage=allpage;
// 				break;
// 				case pagedown:
// 					if(nowpage<allpage)
// 						nowpage++;
// 					else
// 						nowpage=0;
// 				break;
// 				case 1:
// 				case 2:
// 				case 3:
// 					index=key_value+nowpage*3-1;
// 					switch(list[index].type)
// 					{
// 						case TYPE_INT8:
// 							out=(*(int8_t*)(list[index].value));
// 						break;
// 						case TYPE_UINT8:
// 							out=(*(uint8_t*)(list[index].value));
// 						break;
// 						case TYPE_INT16:
// 							out=(*(int16_t*)(list[index].value));
// 						break;
// 						case TYPE_UINT16:
// 							out=(*(uint16_t*)(list[index].value));
// 						break;
// 						case TYPE_INT32:
// 							out=(*(int32_t*)(list[index].value));
// 						break;
// 						case TYPE_UINT32:
// 							out=(*(uint32_t*)(list[index].value));
// 						break;
// 						case TYPE_INT64:
// 							out=(*(int64_t*)(list[index].value));
// 						break;
// 						case TYPE_UINT64:
// 							out=(*(uint64_t*)(list[index].value));
// 						break;
// 						case TYPE_FP32:
// 							out=(*(fp32*)(list[index].value));
// 						break;
// 						case TYPE_FP64:
// 							out=(*(fp64*)(list[index].value));
// 						break;
// 					}
// 					Input_DoubleValue(&out,list[index].name);
// 					switch(list[index].type)
// 					{
// 						case TYPE_INT8:
// 							(*(int8_t*)(list[index].value))=out;
// 						break;
// 						case TYPE_UINT8:
// 							(*(uint8_t*)(list[index].value))=out;
// 						break;
// 						case TYPE_INT16:
// 							(*(int16_t*)(list[index].value))=out;
// 						break;
// 						case TYPE_UINT16:
// 							(*(uint16_t*)(list[index].value))=out;
// 						break;
// 						case TYPE_INT32:
// 							(*(int32_t*)(list[index].value))=out;
// 						break;
// 						case TYPE_UINT32:
// 							(*(uint32_t*)(list[index].value))=out;
// 						break;
// 						case TYPE_INT64:
// 							(*(int64_t*)(list[index].value))=out;
// 						break;
// 						case TYPE_UINT64:
// 							(*(uint64_t*)(list[index].value))=out;
// 						break;
// 						case TYPE_FP32:
// 							(*(fp32*)(list[index].value))=out;
// 						break;
// 						case TYPE_FP64:
// 							(*(fp64*)(list[index].value))=out;
// 						break;
// 					}
// 				break;
// 				case 4:
// 					Input_DoubleValue(&aim_angle,list[3].name);
// 				break;
// 				case 6:
// 					while(1){
// 				error_angle = aim_angle-GPS.radian;
// 	
// 		if(error_angle>pi)
// 			error_angle-=2*pi;
// 		if(error_angle<-pi)
// 			error_angle+=2*pi;
// 	
// 		Vout_A = 1000*AnglePID(error_angle,keep_angle_pid);			//角度纠正值
// 		zhuan=Vout_A;    //赋给zhuan

// 		//LCD_Clear();
// 		LCD_SetXY(0,7);
// 		LCD_WriteString("R:");
// 		LCD_WriteDouble(GPS.radian);
// 		
// 		if(zhuan>300)
// 			zhuan= 300;		
// 		if(zhuan<-300)
// 			zhuan= -300;
// 		
// 		SetSpeed(0,0,zhuan);
// 		if(fabs(error_angle)<=4*pi/180)
// 			break;
// 	}
// 					
// 				case key7:
// 					return;
// 			}
// 		}
// 		Delay_ms(5);
// 	}
// }
/********************************    END OF FILE    ***************************/
