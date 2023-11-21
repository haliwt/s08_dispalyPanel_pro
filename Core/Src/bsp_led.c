#include "bsp_led.h"
#include "run.h"
#include "bsp_key.h"
#include "delay.h"
#include "cmd_link.h"


volatile uint32_t led_k,led_i;


void (*panel_led_fun)(void);





static void TIME_LED_OnOff(uint8_t sel);

static void Power_Breath_Two(void);

static void Delay(int16_t count);


/***********************************************************
*
*Function Name : static void DRY_LED_OnOff(uint8_t sel)
*
*
*
************************************************************/
void DRY_LED_OnOff(uint8_t sel)
{
   if(sel==1){
	 LED_DRY_ON();
   }
   else 
   	LED_DRY_OFF();

}

void PLASMA_LED_OnOff(uint8_t sel)
{
	if(sel==1){
		LED_PLASMA_ON();
   }
   else
   	 LED_PLASMA_OFF();

}

static void TIME_LED_OnOff(uint8_t sel)
{
    if(sel==1)LED_TIME_ON();
	else  LED_TIME_OFF();


}

void KeyLed_Power_On(void)
{
   LED_POWER_ON();
   LED_TIME_ON();


}

void ALL_LED_Off(void)
{
  LED_PLASMA_OFF();
  LED_AI_OFF();
  LED_DRY_OFF();
  LED_TIME_OFF();
  LED_BUG_OFF();


}

/*******************************************************************************************
 	*
 	* Function Name:void Panel_Led_OnOff_Function(void)
 	* Function :panel of led turn on or off which key function 
 	* Input Ref:NO
 	* Return Ref:NO
 	* 
*******************************************************************************************/  
void Panel_Led_OnOff_Function(void)
{

   
   if(run_t.gTimer_run_ico > 30){ //30* 10ms =300ms
		run_t.gTimer_run_ico=0;
		
		LED_POWER_ON();

		
	   if(run_t.ai_model_flag ==AI_MODE){
         LED_AI_ON();
        
      }
	  else{

			LED_AI_OFF() ;
      }
	 
	  
    if(run_t.gDry==1){
		 
	     DRY_LED_OnOff(1);
         if(run_t.ai_model_be_changed_flag == NO_AI_TO_AI_MODE){
            run_t.ai_model_be_changed_flag++;
          	SendData_Set_Command(DRY_ON_NO_BUZZER); //PTC turn On
                 
        }
      

     }
	 else{
	    DRY_LED_OnOff(0);

	 }

	 if(run_t.gPlasma==1){
	 	
	     PLASMA_LED_OnOff(1);
     }
	 else{
	   PLASMA_LED_OnOff(0);

	 }

	 if(run_t.gUltrasonic ==1){
         LED_BUG_ON();


	 }
	 else{
		 LED_BUG_OFF();


	 }

	  if(run_t.time_led_flag ==1){
	    TIME_LED_OnOff(1);
	 }
	 else
	 	TIME_LED_OnOff(0);
      }
}
/***************************************************************
*
*Function Name: void LED_TempHum(void)
*Function : display temperature and humidity and times led 
*
*
*
**************************************************************/
static void Power_Breath_Two(void)
{
    static uint32_t i,j;
    led_k++;

	if(led_k<2001){
        i=0;
        j=0;
		LED_POWER_ON();
		Delay(led_k);
	   LED_POWER_OFF();
	   Delay(8000-led_k);

    }
    if(led_k>1999 && led_k <4001){
        j++;
		LED_POWER_OFF();
		Delay(j);
        LED_POWER_ON();
        Delay(2000-j);
        

    }
    else if(led_k>3999 && led_k <6001){
		led_i++;

	  
	   LED_POWER_ON();
       LED_POWER_OFF();
	   Delay(2300+led_i);
      

	}
    else if(led_k>5999){
        led_k =30000;
        i++; 
      if(i<50000){
          LED_POWER_OFF();
      }
      else{
        led_i=0;
		led_k=0;
        
        
      }
	}
	

}

static void Delay(int16_t count)
{
   
    if(count ==0 || count <0){
       return;
    
    }
    else{
    while(count){

       count--;
   }
   }

}

#if 0
static void Power_BreathOnOff(void)
{
    
     static uint32_t k;
   
    k++;

	if(k<40000){

	LED_POWER_ON();

    }
          

	if(k>40000 && k <80000){

	    LED_POWER_OFF();

	}
	if(k>80000 && k< 120000){

	  LED_POWER_TOGGLE();


	}

	if(k>120000){

        k=0;
	}

}

#endif 


void Breath_Led(void)
{
    
   
   // Power_BreathOnOff();
   Power_Breath_Two();


   
}



/***************************************************************
*
*Function Name: void Panel_Led_OnOff_RunCmd(void (*panelledHandler)(void))
*Function : display temperature and humidity and times led 
*
*
*
**************************************************************/
void Panel_Led_OnOff_RunCmd(void (*panelledHandler)(void))
{

	 panel_led_fun= panelledHandler;
}

void Power_Off_Led_Off(void)
{
   LED_AI_OFF();
   LED_DRY_OFF();
   LED_TIME_OFF();
   LED_PLASMA_OFF();
   LED_BUG_OFF();
   LED_POWER_OFF();
   SMG_POWER_OFF();

}

void Power_ON_Led(void)
{
   LED_AI_ON();
   LED_DRY_ON();
   LED_TIME_ON();
   LED_PLASMA_ON();
   LED_BUG_ON();
   LED_POWER_ON();
   SMG_POWER_ON();

}
