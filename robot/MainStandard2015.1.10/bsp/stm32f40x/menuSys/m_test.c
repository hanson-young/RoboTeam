/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/*
 * 函数名: DataBack
 * 描  述: 数据回传函数
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void DataBackTest(void)
{
//	rt_uint32_t key_value;
	
	while(1)
	{
		
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======DataBack======");
		LCD_SetXY(0,1);
		
		#if DataBackEnable
		LCD_WriteString("DataNumber:");
		LCD_WriteInt(DataBack_GetCount());
		LCD_SetXY(0,2);
		LCD_WriteString("Press OK to send");
		
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
		{
			switch(key_value)
			{
				case keyOK:
					LCD_Clear();
					LCD_SetXY(5,3);
					LCD_WriteString("Waiting...");
				
					DataBack_Write(DataBack_GetCount());
					DataBack_Send();
					Com_SendByte(2,'$');
				
					LCD_Clear();
					LCD_SetXY(7,3);
					LCD_WriteString("Done~!");
					Delay_ms(500);
					break;
				case keyback:
					return;
			}
		}
		#else 
			LCD_WriteString("Please Enable the   DataBack Function");
		#endif
		Delay_ms(10);
	}
}

/********************************    END OF FILE    ***************************/

