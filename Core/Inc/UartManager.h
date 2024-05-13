/*
 * UartManager.h
 *
 *  Created on: May 13, 2024
 *      Author: JDEROCHE BERMONDSEY
 */

#ifndef INC_UARTMANAGER_H_
#define INC_UARTMANAGER_H_

typedef void (* URT_ReadCallBack_TP)(uint8_t * pData, uint16_t Size);

typedef struct
{
  uint8_t Size;
  char * pData;

}MessageUart;

void URT_TaskManager(void const * argument);
void URT_Read(uint8_t * pData, uint16_t Size, URT_ReadCallBack_TP MyCallBack);

#endif /* INC_UARTMANAGER_H_ */
