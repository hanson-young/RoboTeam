#include "all.h"

#define USART1_DR_Address ((u32)0x40013804)     //USART1-DR ��ַ

volatile uint8_t USART1_RxBuffer[RxBufferSize];

void UART1_DMA_init(u32 baud){
    //GPIO�˿�����
	uint32_t i;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//������γ���ǳ���Ҫ��������
	/*������Ϊ8��8λ���ݣ�ͬһ֡����֮��ļ���ǳ�С����֮֡��ļ��Լ1.25ms�����3.4ms����һ������
	��������ʽ��ʼ���������������´��룬���ܱ�֤�ڽ�uartʹ��˲�䣬�����Ƿ��ص����ݴ��ں���״̬������״̬or�����У�
	���ڿ���״̬��ͨ��UART1_DMA_EN()�����ܽ�����ͬ��
	�����ڷ���״̬�����֡���ݿ϶�������ȷ���գ������յ���λ�����ݡ�
	���۷�����ͬ֡���ݼ�ļ��С�����������Ӧ������֡��ʱ������1ms�࣬�㹻��38400�����ʵ�8λ���ݴ��ڿ��С�
	����֪Ϊ�Σ��ڵ���UART1_DMA_EN()����ʱ����ֺܴ���ʵ�������
	�����յ���������ȫ�������֣�����״̬ʱuart���յ�������ƾ�ձ����9�����ݡ�֡ͷ��Ȼ��0xDD���������ݱ����9�����ݣ��϶�����Ҳ�޷�ͬ�����ˡ�
	�Ҿ���Ӧ����stm32�ڲ���Ӳ��uart����ĳ��С���⣬��uart��ʹ��ʱ���������ڷ��;ͻ����һЩС���⡣
	ͨ����������IO���Ĵ���֮���ܱ�֤UARTʹ��ʱ������֮֡��Ŀ���̬���ظ��ܶ�ܶ�κ���δ�������⡣
	����700�μ�⣬ͨ��ʾ�����۲�����Ҫ560us��û�н��м���
	*/
	i=0;
	while(i<=20)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)==0)
		{
			i++;
		}
	}
	i=0;
	while(i<700)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)==1)
		{
			i++;
			LED2_on;
		}else
		{
			i=0;
			LED2_off;
		}
	}
	//������γ���ǳ���Ҫ��������
	
		/* Enable the USART1 DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//Usart1 DMA ����
	/* USARTy RX DMA1 Channel --DMA1_Channel5(triggered by USART1 Rx event) Config */
	DMA_DeInit(DMA1_Channel5); 												// 
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Address;			//DMAͨ��1�ĵ�ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_RxBuffer;		//DMA���͵�ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//���ͷ��� USART������
	DMA_InitStructure.DMA_BufferSize = RxBufferSize;						//�����ڴ��С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//����Դ��ַ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//�����ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//Դ��ַ�����ݳ�����8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//���͵�Ŀ�ĵ�ַ��8λ����
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							//����ģʽѭ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;					//���ȼ�����
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							//DMAͨ��û������Ϊ�ڴ浽�ڴ洫�� 
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);							//


//    //Usart1 NVIC ����
/*2014.3.16  4:27*/
//     NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
// 		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
// 		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//

// 		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
// 		NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1

	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = baud;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/* Enable USARTy DMA Rx request */
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	/* ����DMA1ͨ��1��������ж� */
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC, ENABLE);
	/* Enable USARTy RX DMA1 Channel */
	DMA_Cmd(DMA1_Channel5, DISABLE);
   
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

} 

void DMA_check(void)
{
	if(USART1_RxBuffer[7] != 0xDD)
	{
		DMA_Cmd(DMA1_Channel5, DISABLE);
        
        Gyro_Error_Flag = 1;
		LED4_on;
	}
}

void UART1_DMA_EN(void)
{
	uint8_t gyro_start,j;
	uint8_t gyro_temp;
	gyro_temp = 0;
	gyro_start=1;    //���������ݿ�ͷ��־����ʱ��DMAͨ��ʹ��
	j=0;
	
	while(gyro_start)
	{
		if(j==0)
		{
			if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
			{
				USART_ClearFlag(USART1, USART_FLAG_ORE);
				gyro_temp = USART_ReceiveData(USART1);
			}

			if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
			{
				USART_ClearITPendingBit(USART1, USART_IT_RXNE);
				gyro_temp = USART_ReceiveData(USART1);
			}
			
			if(gyro_temp == 0xDD)
			{
				j=1;
			}
		}
		else
		{
			if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
			{
				USART_ClearFlag(USART1, USART_FLAG_ORE);
				gyro_temp = USART_ReceiveData(USART1);
				j++;
			}

			if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
			{
				USART_ClearITPendingBit(USART1, USART_IT_RXNE);
				gyro_temp = USART_ReceiveData(USART1);
				j++;
			}

			if(j==9)
			{
				if(gyro_temp==0xDD)
				{
					gyro_start = 0;				//�������7�ֽڵ�����0xDD����Ϊ���������ǵ�֡ͷ������DMAͨ��
					DMA_Cmd(DMA1_Channel5, ENABLE);
					LED4_off;
				}
				else
				{
					j=0;
				}
			}else if(j>9)
			{
				j=0;
			}
		}
	}
}

void UART1_SendByte(uint8_t ch)
{
  USART_SendData(USART1, (uint8_t) ch);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}


int64_t Gyro_Float_Total1;
int64_t Gyro_Float_Total2;
void DMA1_Channel5_IRQHandler(void)
{
	static union u8to32
	{
		int32_t value;
		uint8_t datas[4];
	}gyro;
    
	if(DMA_GetITStatus(DMA1_IT_GL5)==SET)
	{
		gyro.datas[3] = USART1_RxBuffer[1];
		gyro.datas[2] = USART1_RxBuffer[2];
		gyro.datas[1] = USART1_RxBuffer[0];
		gyro.datas[0] = 0;
		
		DMA_ClearITPendingBit(DMA1_IT_GL5);
		gyro.value >>=8;
		Gyro_Now.value= gyro.value;
		Gyro_Now.count++;
//         LCD_SetXY(0,1);
//         LCD_PushString("        ");
//         LCD_WriteIntXY(Gyro_Now.count,0,1);
        
		Gyro_Update();
		if(Gyro_Now.count==256&&Gyro_Float_Flag1)
		{
			Gyro_Float_Total1=Gyro_Total;
			Gyro_Float_Flag1=0;
		}
		if(Gyro_Now.count==1280&&Gyro_Float_Flag2)
		{
            Gyro_Float_Total2=Gyro_Total;
			Gyro_Float_Flag2=0;
			Gyro_Float_Flag=0;
		}
	}
}