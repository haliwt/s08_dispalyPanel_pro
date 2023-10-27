#include "cmd_link.h"
#include "bsp.h"

volatile static uint8_t transOngoingFlag; //interrupt Transmit flag bit , 1---stop,0--run
static void MB_Answer_Signal(uint8_t data);

uint8_t outputBuf[8];
static uint8_t transferSize;
static uint8_t state;
uint8_t inputBuf[MAX_BUFFER_SIZE];
uint8_t parse_buf[MAX_BUFFER_BUF];


/****************************************************************************************************
**
*Function Name:static void selectLight(uint8_t index)
*Function: UART2 transmit interrupt process ---4D 58 4C 53 32 30 32 
*Input Ref: LED number 
*Return Ref:NO
*
****************************************************************************************************/
void SendData_Power_OnOff(uint8_t index)
{
	
   //crc=0x55;
	outputBuf[0]='T'; //4D //Touch Key
	outputBuf[1]='K'; //58
	outputBuf[2]='P'; //4C	// 'P' power
	outputBuf[3]=index; //53	// 'S' select light command, 'C' close all light command
	
    transferSize=4;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
	
}
void SendData_Buzzer(void)
{
	
   //crc=0x55;
	outputBuf[0]='T'; //4D
	outputBuf[1]='K'; //58
	outputBuf[2]='Z'; //4C	// buzzer
	outputBuf[3]='Z'; //53	// buzzer
	
    transferSize=4;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
	
}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send 
 * 
*********************************************************/
void SendData_Set_Wifi(uint8_t hdata)
{
	    outputBuf[0]='T'; //
		outputBuf[1]='K'; //58
		outputBuf[2]='W'; //W ->WIFI
		outputBuf[3]=hdata; 
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	
}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Set_Command(uint8_t hdata)

{
	    outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='C'; //"C" ->command
		outputBuf[3]=hdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	
}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Temp_Data(uint8_t tdata)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='M'; //"T"->temperature
		outputBuf[3]=tdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}
/*********************************************************
 * 
 * Function Name:void SendData_Time_Data(uint8_t tdata)
 * Function:send to set  timer timing value
 * 
*********************************************************/
void SendData_Time_Data(uint8_t tdata)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='T'; //"T"->timer timing
		outputBuf[3]=tdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Works_Time(uint8_t tdata,uint8_t tdata_2)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='O'; //"T"->temperature
		outputBuf[2]=tdata; //53	//
		outputBuf[3]=tdata_2;
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}


void SendData_Remaining_Time(uint8_t tdata,uint8_t tdata_2)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='R'; //"T"->temperature
		outputBuf[2]=tdata; //53	//
		outputBuf[3]=tdata_2;
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}



/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   
	if(huart==&huart1) // Motor Board receive data (filter)
	{
		switch(state)
		{
		case 0:  //#0
			if(inputBuf[0]=='M') {//hex :4D - "M" -fixed mainboard
               
			   state=1;

             }
			break;
		case 1: //#1
			if(inputBuf[0]=='A') //hex : 41 -'A'  -fixed master
			{
				
				state=2; 
			}
			else{
				
				state=0;
			}
			break;
		case 2://#2
			if(inputBuf[0]=='D')
			 {
				
				run_t.rx_mb_data_tag=PANEL_DATA; //receive data is single data
                state=3;
			}
            else if(inputBuf[0]=='E'){
                run_t.rx_mb_data_tag=ORDER_DATA;
                state=3;
            }
			else if(inputBuf[0]=='A'){

			     run_t.rx_mb_data_tag=ANSWER_DATA;
				 state=3;

			}
			else{
				state=0;
				


				}
			break;
            
        case 3:

            if(run_t.rx_mb_data_tag==PANEL_DATA){
            
                 run_t.gReal_humtemp[0]=inputBuf[0]; //Humidity value 
                  
                  state = 4; 
                
             }
			else if(run_t.rx_mb_data_tag==ORDER_DATA){

			    outputBuf[0]=  inputBuf[0];  
				state=0;
				run_t.decodeFlag=1;
	


			}
			else if(run_t.rx_mb_data_tag==ANSWER_DATA){
                 
			  outputBuf[0]=  inputBuf[0]; 
			  MB_Answer_Signal(outputBuf[0]);
			  state=0;
			  run_t.decodeFlag=0;


			}
            else {

				state = 0; 
				run_t.decodeFlag=0;
            }
	    break;
        
		case 4: //

		 if(run_t.rx_mb_data_tag==PANEL_DATA){
              run_t.gReal_humtemp[1]=inputBuf[0]; //temperature value
			
		     state=0;
		     run_t.decodeFlag=1;
		
              
          }
         else {

			parse_buf[4] =inputBuf[0];
			state =5;

         }
		
		 
		 break;


		 case 5:
		  parse_buf[5] =inputBuf[0];
              state=0;
			  run_t.decodeFlag=1;
		      run_t.rx_mb_data_tag=0;
			  run_t.rx_mb_data_tag=ANSWER_DATA;
        break;
           
       
		
		
		default:
			
		break;

		}
       // __HAL_UART_CLEAR_OREFLAG(&huart1);
		HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
	}
}

static void MB_Answer_Signal(uint8_t data)
{
   switch(data){

    case 0x51: //power on
       usart_t.response_power_on =0;
    break;

    case 0x50: //power off
      usart_t.response_power_off =1;
	break;


    default:
    break;

   }


}

void USART1_Cmd_Error_Handler(void)
{
   uint32_t temp;
   if(run_t.gTimer_usart_error >4){
	  	run_t.gTimer_usart_error=0;
	    
         
        __HAL_UART_CLEAR_OREFLAG(&huart1);
        temp = USART1->RDR;
		UART_Start_Receive_IT(&huart1,inputBuf,1);
       
		}
      
         
} 
        
   

/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart1)
	{
		transOngoingFlag=0; //UART Transmit interrupt flag =0 ,RUN
	}
	
}

