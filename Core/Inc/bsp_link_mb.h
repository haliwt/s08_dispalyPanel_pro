#ifndef __BSP_LINK_MB_H_
#define __BSP_LINK_MB_H_
#include "main.h"

typedef struct{

   uint8_t gTimer_receive_mb_times;
   uint8_t response_power_on;
   uint8_t response_power_off;



}usart_rx_tx_t;

extern usart_rx_tx_t usart_t;

void bsp_Idle(void);


uint8_t Power_ReadParam_OnOff(uint8_t reg);


uint8_t Wifi_LoginParam_On(void);

uint8_t Aanswering_Signal_BuzzerParam_Sound(void);



#endif 

