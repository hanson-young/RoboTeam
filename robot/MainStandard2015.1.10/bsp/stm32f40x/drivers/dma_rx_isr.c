
#include "io.h"
#include "dma.h"
#include "usart.h"
#include "dma_rx_isr.h"

/* light board */
LB_t LB_recv[2];

/* manual */
Handle_Data HandleData_X,HandleData_Y;

/* laser rangefinder */
laser_t Laser_recv[2];

/* motor */
motor_t mt2;

void rt_hw_serial_dma_rx_isr(u8 uartx)
{
	//rt_uint32_t level;
    uart_dma_t *p_uart_dma = _get_uart_dma(uartx);
    u8 f_size = p_uart_dma->recv_size;  //frame size
    
    /* ????????,?dma???????? */
    DMA_Stream_TypeDef *rx = _get_stream(p_uart_dma->rx_dmax, p_uart_dma->rx_streamx);
    
    char *addr = (char *)rx->M0AR;
    DMA_Cmd(rx, DISABLE);
    
    /* ????????? ,???????????????,?????? */
    if(*addr==p_uart_dma->recv_head && rx->NDTR==f_size)   
    {   
        int i=0;
        char *rbuf = (char *)p_uart_dma->recv_addr;
        
        assert_param(rbuf != RT_NULL);
        
				/** @receive @correctly */
				LED_Toggle(2);
        
				for(i=0; i<p_uart_dma->recv_size; i++)
            *(rbuf+i) = *((char *)(rx->M0AR) + i);
		
		/* ????????? */
		p_uart_dma->rx_cnt ++;
    }
    else if(rx->NDTR == (uint32_t)f_size)
    {/* ?????,?????????????? */  
        
        /* ????dma?????,??????? */
        /* ??? */
        int i=0;
        //int fh_pos=0; /* frame head position */
        for(i=0; i<f_size; i++)
        {
            if(*(addr+i)==p_uart_dma->recv_head)
                break;
        }
        
        if(i==f_size)
        {/* ??????,???????????*/
        }
        else
        {/* ????????*/
            rx->NDTR += i;
        }
        
    }
    else
    {   /* ??????,??????????? */
        /* ??? */
        int i=0;
        //int fh_pos=0; /* frame head position */
        for(i=0; i<f_size; i++)
        {
            if(*(addr+i)==p_uart_dma->recv_head)
                break;
        }
        if(i==f_size)
        {/* ??????, ????*/
        }
        else
        {/* ???????? */
            if((rx->NDTR -i)==f_size)
            {/* ???????????? */
                rx->NDTR = f_size;
            }
        }
    }
    DMA_Cmd(rx, ENABLE);
}



void laser_dma_rx_isr(u8 uartx)
{
	//rt_uint32_t level;
    uart_dma_t *p_uart_dma = _get_uart_dma(uartx);
    u8 f_size = p_uart_dma->recv_size;  //frame size
    
    /* ????????,?dma???????? */
    DMA_Stream_TypeDef *rx = _get_stream(p_uart_dma->rx_dmax, p_uart_dma->rx_streamx);
    
    char *addr = (char *)rx->M0AR;
    DMA_Cmd(rx, DISABLE);
    
    /* ????????? ,???????????????,?????? */
    if(*addr==p_uart_dma->recv_head && rx->NDTR==f_size)   
    {   
        int i=0;
        char *rbuf = (char *)p_uart_dma->recv_addr;
        
        assert_param(rbuf != RT_NULL);
        
				/** @receive @correctly */
				LED_Toggle(2);
        
				for(i=0; i<p_uart_dma->recv_size; i++)
            *(rbuf+i) = *((char *)(rx->M0AR) + i);
		
		/* ????????? */
		p_uart_dma->rx_cnt ++;
    }
    else if(rx->NDTR == (uint32_t)f_size)
    {/* ?????,?????????????? */  
        
        /* ????dma?????,??????? */
        /* ??? */
        int i=0;
        //int fh_pos=0; /* frame head position */
        for(i=0; i<f_size; i++)
        {
            if(*(addr+i)==p_uart_dma->recv_head)
                break;
        }
        
        if(i==f_size)
        {/* ??????,???????????*/
        }
        else
        {/* ????????*/
            rx->NDTR += i;
        }
        
    }
    else
    {   /* ??????,??????????? */
        /* ??? */
        int i=0;
        //int fh_pos=0; /* frame head position */
        for(i=0; i<f_size; i++)
        {
            if(*(addr+i)==p_uart_dma->recv_head)
                break;
        }
        if(i==f_size)
        {/* ??????, ????*/
        }
        else
        {/* ???????? */
            if((rx->NDTR -i)==f_size)
            {/* ???????????? */
                rx->NDTR = f_size;
            }
        }
    }
    DMA_Cmd(rx, ENABLE);
}
