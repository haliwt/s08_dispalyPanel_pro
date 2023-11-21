#include "bsp_display_run.h"
#include "bsp.h"




void (*single_ai_fun)(uint8_t cmd);
void (*single_add_fun)(void);
void (*single_buzzer_fun)(void);
void (*sendAi_usart_fun)(uint8_t senddat);
void (*dispose_key)(uint8_t dsdat);
void (*display_fan_speed_value)(uint8_t fan_level);

static void Led_Panel_OnOff(void);
static void Display_SmgTiming_Value(void);
static void RunLocal_Dht11_Data_Process(void);

static void DisplayPanel_DHT11_Value(void);
static void Display_SetTemperature_Value(void);
static void Display_Works_Time_Fun(void);
static void WorksTime_DonotDisplay_Fun(void);
static void Timer_Timing_Donot_Display(void);


static void Display_Alternate_Slave_Fault_Item(void);



uint8_t power_adjust_on, power_adjust_off;

/******************************************************************************
*
*Function Name:static void Setup_Timer_Times(void)
*Funcion:display setup timer times  //__asm("NOP");//等待1个指令周期，系统主频24M
*Iinput Ref:NO
*Return Ref:NO
*
******************************************************************************/
static void Display_SmgTiming_Value(void)
{

   static uint8_t timer_display_flag, alternate_flag;
	switch(run_t.timer_timing_define_flag){

	case timing_success: //0x01

	   switch(run_t.timer_timing_define_ok){

	   case 1:
	   if(run_t.gTimer_Counter > 59){
	    run_t.gTimer_Counter =0;
		timer_display_flag=1;
		run_t.timer_dispTime_minutes -- ;
	
	    if(run_t.timer_dispTime_minutes <  0 ){
			 
		   run_t.timer_dispTime_hours -- ;
		   run_t.timer_dispTime_minutes =59;
         }

		
		
		 if(run_t.timer_dispTime_hours < 0 ){
		 
			run_t.gTimer_Counter = 57 ;
			run_t.timer_dispTime_hours=0;
			run_t.timer_dispTime_minutes=0;
			run_t.timer_timing_define_flag=timing_power_off;
			
			
	      }
		    
	    break;

		case 0: //NO_AI_MODE by timer timing  auto be changed AI_MODE
			
			if(run_t.gTimer_Counter > 59){
	          run_t.gTimer_Counter =0;

			
		     run_t.timer_works_transform_flag=0;
			
             run_t.ai_model_be_changed_flag =  NO_AI_TO_AI_MODE;

			 /******************************************************/

			  if(run_t.gDry == 0)

			     run_t.gDry = 1;

                SendData_Set_Command(PLASM_ON_NO_BUZZER); //PTC turn On --this is error code 2023.10.10

                HAL_Delay(1);
			  }

		
             run_t.timer_timing_define_flag=timing_donot; 

             run_t.ai_model_flag =AI_MODE; 
             run_t.gDry=1;
             run_t.gPlasma=1;
			 run_t.set_temperature_flag=0;  //WT.EDIT 20230.09.23
			 run_t.timer_timing_define_ok=0xff; //WT.EDIT.2023.09.21 has a little bug.

            
         
		    }

		break;
	   	}
    

    
	
      if(run_t.ptc_warning ==0 && run_t.fan_warning ==0 && run_t.slave_fan_warning == 0 && run_t.slave_ptc_warning ==0){
		   if(timer_display_flag==1 ||  run_t.timer_works_transform_flag ==1){
			   timer_display_flag=0;
			   run_t.timer_works_transform_flag=0;

	           Display_GMT(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);

		   }
		 
	      
      }
	  else if(run_t.ptc_warning ==1 || run_t.fan_warning ==1 ){

	     if(run_t.gTimer_error_digital < 60){//10ms * 60= 600ms

		        if(alternate_flag ==0){
			   	
                     
			     if(run_t.ptc_warning ==1){
                 
					Display_Host_Error_Digital(0x01,0);
			     }
				 else {
			        if(run_t.fan_warning ==1){

					  
                      Display_Host_Error_Digital(0x02,0);

			        }

				 }
			    

               }
			   else{

			      alternate_flag=2;
				   if(run_t.ptc_warning ==1 && run_t.fan_warning ==1){

					     Display_Host_Error_Digital(0x02,0);

				   	}
				    else  if(run_t.ptc_warning ==1 && run_t.fan_warning ==0){
                       
					    Display_Host_Error_Digital(0x01,0);
			        }
					else  if(run_t.ptc_warning ==0 && run_t.fan_warning ==1){
                       
					    Display_Host_Error_Digital(0x02,0);
					    if(alternate_flag==2 ||alternate_flag>2 )alternate_flag=0;
			        }


			   }
			   

		   }
		   else if(run_t.gTimer_error_digital > 59 && run_t.gTimer_error_digital < 121 ){

                  alternate_flag++;
 				Display_Host_Error_Digital(0x10,1);
 				if( alternate_flag==2) alternate_flag=0;
		   }
		   else if(run_t.gTimer_error_digital > 119){

			  run_t.gTimer_error_digital=0;


			 }


	  }

	   WorksTime_DonotDisplay_Fun();
	   
	break;


	case timing_power_off:
        SendData_Power_OnOff(0);
		HAL_Delay(5);
	
	  run_t.gRunCommand_label = RUN_POWER_OFF;//POWER_OFF_PROCESS; //POWER_OFF_PROCESS ;
	  run_t.timer_timing_define_flag = 0xff;

	break;


	case timing_donot:
		
        Timer_Timing_Donot_Display();
		
	    Display_Works_Time_Fun();
    break;

	}
    
 }

/******************************************************************************
*
*Function Name:void Single_RunCmd(void)
*Funcion: handle of receive by usart data
*
*
******************************************************************************/
static void DisplayPanel_DHT11_Value(void)
{
  if(run_t.first_power_on_times==1){
      run_t.first_power_on_times++;
       Display_DHT11_Value();
  
  }
  if(run_t.gTimer_display_dht11 > 32 && run_t.set_temperature_flag==0){
	    run_t.gTimer_display_dht11=0;
       	Display_DHT11_Value();
     
	}
}

/******************************************************************************
	*
	*Function Name:void RunPocess_Command_Handler(void)
	*Funcion: display pannel run of process 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void RunPocess_Command_Handler(void)
{
   static uint8_t power_off_set_flag;
   static uint8_t  step_state;
   switch(run_t.gRunCommand_label){

      case RUN_POWER_ON:

            power_adjust_on = Power_ReadParam_OnOff(1);

		    if(power_adjust_on ==1){
		   	    power_adjust_on =0;
	 			run_t.gTimer_set_temp_times=0; //conflict with send temperatur value
	 		
	          	Power_On_Fun();
			    run_t.gRunCommand_label= UPDATE_DATA;

		    }
           
              
	break;

	  case RUN_POWER_OFF://2

	       power_adjust_off = Power_ReadParam_OnOff(0);

		    if(power_adjust_off ==1){
	          power_adjust_off=0;
		   
		   run_t.ptc_warning =0;
		    run_t.fan_warning =0;
			
            power_off_set_flag=0;
		   
		    Power_Off_Fun();
            run_t.gRunCommand_label =POWER_OFF_PROCESS;

         }
            
        
	  break;


	  case UPDATE_DATA: //3

	           switch(run_t.display_timer_timing_flag){

                case 1:
					
                    run_t.display_timer_timing_flag=0;
                
					TM1639_Write_4Bit_Time(run_t.hours_two_decade_bit,run_t.hours_two_unit_bit, run_t.minutes_one_decade_bit,run_t.minutes_one_unit_bit,0) ; //timer is defau
                     SendData_Buzzer();
                     HAL_Delay(2);

                

                break;

				case 2:
					
					run_t.display_timer_timing_flag=0;
                    
                    TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);
                    SendData_Buzzer();
                    HAL_Delay(2);

                     
				break;

                case 0:

				    switch(step_state){

					case 0:
						Led_Panel_OnOff();
					    step_state=1;
					break;

					case 1:
               			RunLocal_Dht11_Data_Process();
				        step_state=2;
	                    
				   break;

					case 2:
                      
                      Set_Timing_Temperature_Number_Value();

                      step_state=3;
					break;

					case 3:
                       
					       Display_SmgTiming_Value();
                   
					    step_state=4;

					break;

					case 4:

	                    Display_SetTemperature_Value(); 
						step_state=5;

					break;

					case 5:
                  
                         if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
                                Display_TimeColon_Blink_Fun();
                         }
                       

                      
                       step_state=6;
                    break;

					case 6:  //display slave of machine fan and ptc fault .
					
							if(run_t.slave_ptc_warning ==1 || run_t.slave_fan_warning ==1){
								Display_Alternate_Slave_Fault_Item();
							}
									
					 step_state=0;
					
				   break;

				
				   }
                break;
                    
            case 4:
                run_t.gRunCommand_label= POWER_OFF_PROCESS;
                Power_Off_Fun();
            break;
          }

	
     break;

	  case POWER_OFF_PROCESS://4

	   if(run_t.gPower_On ==RUN_POWER_OFF && run_t.gRunCommand_label ==POWER_OFF_PROCESS){
	   	     if(power_off_set_flag==0){
					power_off_set_flag++;
                   Power_Off_Fun();
				
	   	   }
					Breath_Led();
		 
		
       }

	  break;

  }
}
/*******************************************************
	*
	*Function Name: static void RunLocal_Dht11_Data_Process(void)
	*Function : display pannel display conetent
	*
	*
	*
*******************************************************/
static void RunLocal_Dht11_Data_Process(void)
{
   DisplayPanel_DHT11_Value();

}
static void Led_Panel_OnOff(void)
{
	Panel_Led_OnOff_Function() ;
}
/*******************************************************
	*
	*Function Name: static void RunLocal_Dht11_Data_Process(void)
	*Function : display pannel display conetent
	*
	*
	*
*******************************************************/
static void Display_SetTemperature_Value(void)
{
	
    static uint8_t set_temperature_value;
    if(run_t.temperature_set_flag ==1 && run_t.gTimer_temp_delay >60){
               run_t.gTimer_temp_delay =0;
		 
		  set_temperature_value= run_t.set_temperature_decade_value*10+ run_t.set_temperature_unit_value;
		  if(set_temperature_value <= run_t.gReal_humtemp[1] || run_t.gReal_humtemp[1] >40){//envirment temperature
	  
				run_t.gDry = 0;
                
		        SendData_Set_Command(DRY_OFF_NO_BUZZER);//PTC turn off
			    
			    
                
		  }
		  else if((set_temperature_value -3) >= run_t.gReal_humtemp[1]){


            if(run_t.ai_model_flag == NO_AI_MODE && run_t.manual_dry_turn_off ==0){

                run_t.gDry = 1;
                SendData_Set_Command(DRY_ON_NO_BUZZER); //PTC turn On


            }
            else if(run_t.manual_dry_turn_off ==0){
            
    		     run_t.gDry = 1;
    	         SendData_Set_Command(DRY_ON_NO_BUZZER); //PTC turn On
             
               
             }
		  }
	  
	    
	  }
    else{ //no define set up temperature value 
		if(run_t.gReal_humtemp[1] >40 && run_t.gTimer_temp_delay >119){//envirment temperature
			run_t.gTimer_temp_delay =0;
			run_t.gDry = 0;
			run_t.auto_model_shut_off_ptc_flag =1;
			SendData_Set_Command(DRY_OFF_NO_BUZZER);

		}
        else if(run_t.gReal_humtemp[1] < 38 && run_t.auto_model_shut_off_ptc_flag ==1 &&  run_t.gTimer_temp_delay >119){
			run_t.gTimer_temp_delay =0;
			if(run_t.ai_model_flag == AI_MODE){
			run_t.gDry = 1;
			SendData_Set_Command(DRY_ON_NO_BUZZER); //PTC turn On

           }
             
          }
			    
      }

}


static void Display_Works_Time_Fun(void)
{
     static uint8_t works_timing_flag,alternate_flag;

	 if(run_t.ptc_warning ==0 && run_t.fan_warning ==0 && run_t.slave_fan_warning ==0 && run_t.slave_ptc_warning ==0){
     if(run_t.gTimes_time_seconds > 59 ){
            run_t.gTimes_time_seconds=0;
            works_timing_flag =1;
			run_t.works_dispTime_minutes++; //1 minute 
		
            run_t.send_app_wokes_minutes_two++;
			if(run_t.works_dispTime_minutes> 59){ //1 hour
			run_t.works_dispTime_minutes=0;
			run_t.works_dispTime_hours++;
			if(run_t.works_dispTime_hours > 99){ //WT.edit times over 99hours 2023.09.20
			run_t.works_dispTime_hours =0;
			}
        
            }

     	}
       
	       if(works_timing_flag==1 || run_t.timer_works_transform_flag ==0 ){
	          works_timing_flag=0;
			  run_t.timer_works_transform_flag=1;
			 Display_GMT(run_t.works_dispTime_hours,run_t.works_dispTime_minutes);
		  
	        }
        }
		else if(run_t.ptc_warning ==1 || run_t.fan_warning ==1){

		    if(run_t.gTimer_error_digital < 60){//10ms * 51= 510

		      
               if(alternate_flag ==0){
			   	  
                     
			     if(run_t.ptc_warning ==1){
                 
					Display_Host_Error_Digital(0x01,0);
			     }
				 else {
			        if(run_t.fan_warning ==1){

					  
                      Display_Host_Error_Digital(0x02,0);

			        }

				 }
			    

               }
			   else{

			      alternate_flag=2;
				   if(run_t.ptc_warning ==1 && run_t.fan_warning ==1){

					     Display_Host_Error_Digital(0x02,0);

				   	}
				    else  if(run_t.ptc_warning ==1 && run_t.fan_warning ==0){
                       
					    Display_Host_Error_Digital(0x01,0);
			        }
					else  if(run_t.ptc_warning ==0 && run_t.fan_warning ==1){
                       
					    Display_Host_Error_Digital(0x02,0);
			        }


			   }
			   

		   }
		   else if(run_t.gTimer_error_digital > 59 && run_t.gTimer_error_digital  < 121 ){
		   		alternate_flag++;
 				Display_Host_Error_Digital(0x10,1);
 				if(alternate_flag==2 ||alternate_flag>2 )alternate_flag=0;
		   }
		    else if(run_t.gTimer_error_digital > 119){

			  run_t.gTimer_error_digital=0;


			 }

		}



}

/****************************************************************
 * 
 * Function Name: static void Timer_Timing_Donot_Display(void)
 * Function :function of pointer 
 * Input Ref:NO
 *
 * 
*****************************************************************/
static void Timer_Timing_Donot_Display(void)
{
     
 
     if(run_t.gTimer_Counter > 59 && run_t.timer_timing_define_ok==1){
	    run_t.gTimer_Counter =0;
		run_t.timer_dispTime_minutes -- ;
	
	    if(run_t.timer_dispTime_minutes <  0 ){
			 
		   run_t.timer_dispTime_hours -- ;
		   run_t.timer_dispTime_minutes =59;
         }

		
		
		if(run_t.timer_dispTime_hours <0){ 
				run_t.gTimer_Counter = 57 ;
				run_t.timer_dispTime_hours=0;
				run_t.timer_dispTime_minutes=0;
				run_t.timer_timing_define_flag=timing_power_off;
			}
			
        }



}

/****************************************************************
 * 
 * Function Name: static void WorksTime_DonotDisplay_Fun(void)
 * Function :function of pointer 
 * 
 *
 * 
****************************************************************/
static void WorksTime_DonotDisplay_Fun(void)
{
//send to APP works times every minute onece
   if(run_t.gTimes_time_seconds > 59 && run_t.timer_timing_define_flag ==timing_success && run_t.temp_set_timer_timing_flag ==0){
		   run_t.gTimes_time_seconds=0;
		 
		   run_t.works_dispTime_minutes++; //1 minute 
		
		   run_t.send_app_wokes_minutes_two++;
		   if(run_t.works_dispTime_minutes> 59){ //1 hour
		   run_t.works_dispTime_minutes=0;
		   run_t.works_dispTime_hours++;
		   if(run_t.works_dispTime_hours > 99){ //100 hours
		   run_t.works_dispTime_hours =0;
		   }
	       }
  }
//	while(run_t.send_works_times_to_app==1){
//		   run_t.send_works_times_to_app=0;
//	   SendData_Works_Time(run_t.send_app_wokes_minutes_one ,run_t.send_app_wokes_minutes_two);
//	   }
}


/****************************************************************
 * 
 * Function Name:
 * Function :function of pointer 
 * 
 * 
****************************************************************/
static void Display_Alternate_Slave_Fault_Item(void)
{
   static uint8_t alternate_flag;

	if(run_t.gTimer_slave_fault_times  < 60){//10ms * 51= 510

		      
               if(alternate_flag ==0){
			   	  
                     
			     if(run_t.slave_ptc_warning ==1){
                 
					Display_Slave_Error_Digital(0x01,0);
			     }
				 else {
			        if(run_t.slave_fan_warning ==1){

					  
                      Display_Slave_Error_Digital(0x02,0);

			        }

				 }
			    

               }
			   else{

			      alternate_flag=2;
				   if(run_t.slave_ptc_warning ==1 && run_t.slave_fan_warning ==1){

					     Display_Slave_Error_Digital(0x02,0);

				   	}
				    else  if(run_t.slave_ptc_warning ==1 && run_t.slave_fan_warning ==0){
                       
					    Display_Slave_Error_Digital(0x01,0);
			        }
					else  if(run_t.slave_ptc_warning ==0 && run_t.slave_fan_warning ==1){
                       
					    Display_Slave_Error_Digital(0x02,0);
			        }


			   }
			   

		   }
		   else if(run_t.gTimer_slave_fault_times > 59 && run_t.gTimer_slave_fault_times < 112 ){
		   		alternate_flag++;
 				//Display_Slave_Error_Digital(0x10,0);
 				if(alternate_flag==2 ||alternate_flag>2 )alternate_flag=0;
		   }
		    else if(run_t.gTimer_slave_fault_times> 119){

			  run_t.gTimer_slave_fault_times=0;

			 }
}
/****************************************************************
 * 
 * Function Name:
 * Function :function of pointer 
 * 
 * 
****************************************************************/
void Single_Add_RunCmd(void(*addHandler)(void))
{
    single_add_fun = addHandler;   

}

void Single_SendBuzzer_RunCmd(void(*buzzerHandler)(void))
{
	single_buzzer_fun = buzzerHandler;

}
void Single_SendAi_Usart_RunCmd(void(*sendaiHandler)(uint8_t seddat))
{
    sendAi_usart_fun = sendaiHandler;

}

void SplitDispose_Key_RunCmd(void(*keyHandler)(uint8_t dat))
{

  dispose_key = keyHandler;

}

void Smg_DisplayFan_Leve(void(*fandisplayHandler)(uint8_t fan_level))
{
   display_fan_speed_value = fandisplayHandler;

}



