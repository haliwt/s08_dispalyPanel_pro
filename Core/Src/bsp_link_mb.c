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





