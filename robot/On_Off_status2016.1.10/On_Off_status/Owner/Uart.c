#include "all.h"

void Uart2_Init(u16 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	 //USART4_TX   PA2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART4_RX	  PA3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
	
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;			// 时钟低电平活动
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;				// 时钟低电平
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;				// 时钟第二个边沿进行数据捕获
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;		// 最后一位数据的时钟脉冲不从SCLK输出
  /* Configure the USART2 synchronous paramters */
  USART_ClockInit(USART2, &USART_ClockInitStructure);					// 时钟参数初始化设置
	  
	//USART 初始化设置
   
	USART_InitStructure.USART_BaudRate = baudrate;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  
	
	USART_Cmd(USART2, ENABLE);                    //使能串口
	
}




void USART1_Configuration(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
  
   //USART 初始化设置
   
	USART_InitStructure.USART_BaudRate = 115200;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
   

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
   
    USART_Cmd(USART1, ENABLE);                    //使能串口 
//这里是开始DMA传输前的一些准备工作，将USART1模块设置成DMA方式工作
// DMA_Configuration();

// USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
//开始一次DMA传输！
//DMA_Cmd(DMA1_Channel4, ENABLE);


}


// void Uart1_SendString(uint8* data)
// {
// 	while(*data)
// 	{					
// 		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	   			 
// 		USART_SendData(USART1,*data++);
//     }
// }

// void Uart1_SendByte(uint8 data)
// {	  	 
// 	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);   
// 	USART_SendData(USART1,data);
// }
void Get_Distance1(void)
{
	USART_SendData(USART1,0x55);
}

void Get_Distance2(void)
{
	USART_SendData(USART2,0x55);
}

void USART2_IRQHandler(void)
{
 
	uint8_t rec_buf2 = 0;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		rec_buf2 = USART_ReceiveData(USART2);
		LED2_on;
			
	}
}

void USART1_IRQHandler(void)
{

	uint8_t rec_buf = 0;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
	  USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		rec_buf = USART_ReceiveData(USART1);
		USART_SendData(USART1,rec_buf);
		LED3_on;	
  }

}
