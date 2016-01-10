
/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* ������ƿ� */

struct rt_mailbox Mb_Key;
struct rt_mailbox Mb_Emergency;
struct rt_mailbox Mb_Auto;
struct rt_mailbox Mb_SensorStart;
struct rt_mailbox Mb_SensorEnd;

/* ���ڷ��ʼ����ڴ�� */

static char Mb_Key_pool[128];
static char Mb_Emergency_pool[128];
static char Mb_Auto_pool[128];
static char Mb_SensorStart_pool[128];
static char Mb_SensorEnd_pool[128];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*
 * ������: Mb_Key_Init
 * ��  ��: ���������ʼ��
 * ��  ��: ��
 * ��  ��: ��ʼ�����
 * ��  ��: �ⲿ����
 */
int Mb_Key_Init(void)
{
    rt_err_t result;
    
    result = rt_mb_init(
						&Mb_Key,
						"Mb_Key",             			// ����
						Mb_Key_pool,       				// �����õ����ڴ��
						sizeof(Mb_Key_pool)/4, 			// �����е��ʼ���Ŀ��һ���ʼ�ռ4�ֽ�
						RT_IPC_FLAG_FIFO); 				// ����FIFO��ʽ�����̵߳ȴ�
    
    if (result != RT_EOK)
    {
        rt_kprintf("init Mb_Key failed.\n");
        return 0;
	}
    return 1;
}

/*
 * ������: Mb_Emergency_Init
 * ��  ��: �������������ʼ��
 * ��  ��: ��
 * ��  ��: ��ʼ�����
 * ��  ��: �ⲿ����
 */
int Mb_Emergency_Init(void)
{
    rt_err_t result;
    
    result = rt_mb_init(
						&Mb_Emergency,
						"Mb_Emergency",             	// ����
						Mb_Emergency_pool,       		// �����õ����ڴ��
						sizeof(Mb_Emergency_pool)/4, 	// �����е��ʼ���Ŀ��һ���ʼ�ռ4�ֽ�
						RT_IPC_FLAG_FIFO); 				// ����FIFO��ʽ�����̵߳ȴ�
    
    if (result != RT_EOK)
    {
        rt_kprintf("init Mb_Emergency failed.\n");
        return 0;
	}
    return 1;
}

/*
 * ������: Mb_Auto_Init
 * ��  ��: �Զ����������ʼ��
 * ��  ��: ��
 * ��  ��: ��ʼ�����
 * ��  ��: �ⲿ����
 */
int Mb_Auto_Init(void)
{
    rt_err_t result;
    
    result = rt_mb_init(
						&Mb_Auto,
						"Mb_Auto",             		// ����
						Mb_Auto_pool,       		// �����õ����ڴ��
						sizeof(Mb_Auto_pool)/4, 	// �����е��ʼ���Ŀ��һ���ʼ�ռ4�ֽ�
						RT_IPC_FLAG_FIFO); 			// ����FIFO��ʽ�����̵߳ȴ�
    
    if (result != RT_EOK)
    {
        rt_kprintf("init Mb_Auto failed.\n");
        return 0;
	}
    return 1;
}


int Mb_SensorStart_Init(void)
{
    rt_err_t result;
    
    result = rt_mb_init(
						&Mb_SensorStart,
						"Mb_SensorStart",             		// ����
						Mb_SensorStart_pool,       			// �����õ����ڴ��
						sizeof(Mb_SensorStart_pool)/4, 		// �����е��ʼ���Ŀ��һ���ʼ�ռ4�ֽ�
						RT_IPC_FLAG_FIFO); 			// ����FIFO��ʽ�����̵߳ȴ�
    
    if (result != RT_EOK)
    {
        rt_kprintf("init Mb_SensorStart failed.\n");
        return 0;
	}
    return 1;
}

int Mb_SensorEnd_Init(void)
{
    rt_err_t result;
    
    result = rt_mb_init(
						&Mb_SensorEnd,
						"Mb_SensorEnd",             		// ����
						Mb_SensorEnd_pool,       			// �����õ����ڴ��
						sizeof(Mb_SensorEnd_pool)/4, 		// �����е��ʼ���Ŀ��һ���ʼ�ռ4�ֽ�
						RT_IPC_FLAG_FIFO); 			        // ����FIFO��ʽ�����̵߳ȴ�
    
    if (result != RT_EOK)
    {
        rt_kprintf("init Mb_SensorEnd failed.\n");
        return 0;
	}
    return 1;
}



/********************************    END OF FILE    ***************************/

