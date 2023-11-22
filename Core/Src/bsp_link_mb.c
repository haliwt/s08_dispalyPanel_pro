#include "bsp_link_mb.h"
#include "bsp.h"

#define BSP_TIMEOUT		1		/* 锟斤拷锟斤拷锟斤拷锟筋超时时锟斤拷, 锟斤拷位ms */
#define NUM			1			/* 循锟斤拷锟斤拷锟酵达拷锟斤拷 */

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
*	函 数 名: uint8_t Power_ReadParam_OnOff(uint8_t reg)
*	功能说明: 单个参数. 通过发送06H指令实现，发送之后，等待从机应答。循环NUM次写命令
*	形    参: 无
*	返 回 值: 1 表示成功。0 表示失败（通信超时或被拒绝）
*********************************************************************************************************/
uint8_t Power_ReadParam_OnOff(uint8_t reg)
{	
	uint8_t i,j;
	
	for (i = 0; i < (NUM+ADD_NUM); i++)
	{
		SendData_Power_OnOff(reg);  /*send power  on /off command*/
		
		usart_t.gTimer_receive_mb_times =0;/* 记录命令发送的时刻 */
		
		while (1)				/*  */
		{
			bsp_Idle();

			if (usart_t.gTimer_receive_mb_times > BSP_TIMEOUT)		
			{
				break;		/* 通锟脚筹拷时锟斤拷 */
			}
			
			if (reg == 1 )
			{
               if(usart_t.response_power_on ==1)
				break;		/* 锟斤拷锟秸碉拷应锟斤拷 */
			}
			else{
				if(usart_t.response_power_off ==1)
			    break;;	/* 锟斤拷锟秸碉拷应锟斤拷 */

			}
		}
	
		
		if (reg == 1 ){ //power on 
			if(usart_t.response_power_on ==1)break;		/* mainboard receive power on command signal */
			else{
			if(j==0){
					j++;
					ADD_NUM++;

				}
				else 
				  return 0;
			  }
			
		}
		else{ //power off
			if(usart_t.response_power_off ==1)
			return 1;	/* mainboard receive power off command signal */
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
	
	if (reg == 1 ) //power on
	{
       if(usart_t.response_power_on ==1)
		return 1 ;/* 锟斤拷锟秸碉拷应锟斤拷 */
	   else
	   	return 0;
	}
	else{ //power off
		
		if(usart_t.response_power_off ==1)
			return 1;
        else 
			return 0;
	}

}


