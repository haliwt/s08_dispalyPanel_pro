#include "bsp_link_mb.h"
#include "bsp.h"

#define BSP_TIMEOUT		1		/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½î³¬Ê±Ê±ï¿½ï¿½, ï¿½ï¿½Î»ms */
#define NUM			1			/* Ñ­ï¿½ï¿½ï¿½ï¿½ï¿½Í´ï¿½ï¿½ï¿½ */

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
*	º¯ Êý Ãû: uint8_t Power_ReadParam_OnOff(uint8_t reg)
*	¹¦ÄÜËµÃ÷: µ¥¸ö²ÎÊý. Í¨¹ý·¢ËÍ06HÖ¸ÁîÊµÏÖ£¬·¢ËÍÖ®ºó£¬µÈ´ý´Ó»úÓ¦´ð¡£Ñ­»·NUM´ÎÐ´ÃüÁî
*	ÐÎ    ²Î: ÎÞ
*	·µ »Ø Öµ: 1 ±íÊ¾³É¹¦¡£0 ±íÊ¾Ê§°Ü£¨Í¨ÐÅ³¬Ê±»ò±»¾Ü¾ø£©
*********************************************************************************************************/
uint8_t Power_ReadParam_OnOff(uint8_t reg)
{	
	uint8_t i,j;
	
	for (i = 0; i < (NUM+ADD_NUM); i++)
	{
		SendData_Power_OnOff(reg);  /*send power  on /off command*/
		
		usart_t.gTimer_receive_mb_times =0;/* ¼ÇÂ¼ÃüÁî·¢ËÍµÄÊ±¿Ì */
		
		while (1)				/*  */
		{
			bsp_Idle();

			if (usart_t.gTimer_receive_mb_times > BSP_TIMEOUT)		
			{
				break;		/* Í¨ï¿½Å³ï¿½Ê±ï¿½ï¿½ */
			}
			
			if (reg == 1 )
			{
               if(usart_t.response_power_on ==1)
				return 1;		/* ï¿½ï¿½ï¿½Õµï¿½Ó¦ï¿½ï¿½ */
			}
			else{
				if(usart_t.response_power_off ==1)
			    return 1;	/* ï¿½ï¿½ï¿½Õµï¿½Ó¦ï¿½ï¿½ */

			}
		}
	
		
	if (reg == 1 )
		{
      if(usart_t.response_power_on ==1)
			    return 1;		/* ï¿½ï¿½ï¿½Õµï¿½Ó¦ï¿½ï¿½ */
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
			return 1;		/* ï¿½ï¿½ï¿½Õµï¿½Ó¦ï¿½ï¿½ */
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
		return 1 ;/* ï¿½ï¿½ï¿½Õµï¿½Ó¦ï¿½ï¿½ */
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
*	ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½: uint8_t Power_ReadParam_OnOff(uint8_t reg)
*	ï¿½ï¿½ï¿½ï¿½Ëµï¿½ï¿½: ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½. Í¨ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½01HÖ¸ï¿½ï¿½Êµï¿½Ö£ï¿½ï¿½ï¿½ï¿½ï¿½Ö®ï¿½ó£¬µÈ´ï¿½ï¿½Ó»ï¿½Ó¦ï¿½ï¿½
*	ï¿½ï¿½    ï¿½ï¿½: ï¿½ï¿½
*	ï¿½ï¿½ ï¿½ï¿½ Öµ: 1 ï¿½ï¿½Ê¾ï¿½É¹ï¿½ï¿½ï¿½0 ï¿½ï¿½Ê¾Ê§ï¿½Ü£ï¿½Í¨ï¿½Å³ï¿½Ê±ï¿½ò±»¾Ü¾ï¿½ï¿½ï¿½
**********************************************************************************************************/
#if 0
uint8_t Wifi_LoginParam_On(void)
{	
	uint8_t i;
	
	for (i = 0; i < (NUM+ADD_NUM); i++)
	{
		 SendData_Set_Wifi(0x01);		  /* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
		//time1 = bsp_GetRunTime();	/* ï¿½ï¿½Â¼ï¿½ï¿½ï¿½î·¢ï¿½Íµï¿½Ê±ï¿½ï¿½ */
		usart_t.gTimer_receive_mb_times =0;
		
		while (1)				/* ï¿½È´ï¿½Ó¦ï¿½ï¿½,ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ï¿½Õµï¿½Ó¦ï¿½ï¿½ï¿½ï¿½break  */
		{
			bsp_Idle();

			if (usart_t.gTimer_receive_mb_times > BSP_TIMEOUT)		
			{
				break;		/* Í¨ï¿½Å³ï¿½Ê±ï¿½ï¿½ */
			}
			
			
            if(run_t.wifi_led_fast_blink_flag ==1){
				break;		/* ï¿½ï¿½ï¿½Õµï¿½Ó¦ï¿½ï¿½ */
			}
			

			
		}
		
		
        if(run_t.wifi_led_fast_blink_flag ==1){
			break;		/* ï¿½ï¿½ï¿½Õµï¿½Ó¦ï¿½ï¿½ */
		 
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
		return 1 ;/* ï¿½ï¿½ï¿½Õµï¿½Ó¦ï¿½ï¿½ */
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
		//time1 = bsp_GetRunTime();	/* ï¿½ï¿½Â¼ï¿½ï¿½ï¿½î·¢ï¿½Íµï¿½Ê±ï¿½ï¿½ */
		usart_t.gTimer_receive_mb_times =0;
		
		while (1)				/* ï¿½È´ï¿½Ó¦ï¿½ï¿½,ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ï¿½Õµï¿½Ó¦ï¿½ï¿½ï¿½ï¿½break  */
		{
			iwdg_feed();//bsp_Idle();

			if (usart_t.gTimer_receive_mb_times > BSP_TIMEOUT)		
			{
				break;		/* Í¨ï¿½Å³ï¿½Ê±ï¿½ï¿½ */
			}
			
			
            if(run_t.response_buzzer_sound_flag ==1){
				break;		/* ï¿½ï¿½ï¿½Õµï¿½Ó¦ï¿½ï¿½ */
			}
			

			
		}
		
		
        if(run_t.response_buzzer_sound_flag ==1){
			break;		/* ï¿½ï¿½ï¿½Õµï¿½Ó¦ï¿½ï¿½ */
		 
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
		return 1 ;/* ï¿½ï¿½ï¿½Õµï¿½Ó¦ï¿½ï¿½ */
	else
	   return 0;
	



}
#endif 

