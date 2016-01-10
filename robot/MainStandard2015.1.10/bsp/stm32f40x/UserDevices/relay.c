/* Includes ------------------------------------------------------------------*/
#include "includes.h"

uint32_t RELAY_VALUE=0;


//=================================================================
/************************************************************************************************
** ��������	:	Relay_Multi
** ��������	:   set  �� clear ĳ���̵����˿ڣ�����ͬʱ����������
** �䡡�� 	:	1.set_num  ��Ҫset�Ķ˿�
**              2.clear_num��Ҫclear�Ķ˿�
**          	ʹ��ʾ������set 12  13 �ں�clear 15 ��44��62�ڣ�����������:
**          	Relay(RELAY_NUM(3)|RELAY_NUM(7),RELAY_NUM(15)|RELAY_NUM(44)|RELAY_NUM(62))
** �䡡��   :	
** ע������ :ѡ��˿����������궨�壺 RELAY_LOW(port) 
**************************************************************************************************/
// void Relay_Multi(uint64_t set_num,uint64_t clear_num)
// {
// 	MISO_RELAY64_DATA.u64_data |=  set_num;

// 	MISO_RELAY64_DATA.u64_data &=  ~clear_num;
// 	
// 	RELAY_VALUE = MISO_RELAY64_DATA.u64_data;
// 	
// 	Write_Database(W_RELAY64_ID);
// }

/*������λĳ���˿�*/
void Relay_Set(uint8_t port)
{
	if((port>0)&&(port<17))
	{
		MOSI_RELAY1_DATA = (uint8_t)((port-1) | 0x80);
	
		Write_Database(W_RELAY1_ID);
	}else if((port>16)&&(port<33))
	{
		MOSI_RELAY2_DATA = (uint8_t)((port-17) | 0x80);
	
		Write_Database(W_RELAY2_ID);
	}
	RELAY_VALUE |= RELAY_NUM(port);
}

/*������λĳ���˿�*/
void Relay_Clear(uint8_t port)
{
	static uint8_t i;
	if((port>0)&&(port<17))
	{
		MOSI_RELAY1_DATA = (port-1);
		i &= 0x12;
		i++;
		
		Write_Database(W_RELAY1_ID);
	}else if((port>16)&&(port<33))
	{
		MOSI_RELAY2_DATA = (port-17);
		
		Write_Database(W_RELAY2_ID);
	}
	RELAY_VALUE &= ~RELAY_NUM(port);
}


/*
 * ������: Realy_View
 * ��  ��: �鿴�̵�����Ϣ
 * ��  ��: ��
 * ��  ��: ��
 * ��  ��: �ⲿ����
 */
void Relay_View(void)
{
	rt_uint32_t key_value;
	int i,j;
	int page=0;
	
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("=======Relay========");
		
		for(i=0;i<2;i++)
			for(j=0;j<4;j++)
			{
				LCD_SetXY(i*10,j+1);
				LCD_Printf("%d:",8*page+i*4+j+1);
				if(RELAY_VALUE>>(8*page+i*4+j) & 1)
					LCD_WriteString("Set");
				else
					LCD_WriteString("Clear");
			}
		
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{
			switch(key_value)
			{
				case key1:
					if(RELAY_VALUE>>(8*page) & 1)
						Relay_Clear(8*page+1);
					else
						Relay_Set(8*page+1);
					break;
				case key2:
					if(RELAY_VALUE>>(8*page+1) & 1)
						Relay_Clear(8*page+2);
					else
						Relay_Set(8*page+2);
					break;
				case key3:
					if(RELAY_VALUE>>(8*page+2) & 1)
						Relay_Clear(8*page+3);
					else
						Relay_Set(8*page+3);
					break;
				case key4:
					if(RELAY_VALUE>>(8*page+3) & 1)
						Relay_Clear(8*page+4);
					else
						Relay_Set(8*page+4);
					break;
				case key5:
					if(RELAY_VALUE>>(8*page+4) & 1)
						Relay_Clear(8*page+5);
					else
						Relay_Set(8*page+5);
					break;
				case key6:
					if(RELAY_VALUE>>(8*page+5) & 1)
						Relay_Clear(8*page+6);
					else
						Relay_Set(8*page+6);
					break;
				case key7:
					if(RELAY_VALUE>>(8*page+6) & 1)
						Relay_Clear(8*page+7);
					else
						Relay_Set(8*page+7);
					break;
				case key8:
					if(RELAY_VALUE>>(8*page+7) & 1)
						Relay_Clear(8*page+8);
					else
						Relay_Set(8*page+8);
					break;
				case pageup:
					if(page>0) page--;
					break;
				case pagedown:
					if(page<3) page++;
					break;
				case keyback:
					return;
			}
		}
		Delay_ms(10);
	}
}

u8 relay_attain(u8 port)
{
	return (RELAY_VALUE>>(port-1))&1;
}

/*----------------------------------------------------------------------------*
 *  
 *  ����ĵ�ŷ���ÿ���³�������Ҫ���£��ر�ע��:����(HALL)�˿�
 *  
 *----------------------------------------------------------------------------*/


void HandOpen(void)//������  ������5.4
{
	Relay_Set(20);
}
void HandClose(void)
{
	Relay_Clear(20);
}
void HandTurnRight(void)//��ת����  ������5.4
{
// 	Relay_Clear(19);
	Relay_Set(23);
// 	Delay_ms(500);
// 	Relay_Set(20);
}
void HandTurnLeft(void)
{
// 	Relay_Clear(20);
	Relay_Clear(23);
// 	Delay_ms(500);
// 	Relay_Set(19);
}
void HandPush(void)   //  ������5.4
{
	//Relay_Set(24);
}
void HandPushOff(void)
{
	//Relay_Clear(24);
}
void HandRaise(void)//��������  ������5.4
{
	Relay_Set(19);
}
void HandFall(void)
{
	Relay_Clear(19);
}
void BufferOn(void)//�򿪻���װ�ã����ײ�ѹ��  ������5.4
{
    Relay_Clear(1);
}
void BufferOff(void)
{
	Relay_Set(1);
}
void GunOn(void)//���ô���  ������5.4
{
	HandClose();
	Relay_Set(2);
}
void GunOff(void)//�������
{
	HandClose();
	Relay_Clear(2);
}
void FireOn(void)//����
{
	Relay_Clear(3);
	Relay_Set(4);
	Relay_Clear(5);
	Relay_Set(6);
}
void FireOff(void)
{
	Relay_Clear(4);
	Relay_Set(3);
	Relay_Clear(6);
	Relay_Set(5);
}
void FireClear(void)//�����赲״̬
{
	Relay_Clear(3);
	Relay_Clear(4);
	Relay_Clear(5);
	Relay_Clear(6);
}

/*----------------------------------------------------------------------------*
 *  ����������ͨ�ţ������ߣ���Ϊ��ʼλ��Ȼ�����4���ź�λ
 *  ��������:
 *  0001: ���ΰ��ʼ����SS��
 *	0010���ɰ��� & ÷��׮��ʼ����PW��
 *  0011���е�һ������
 *  0100�������һ������ & ��ǧ��ʼ��(SW) & ����ǧ
 *  0101���ɿ���ǧ
 *  0110����¥�ݳ�ʼ����JG�� & ץ¥��

 *  0111��
 *  1000��
 *  1001��

 *  1010��
 *	1011��
 *	1011��
 *----------------------------------------------------------------------------*/

/*
 * ������: Hall_Send
 * ��  ��: 
 * ��  ��: ��
 * ��  ��: ��
 * ��  ��: �ⲿ����
 */
 u8 Hall_Count = 0;
 
void Hall_Send( u8 data)
{
	u8 count ;
	u8 port = 18;
	u8 delay = 20;

	Relay_Set(port);
	Delay_ms( delay );

	for( count = 4 ; count > 0 ; count -- )
	{
		if( data & (1<<(count-1)) )
			Relay_Set(port);
		else
			Relay_Clear(port);
		
		Delay_ms( delay );
	}
	Relay_Clear(port);
	
	Hall_Count = data+1;
}



/**********************************END OF FILE*********************************/
