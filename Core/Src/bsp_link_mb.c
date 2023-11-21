#include "bsp_link_mb.h"
#include "bsp.h"

#define BSP_TIMEOUT		1		/* �������ʱʱ��, ��λms */
#define NUM			1			/* ѭ�����ʹ��� */

usart_rx_tx_t usart_t;


uint8_t ADD_NUM ;
/**********************************************************************************************************
*
*	Function Name: void bsp_Idle(void)
*	Function:
*   Input Ref:
*	Return Ref:
*
**********************************************************************************************************/
void bsp_Idle(void)
{

  Decode_Handler();

}


/*********************************************************************************************************
*	�� �� ��: uint8_t Power_ReadParam_OnOff(uint8_t reg)
*	����˵��: ��������. ͨ������06Hָ��ʵ�֣�����֮�󣬵ȴ��ӻ�Ӧ��ѭ��NUM��д����
*	��    ��: ��
*	�� �� ֵ: 1 ��ʾ�ɹ���0 ��ʾʧ�ܣ�ͨ�ų�ʱ�򱻾ܾ���
*********************************************************************************************************/
uint8_t Power_ReadParam_OnOff(uint8_t reg)
{	
	uint8_t i,j;
	
	for (i = 0; i < (NUM+ADD_NUM); i++)
	{
		SendData_Power_OnOff(reg);  /*send power  on /off command*/
		
		usart_t.gTimer_receive_mb_times =0;/* ��¼����͵�ʱ�� */
		
		while (1)				/*  */
		{
			bsp_Idle();

			if (usart_t.gTimer_receive_mb_times > BSP_TIMEOUT)		
			{
				break;		/* ͨ�ų�ʱ�� */
			}
			
			if (reg == 1 )
			{
               if(usart_t.response_power_on ==1)
				return 1;		/* ���յ�Ӧ�� */
			}
			else{
				if(usart_t.response_power_off ==1)
			    return 1;	/* ���յ�Ӧ�� */

			}
		}
	
		
	if (reg == 1 )
		{
      if(usart_t.response_power_on ==1)
			    return 1;		/* ���յ�Ӧ�� */
		  else{
		   	  if(j==0){
			  	  j++;
		      	ADD_NUM++;

		   	   }
					 else 
						 return 0;
			}
			   
	}
	else{
			if(usart_t.response_power_off ==1)
			return 1;		/* ���յ�Ӧ�� */
			else{
				if(j==0){
			  	j++;
		      ADD_NUM++;

		   	  }
			    else 
						return 0;

			}

		}
	
	}
	if (reg == 1 )
	{
       if(usart_t.response_power_on ==1)
		return 1 ;/* ���յ�Ӧ�� */
	   else
	   	return 0;
	}
	else{
		
		if(usart_t.response_power_off ==1)
			return 1;
        else 
			return 0;
	}
}





