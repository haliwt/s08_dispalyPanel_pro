#include "run.h"
#include "smg.h"
#include "gpio.h"
#include "cmd_link.h"
#include "bsp_led.h"
#include "display.h"
#include "bsp_display_run.h"
#include "bsp_key.h"

RUN_T run_t;

uint8_t power_on_off_flag;

void Power_Off(void);





/**********************************************************************************************************
**
*Function Name:static void notifyStatusToHost(uint8_t lightNum,uint8_t filterNum,uint8_t unionNum)
*Function : 
*Input Ref:lightNum--LED ,filterNum -filter number, unionNum - smart menu number
*Return Ref:NO
*
*********************************************************************************************************/
void Decode_Handler(void)
{
   if(run_t.decodeFlag ==1){
   run_t.decodeFlag =0;
   Receive_MainBoard_Data_Handler(run_t.rx_mb_data_tag);
   }
  
}

/**********************************************************************
	*
	*Functin Name: void Display_DHT11_Value(void)
	*Function : Timer of key be pressed handle
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
void Power_Off(void)
{
    	 if(run_t.gPower_On ==RUN_POWER_OFF){

			Smg_AllOff();
            SMG_POWER_OFF()	;
			run_t.gPlasma=0;
			run_t.gDry=0;
		
			
		}
	}

/**********************************************************************
*
*Functin Name: void Receive_MainBoard_Data_Handler(run_t.single_data)
*Function : run order from main command 
*Input Ref:  run_t.single_dat is command 
*Return Ref: NO
*
**********************************************************************/
void Receive_MainBoard_Data_Handler(uint8_t cmd)
{
	
	static uint8_t hum1,hum2,temp1,temp2; 
	
    switch(cmd){

      case PANEL_DATA://0X02
	   
        if(run_t.gPower_On ==RUN_POWER_ON){
        hum1 =  run_t.gReal_humtemp[0]/10 ;  //Humidity 
        hum2 =  run_t.gReal_humtemp[0]%10;
        
        temp1 = run_t.gReal_humtemp[1]/10 ;  // temperature
        temp2 = run_t.gReal_humtemp[1]%10;

         //temperature 
         TM1639_Write_2bit_TempData(temp1,temp2); 
	      TM1639_Write_2bit_HumData(hum1,hum2);
	      
        }


       cmd =0xff;
       run_t.rx_mb_data_tag = 0xff;
      break;

	  case ANSWER_DATA:
          
      
		 
	 
       

	  cmd =0xff;
      run_t.rx_mb_data_tag = 0xff;

	  break;

	  case ORDER_DATA:

	   switch(outputBuf[0]){

         case PTC_WARNING:
		 	run_t.ptc_warning =1;
		 	run_t.gDry =0;

	     break;

		 case FAN_WARNING:
		 	run_t.fan_warning =1;
		 	

		 break;

		 
		 case SLAVE_PTC_WARNING:
			run_t.slave_ptc_warning =1;
		 
		 break;
		 
		 case SLAVE_FAN_WARNING :
			run_t.slave_fan_warning =1;
		 
		 break;

		 case FAN_REMOVE_WARNING:

		  // run_t.fan_warning =0;

		 break;

		 case WIFI_POWER_ON:
		 	run_t.gRunCommand_label =RUN_POWER_ON;

		 break;
		 case WIFI_POWER_OFF:
			run_t.gRunCommand_label =RUN_POWER_OFF;
		 break;

		 case WIFI_KILL_ON:

			run_t.gPlasma =1;

		 break;

		 case WIFI_KILL_OFF:
		    run_t.ai_model_flag =NO_AI_MODE;
			run_t.gPlasma = 0;
		
		 break;

		 case WIFI_PTC_OFF:
		    run_t.ai_model_flag =NO_AI_MODE;
			run_t.gDry =0;
			
		 break;

		 case WIFI_PTC_ON:
		      run_t.gDry =1;
		 break;

		 case WIFI_SONIC_ON:
              run_t.gUltrasonic =1;
		 break;

		 case WIFI_SONIC_OFF:
		   run_t.ai_model_flag =NO_AI_MODE;
		   run_t.gUltrasonic =0;
			
		 break;

		
		}

	  break;

   }

}

/**********************************************************************
*
*Functin Name: void Receive_ManiBoard_Cmd(uint8_t cmd)
*Function :  wifi recieve data
*Input Ref:  receive wifi send order
*Return Ref: NO
*
**********************************************************************/
void Power_On_Fun(void)
{
                
   static uint8_t hour_decade,hour_unit,minutes_one,minutes_two;

  
	
		run_t.gPlasma=1;
		run_t.gDry =1;
		run_t.gUltrasonic =1;
		run_t.ai_model_flag =AI_MODE;
		

	 run_t.gPower_On=RUN_POWER_ON;
	 Power_ON_Led();
	
	SMG_POWER_ON(); //WT.EDIT 2023.03.02

	
	run_t.gTimer_time_colon =0;
	run_t.set_temperature_decade_value=40;

	
	run_t.ptc_warning =0; 
	run_t.fan_warning =0;
	run_t.slave_ptc_warning =0; 
	run_t.slave_fan_warning =0;


	run_t.power_on_send_to_mb_times=36;
	run_t.timer_timing_define_ok=0;

  
   

    run_t.time_led_flag=1;
	



	run_t.works_dispTime_hours=0;
	run_t.works_dispTime_minutes=0;
	run_t.gTimes_time_seconds =0;
	
    run_t.gTimer_Counter=0;
	run_t.timer_dispTime_hours =0;
	run_t.timer_dispTime_minutes=0;
		
        

	  hour_decade=0;
	  hour_unit=0;
    
	  minutes_one = 0;
      minutes_two = 0;
	  
      


	  run_t.hours_two_unit_bit = hour_unit;
	  run_t.minutes_one_decade_bit =  minutes_one;
      
	 TM1639_Write_4Bit_Time(hour_decade,run_t.hours_two_unit_bit,run_t.minutes_one_decade_bit,minutes_two,0);
     Display_DHT11_Value();
    
}


/************************************************************************
	*
	*Function Name: static void Power_Off_Fun(void)
	*
	*
	*
	*
************************************************************************/
void Power_Off_Fun(void)
{
	
 
		run_t.gPlasma=0;
		run_t.gDry =0;
		run_t.gUltrasonic =0;
		
        run_t.gPower_On =RUN_POWER_OFF;
		run_t.gTimer_set_temp_times=0; //conflict with send temperatur value 
       Power_Off_Led_Off();

  
} 

