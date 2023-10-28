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

/**********************************************************************************************************
*
*	Function Name: uint8_t Power_ReadParam_OnOff(uint8_t reg)
*	Function:
*   Input Ref:
*	Return Ref:
*
**********************************************************************************************************/
uint8_t Power_ReadParam_OnOff(uint8_t reg)
{	
	uint8_t i,j;
	
	for (i = 0; i < (NUM+ADD_NUM); i++)
	{
		SendData_Power_OnOff(reg);		  /* �������� */
		
		usart_t.gTimer_receive_mb_times =0;
		
		while (1)				/* �ȴ�Ӧ��,��ʱ����յ�Ӧ����break  */
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
			  	break;

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
			  	break;

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



/**********************************************************************************************************
*	�� �� ��: uint8_t Power_ReadParam_OnOff(uint8_t reg)
*	����˵��: ��������. ͨ������01Hָ��ʵ�֣�����֮�󣬵ȴ��ӻ�Ӧ��
*	��    ��: ��
*	�� �� ֵ: 1 ��ʾ�ɹ���0 ��ʾʧ�ܣ�ͨ�ų�ʱ�򱻾ܾ���
**********************************************************************************************************/
#if 0
uint8_t Wifi_LoginParam_On(void)
{	
	uint8_t i;
	
	for (i = 0; i < (NUM+ADD_NUM); i++)
	{
		 SendData_Set_Wifi(0x01);		  /* �������� */
		//time1 = bsp_GetRunTime();	/* ��¼����͵�ʱ�� */
		usart_t.gTimer_receive_mb_times =0;
		
		while (1)				/* �ȴ�Ӧ��,��ʱ����յ�Ӧ����break  */
		{
			bsp_Idle();

			if (usart_t.gTimer_receive_mb_times > BSP_TIMEOUT)		
			{
				break;		/* ͨ�ų�ʱ�� */
			}
			
			
            if(run_t.wifi_led_fast_blink_flag ==1){
				break;		/* ���յ�Ӧ�� */
			}
			

			
		}
		
		
        if(run_t.wifi_led_fast_blink_flag ==1){
			break;		/* ���յ�Ӧ�� */
		 
		}
		else{
			if(i==0){
              i++;
			  ADD_NUM ++;

			}
			else
		       break;
        }
	}
	

    if(run_t.wifi_led_fast_blink_flag ==1)
		return 1 ;/* ���յ�Ӧ�� */
	else
	   return 0;
	
	
}
#endif 
/********************************************************
********************************************************/
#if 0
uint8_t Aanswering_Signal_BuzzerParam_Sound(void)
{
	uint8_t i;
	
	for (i = 0; i < (NUM+ADD_NUM); i++)
	{
		 SendData_Buzzer();
		//time1 = bsp_GetRunTime();	/* ��¼����͵�ʱ�� */
		usart_t.gTimer_receive_mb_times =0;
		
		while (1)				/* �ȴ�Ӧ��,��ʱ����յ�Ӧ����break  */
		{
			iwdg_feed();//bsp_Idle();

			if (usart_t.gTimer_receive_mb_times > BSP_TIMEOUT)		
			{
				break;		/* ͨ�ų�ʱ�� */
			}
			
			
            if(run_t.response_buzzer_sound_flag ==1){
				break;		/* ���յ�Ӧ�� */
			}
			

			
		}
		
		
        if(run_t.response_buzzer_sound_flag ==1){
			break;		/* ���յ�Ӧ�� */
		 
		}
		else{
			if(i==0){
              i++;
			  ADD_NUM ++;

			}
			else
		       break;
        }
	}
	

    if(run_t.response_buzzer_sound_flag ==1)
		return 1 ;/* ���յ�Ӧ�� */
	else
	   return 0;
	



}
#endif 

