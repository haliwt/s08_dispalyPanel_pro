#include "bsp_key.h"
#include "bsp.h"



key_types key_t;

/***********************************************************
*
*
*
*
*
***********************************************************/
#if 0
uint8_t KEY_Scan(void)
{
  uint8_t  reval = 0;
  key_t.read = _KEY_ALL_OFF; //0xFF 


    if(AI_KEY_VALUE() ==1 ) //WIFI_KEY_ID = 0x80
	{
		key_t.read &= ~0x80; // 0x1f & 0x7F =  0x7F
	}
    else if(BUG_KEY_VALUE()   ==1 ) //FAN_KEY_ID = 0x10
	{
		  key_t.read &= ~0x10; // 0xFf & 0xEF =  0xEF
	}
	else if(PLASMA_KEY_VALUE()   ==1 ) //PLASMA_KEY_ID = 0x20
	{
		  key_t.read &= ~0x20; // 0xFf & 0xDF =  0xDF
	}
	else if(DRY_KEY_VALUE()  ==1 ) //DRY_KEY_ID = 0x40
	{
		  key_t.read &= ~0x40; // 0xFf & 0xBF =  0xBF
	}

	
	
   
    switch(key_t.state )
	{
		case start:
		{
			if(key_t.read != _KEY_ALL_OFF)
			{
				key_t.buffer   = key_t.read; //??:key.buffer = 0xFE  POWER KEY 
				key_t.state    = first;
				key_t.on_time  = 0;
				key_t.off_time = 0;
            
                
			}
			break;
		}
		case first:
		{
			if(key_t.read == key_t.buffer) //  short  key be down ->continunce be pressed key
			{
				if(++key_t.on_time>35 )//25 //10000  0.5us
				{
					//run_t.power_times++;
                    key_t.value = key_t.buffer^_KEY_ALL_OFF; // key.value = 0xFE ^ 0xFF = 0x01
					key_t.on_time = 0;                      //key .value = 0xEF ^ 0XFF = 0X10
                    key_t.state   = second;
                   
                    
				}
			  
			}
			else
			{
				key_t.state   = start;
			}
			break;
		}
		case second:
		{
			if(key_t.read == key_t.buffer) //long key key if be pressed down 
			{
				if(++key_t.on_time>70000)// 80000 long key be down
				{
				    key_t.value = key_t.value|0x80; //key.value = 0x02 | 0x80  =0x82
                    key_t.on_time = 0;
					key_t.state   = finish;
	               
				}
					
			}
			else if(key_t.read == _KEY_ALL_OFF)  // loose hand 
			{
					if(++key_t.off_time> 0) //20//30 don't holding key dithering
					{
						key_t.value = key_t.buffer^_KEY_ALL_OFF; // key.value = 0x1E ^ 0x1f = 0x01
						
						key_t.state   = finish; // loose hand
					}
			}
		   
			break;
		}
		case finish:
		{
		
			reval = key_t.value; // is short time  TIMER_KEY = 0x01  2. long times TIMER_KEY = 0X81

			key_t.state   = end;
         
			break;
		}
		case end:
		{
			if(key_t.read == _KEY_ALL_OFF)
			{
				if(++key_t.off_time>0)//5//10//50 //100
				{
					key_t.state   = start;
                  
				}
			}
			break;
		}
		default:
		{
			key_t.state   = start;
         
			break;
		}
	}
	return  reval;


}

#endif 
/************************************************************************
	*
	*Function Name: void Process_Key_Handler(uint8_t keylabel)
	*Function : key by pressed which is key numbers process 
	*Input Ref: key be pressed value 
	*Return Ref:No
	*
************************************************************************/
void Process_Key_Handler(uint8_t keylabel)
{
 
   static uint8_t power_adjust_on,power_adjust_off;

  	switch(keylabel){

      case POWER_KEY_ID:
	 
           if(run_t.gPower_On == RUN_POWER_ON){
	      

		    power_adjust_on = Power_ReadParam_OnOff(1);

		    if(power_adjust_on ==1){
		   	
	 			run_t.gTimer_set_temp_times=0; //conflict with send temperatur value
	 		
	            run_t.gRunCommand_label =RUN_POWER_ON;
			    run_t.keyvalue = 0xff;

		    }
           
              
		 }
		 else if(run_t.gPower_On == RUN_POWER_OFF){

		     power_adjust_off = Power_ReadParam_OnOff(0);

		    if(power_adjust_off ==1){
	        
			    run_t.gRunCommand_label =RUN_POWER_OFF;
		   
		    }
			  run_t.keyvalue = 0xff;
		 }
	  	 
	   run_t.keyvalue = 0xff;

	  break;

	  case MODE_LONG_KEY_ID:
	  	if(run_t.gPower_On ==RUN_POWER_ON){

		  if(run_t.ptc_warning==0 && run_t.fan_warning ==0){
		  	  

		        
				  run_t.temp_set_timer_timing_flag= TIMER_TIMING;
			      run_t.gTimer_key_timing=0;
				

                  run_t.timer_timing_define_ok=0; //WT.EDIT 2023.09.15
                  run_t.judge_hours_if_zero =0;
                  run_t.judge_minutes_if_zero =0;
				  run_t.set_temperature_flag=0;  //WT.EDIT 20230.09.23
                 	SendData_Buzzer();//single_buzzer_fun();
                    HAL_Delay(2);
			  
          }
	       
		 }
	  	
		 run_t.keyvalue = 0xff;
	  break;

	  case AI_KEY_ID:
        
        if(run_t.gPower_On ==RUN_POWER_ON){

		if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
         switch(run_t.temp_set_timer_timing_flag){

              case 0:
                   
					if(run_t.ai_model_flag ==AI_MODE){
						run_t.ai_model_flag =NO_AI_MODE;
						 SendData_Set_Command(AI_MODE_OFF);
                         //HAL_Delay(5);
                        
					run_t.timer_timing_define_flag=timing_timer_model;
					run_t.gTimer_Counter=0;
				

					}
					else{
						if(run_t.ai_model_flag ==NO_AI_MODE){
							run_t.ai_model_flag =AI_MODE;
							SendData_Set_Command(AI_MODE_ON);
	                       // HAL_Delay(5);
						 
	                       if(run_t.ptc_warning ==0 && run_t.fan_warning==0){
	                         run_t.gDry= 1;
							 run_t.gUltrasonic =1; //WT.EDIT.2023.11.22

	                       	}
	                        run_t.gPlasma = 1;

	                       	
	                        
	                        run_t.manual_dry_turn_off=0;
						    run_t.timer_timing_define_flag=timing_works_model;
					   }

					}
                
			
			
			break;

			case 1://set timer timing numbers 
				
		
		    if(run_t.judge_hours_if_zero >0 || run_t.judge_minutes_if_zero >20){
			     SendData_Buzzer();
                 HAL_Delay(1);
				 run_t.timer_timing_define_flag=timing_timer_model;
				 run_t.timer_timing_define_ok = 1;
				 run_t.temp_set_timer_timing_flag=0;
                 run_t.ai_model_flag =NO_AI_MODE;
                
              
		    }
             else{

				run_t.ai_model_flag =AI_MODE;
			
		
				SendData_Buzzer();
                  HAL_Delay(1);

                  run_t.gDry=1;
                  run_t.gPlasma=1;
				  run_t.gUltrasonic =1; //WT.EDIT.2023.11.22
				  
				  run_t.timer_timing_define_flag=timing_works_model;
					
					run_t.timer_works_transform_flag =0; //at once display AI mode 
					
					run_t.timer_timing_define_ok = 0;

					run_t.temp_set_timer_timing_flag=0;
               
                

				}
					
		    	run_t.gTimer_Counter=0;

				
			

			break;

            }	

		   }
		}
	if(run_t.gPower_On ==RUN_POWER_ON){
	   if(run_t.ai_model_flag ==NO_AI_MODE){
			LED_AI_OFF();
	   }
	   else{
			LED_AI_ON();
	   }
	  }
	   run_t.keyvalue = 0xFF;

	  break;

      case MODEL_KEY_ID://model_key: AI_mode to on_AI_mode
          if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
            if(run_t.ai_model_flag ==AI_MODE){
			run_t.ai_model_flag =NO_AI_MODE;
			 SendData_Set_Command(AI_MODE_OFF);
         
             run_t.timer_timing_define_flag=timing_timer_model;
              run_t.gTimer_Counter=0;

			}
		    else{
				run_t.ai_model_flag =AI_MODE;
				SendData_Set_Command(AI_MODE_ON);
              
                run_t.timer_timing_define_flag=timing_works_model;
                 if(run_t.ptc_warning ==0){
                 run_t.gDry= 1;

               }
                run_t.gPlasma = 1;
     

			}

          }
         run_t.keyvalue = 0xFF;
      break;

	  default:
          
	  break;

	}
	//
	

}
/****************************************************************
	*
	*Function Name :void Set_Timing_Temperature_Number_Value(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void Set_Timing_Temperature_Number_Value(void)
{

    static uint8_t temp;
    static uint8_t set_temp_flag,counter_times;
	
	//set timer timing value 
	if(run_t.temp_set_timer_timing_flag == TIMER_TIMING){

	
	if(run_t.gTimer_key_timing > 5){
		run_t.gTimer_key_timing =0;		
		
	   // run_t.input_timer_timing_numbers_flag =0;
       
		 
		run_t.timer_dispTime_hours =0;
		run_t.timer_dispTime_minutes =0;

        if(run_t.judge_hours_if_zero > 0 || run_t.judge_minutes_if_zero >20){
            run_t.ai_model_flag =NO_AI_MODE;

        }

		run_t.timer_works_transform_flag =0;
        run_t.temp_set_timer_timing_flag=0;
	
		
        run_t.gTimer_Counter=0;
	
	 }
     //AI of led blink waiting select
	// if(run_t.input_timer_timing_numbers_flag ==1){
     else{
           if(run_t.gTimer_smg_timing < 26){
	              LED_AI_ON();
           	}
		   else if(run_t.gTimer_smg_timing > 24 && run_t.gTimer_smg_timing < 54){
		   	     LED_AI_OFF();
		   	
		   	}
		   else if(run_t.gTimer_smg_timing > 53){
			run_t.gTimer_smg_timing=0;

			
		   }

	}

	}
    else if(run_t.temp_set_timer_timing_flag==0){

	 if(run_t.set_temperature_flag ==set_temperature_value){

	  //waiting for 4 s 
	  if(run_t.gTimer_key_temp_timing > 3 && run_t.set_temperature_special_value ==0){
			set_temp_flag++;
			
			run_t.set_temperature_special_value =1;
			run_t.gTimer_set_temp_times =0; //couter time of smg blink timing 

	 }
	 //temperature of smg of LED blink .
	  if(run_t.set_temperature_special_value ==1){
	  	
	  	
		  if(run_t.gTimer_set_temp_times < 15 ){ // 4
		        TM1639_Write_2bit_SetUp_TempData(0,0,1);
          }
		  else if(run_t.gTimer_set_temp_times > 14 && run_t.gTimer_set_temp_times < 29){
		  	
			  TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);
			  

		  }
		  else{
		  	 run_t.gTimer_set_temp_times=0;
             counter_times++ ;  

		  }


           if(counter_times > 3){
			 
			 set_temp_flag=0;
		     counter_times=0;
			  run_t.temperature_set_flag =1;
			  run_t.set_temperature_special_value =0xff;
			  run_t.set_temperature_flag= 0; //WT.EDTI 2023.09.27
			  run_t.gTimer_temp_delay = 70; //at once shut down ptc  funciton
		
			  TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);

              temp =  run_t.set_temperature_decade_value*10 + run_t.set_temperature_unit_value;
			  SendData_Temp_Data(temp); //WT.EDIT 2023.10.10
			  HAL_Delay(2);
			  
	       
	       }
	     }
	 }
	}

}

/*********************************************************************
	*
	*Function Name:void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
	*Function : GPIO inetrrupt backcall function
	*Input Ref: interrupt GPIO special gpio
	*Return Ref: NO
	*
**********************************************************************/
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
 

   volatile static  uint8_t set_up_temperature_value;
  switch(GPIO_Pin){

     case POWER_KEY_Pin:

	  
	    run_t.gTimer_time_colon=0;
	   
	 	if(POWER_KEY_VALUE()  ==KEY_DOWN && run_t.power_times==1){
			

		if(run_t.gPower_On ==RUN_POWER_OFF){
				
		    run_t.gRunCommand_label =RUN_POWER_ON;
		        
		}
        else{
		   	
		  run_t.gRunCommand_label =RUN_POWER_OFF;
			
		}
				
       }
       __HAL_GPIO_EXTI_CLEAR_RISING_IT(POWER_KEY_Pin);
    
     break;

	 case MODEL_KEY_Pin:

	  
	
      if(run_t.gPower_On ==RUN_POWER_ON && MODEL_KEY_VALUE() ==1){

	      
          run_t.gTimer_time_colon=0;
          if(run_t.recoder_start_conuter_flag==0){
			run_t.recoder_start_conuter_flag++;
			run_t.gTimer_mode_key_start_counter=1;
			  run_t.gTimer_mode_key_counter=0;

		 }
		 if(run_t.gTimer_mode_key_counter> 3 || run_t.gTimer_mode_key_counter==3){
                        
		       run_t.keyvalue  = MODE_LONG_KEY_ID;
			   run_t.recoder_start_conuter_flag=0;	
		       run_t.gTimer_mode_key_start_counter=0;

		}


	    
		 

	  }

       __HAL_GPIO_EXTI_CLEAR_RISING_IT(MODEL_KEY_Pin);
	 break;

	 case DEC_KEY_Pin:
	 	 

	 	if(run_t.gPower_On ==RUN_POWER_ON && DEC_KEY_VALUE() == 1){

         if(run_t.ptc_warning ==0){
		 	 run_t.gTimer_time_colon=0;
		// SendData_Buzzer();
	 	 //run_t.keyvalue  = DEC_KEY_ID;
	 	  switch(run_t.temp_set_timer_timing_flag){

		 	case 0: //set temperature value
	    
			//setup temperature of value,minimum 20,maximum 40
			set_up_temperature_value--;
			if(set_up_temperature_value<20) set_up_temperature_value=40;
	        else if(set_up_temperature_value >40)set_up_temperature_value=40;

	        run_t.set_temperature_decade_value = set_up_temperature_value / 10 ;
			run_t.set_temperature_unit_value  =set_up_temperature_value % 10; //

			
			// TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);
			
		      run_t.set_temperature_flag=set_temperature_value;
			  run_t.set_temperature_special_value=0;
			  run_t.gTimer_key_temp_timing=0;
			  run_t.gTimer_time_colon=0;
			  run_t.display_timer_timing_flag=2;
			 
	    	
		   break;

		   case 1: //set timer timing value
	    	
			
				run_t.gTimer_key_timing =0;
				run_t.timer_dispTime_minutes =  run_t.timer_dispTime_minutes -30;
		        if(run_t.timer_dispTime_minutes < 0){
					run_t.timer_dispTime_hours--;
                   if(run_t.timer_dispTime_hours <0){
                         
				      run_t.timer_dispTime_hours=24;
					  run_t.timer_dispTime_minutes=0;

				   }
				   else{

				     run_t.timer_dispTime_minutes =30;


				   }
				  
				}
				
				run_t.judge_minutes_if_zero = run_t.timer_dispTime_minutes;
                run_t.judge_hours_if_zero  = run_t.timer_dispTime_hours;

			    if(run_t.timer_dispTime_hours > 9  && run_t.timer_dispTime_hours <20){
					      run_t.hours_two_decade_bit = 1 ;
					      run_t.hours_two_unit_bit =run_t.timer_dispTime_hours %10 ; 
			    }
				else if(run_t.timer_dispTime_hours > 19 ){
					
				  run_t.hours_two_decade_bit = 2 ;
				  run_t.hours_two_unit_bit =run_t.timer_dispTime_hours %10  ; 

				}
				else{

					       run_t.hours_two_decade_bit =0;
						   run_t.hours_two_unit_bit= run_t.timer_dispTime_hours;
				}

				switch(run_t.timer_dispTime_minutes){

					   case 30:
							run_t.minutes_one_decade_bit= 3;
							run_t.minutes_one_unit_bit= 0;
					   break;

		
					   case 0:
							run_t.minutes_one_decade_bit= 0;
							run_t.minutes_one_unit_bit= 0;
					   break;
					

					}
                 run_t.display_timer_timing_flag=1;


		     run_t.gTimer_time_colon =0;


		  
		  break;
	   	  }

         }


	  }
       __HAL_GPIO_EXTI_CLEAR_RISING_IT(DEC_KEY_Pin);
	 break;

	 case ADD_KEY_Pin:
	 	
	
	 	if(run_t.gPower_On ==RUN_POWER_ON && ADD_KEY_VALUE() ==1){

		  if(run_t.ptc_warning ==0){
				// SendData_Buzzer();

                run_t.gTimer_time_colon=0;

			  switch(run_t.temp_set_timer_timing_flag){

		    case 0:  //set temperature value 
                set_up_temperature_value ++;
	            if(set_up_temperature_value < 20){
				    set_up_temperature_value=20;
				}
				
				if(set_up_temperature_value > 40)set_up_temperature_value= 20;
				
			   run_t.set_temperature_decade_value = set_up_temperature_value / 10 ;
			   run_t.set_temperature_unit_value  =set_up_temperature_value % 10; //
   
              run_t.set_temperature_flag=set_temperature_value;
			   run_t.set_temperature_special_value=0;
			  run_t.gTimer_key_temp_timing=0;
			  run_t.gTimer_time_colon=0;
			  run_t.display_timer_timing_flag=2;
			
			break;

			case 1: //set timer timing value 
				 run_t.gTimer_key_timing =0;
				 if(run_t.timer_dispTime_hours !=24)
				 		run_t.timer_dispTime_minutes =  run_t.timer_dispTime_minutes + 30;
				 else if(run_t.timer_dispTime_hours ==24)
				 	    run_t.timer_dispTime_minutes =  run_t.timer_dispTime_minutes + 60;
			     if(run_t.timer_dispTime_minutes >59){
					 run_t.timer_dispTime_hours ++;
		             if(run_t.timer_dispTime_hours ==24){
						run_t.timer_dispTime_minutes=0;
					}
					else if(run_t.timer_dispTime_hours >24){

					   run_t.timer_dispTime_hours=0;
					   run_t.timer_dispTime_minutes=0;


					}
					else{

					   run_t.timer_dispTime_minutes =0;


					}
						
			     }

                run_t.judge_minutes_if_zero = run_t.timer_dispTime_minutes;
                run_t.judge_hours_if_zero  = run_t.timer_dispTime_hours;

				if(run_t.timer_dispTime_hours > 9  && run_t.timer_dispTime_hours <20){
					      run_t.hours_two_decade_bit = 1 ;
					      run_t.hours_two_unit_bit =run_t.timer_dispTime_hours %10 ; 
			    }
				else if(run_t.timer_dispTime_hours > 19 ){
					
				  run_t.hours_two_decade_bit = 2 ;
				  run_t.hours_two_unit_bit =run_t.timer_dispTime_hours %10  ; 

				}
				else{

					       run_t.hours_two_decade_bit =0;
						   run_t.hours_two_unit_bit= run_t.timer_dispTime_hours;
				}

					switch(run_t.timer_dispTime_minutes){

					   case 30:
							run_t.minutes_one_decade_bit= 3;
							run_t.minutes_one_unit_bit= 0;
					   break;

					   case 0:
							run_t.minutes_one_decade_bit= 0;
							run_t.minutes_one_unit_bit= 0;
					   break;
					

					}
                    run_t.display_timer_timing_flag=1;
					// TM1639_Write_4Bit_Time(run_t.hours_two_decade_bit,run_t.hours_two_unit_bit, run_t.minutes_one_decade_bit,run_t.minutes_one_unit_bit,0) ; //timer is default 12 hours "12:00" 
			
              run_t.gTimer_time_colon =0;
				
	  	    }

		  }
	  }
       __HAL_GPIO_EXTI_CLEAR_RISING_IT(ADD_KEY_Pin);
	 break;


     case ULTRASONIC_KEY_Pin :
       
        if(run_t.gPower_On ==RUN_POWER_ON && ULTRASONIC_KEY_VALUE() ==1){
                if(run_t.fan_warning ==0 && run_t.ptc_warning == 0){ 

               if(run_t.ai_model_flag== NO_AI_MODE ){

                   if(run_t.gUltrasonic ==1){
                       
				     run_t.gUltrasonic =0;
					 LED_BUG_OFF();
				      SendData_Set_Command(ULTRASONIC_OFF);


				   }
                   else{
				   	run_t.gUltrasonic =1;
					LED_BUG_ON();
				    SendData_Set_Command(ULTRASONIC_ON);

				   }
               }
				  
				 
			}
		 }
		  __HAL_GPIO_EXTI_CLEAR_RISING_IT(ULTRASONIC_KEY_Pin);
     break;

	 case PLASMA_KEY_Pin:

	    
        if(run_t.gPower_On ==RUN_POWER_ON){
			
               switch(run_t.ai_model_flag){

               case NO_AI_MODE:
			   if(run_t.gPlasma ==1){  //turun off kill 
			   	
			       run_t.gPlasma = 0;
				   SendData_Set_Command(PLASMA_OFF);
			      PLASMA_LED_OnOff(1);
			       
                
			   	}  
                else{
                   run_t.gPlasma = 1;
				   SendData_Set_Command(PLASMA_ON);
				   PLASMA_LED_OnOff(0);
               
				}

               break;

               case AI_MODE:

               break;
				   
		       
			 }

            }
	    __HAL_GPIO_EXTI_CLEAR_RISING_IT(PLASMA_KEY_Pin);
	 break;

	 case DRY_KEY_Pin:
      if(run_t.gPower_On ==RUN_POWER_ON){
		      if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){

              switch(run_t.ai_model_flag){ //WT.EDIT 2023.09.12

              case NO_AI_MODE:
			  if(run_t.gDry== 1){
				    run_t.gDry =0;
					SendData_Set_Command(DRY_OFF);
                  
                    run_t.manual_dry_turn_off=1;
					DRY_LED_OnOff(1);
               }
               else{
                    run_t.gDry =1;
					run_t.manual_dry_turn_off=0;
					SendData_Set_Command(DRY_ON);
					DRY_LED_OnOff(0);
                  
                 }  
			   
              break;

              case AI_MODE:
 
              break;
              }
		    }
           }
	  
    
	 __HAL_GPIO_EXTI_CLEAR_RISING_IT(DRY_KEY_Pin);

	 break;

	

    }
 
}

void Key_TheSecond_Scan(void)
{
	
    
    if(run_t.gTimer_mode_key_start_counter ==1 && run_t.gPower_On ==RUN_POWER_ON){

		if(MODEL_KEY_VALUE()    ==KEY_UP){
			 run_t.gTimer_time_colon=0;
			if(run_t.gTimer_mode_key_counter < 2){

				run_t.keyvalue  = MODEL_KEY_ID;
				run_t.gTimer_mode_key_start_counter=0;
			    run_t.recoder_start_conuter_flag=0;	
                return ;
			}
		}
        else if(MODEL_KEY_VALUE()    ==KEY_DOWN){
			 run_t.gTimer_time_colon=0;
			if(run_t.gTimer_mode_key_counter> 3 || run_t.gTimer_mode_key_counter==3){

			run_t.keyvalue  = MODE_LONG_KEY_ID;
			run_t.gTimer_mode_key_start_counter=0;
			run_t.recoder_start_conuter_flag=0;	
		

			 return ;
			}


		}
	}

}

//������������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼￄ1�7,KEY0>KEY1>KEY2>WK_UP!!
uint8_t KEY_Normal_Scan(uint8_t mode)
{
    static uint8_t key_up=1;     //�����ɿ���־
    if(mode==1)key_up=1;    //֧������
    if(key_up&&(AI_KEY_VALUE()==1))
    {
        HAL_Delay(20);
		run_t.gTimer_time_colon =0;
        key_up=0;
        if(AI_KEY_VALUE()==1)       return run_t.keyvalue  = AI_KEY_ID;
      //  else if(DRY_KEY_VALUE()==1)  return run_t.keyvalue  = DRY_KEY_ID;
      //  else if(PLASMA_KEY_VALUE()==1)  return run_t.keyvalue  = PLASMA_KEY_ID;
    }else if(AI_KEY_VALUE()==0)key_up=1;
    return 0;   //�ް�������
}




