/*
 * UartManager.c
 *
 *  Created on: May 13, 2024
 *      Author: JDEROCHE BERMONDSEY
 */

#include "main.h"
#include "cmsis_os.h"

#include "UartManager.h"

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;

extern osMessageQId UartRxDataHandle;


uint8_t Test[4] = {0};
float DutyCycle = 0;
int PwmState = 0;
float Inc = 0.01f;


/**
* @brief Function implementing the UartTask thread.
* @param argument: Not used
* @retval None
*/
void URT_TaskManager(void const * argument)
{
  /* USER CODE BEGIN TSK_UartManager */
  /* Infinite loop */
  osEvent evt;
  MessageUart* pMessage;

  for(;;)
  {
    osDelay(10);
    if (HAL_UART_Receive_IT(&huart2, Test, 1) == HAL_OK)
    {
      if (*Test == 'S')
      {
        // Start PWM signal generation on TIM1 CH1:
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

        // Set pulse value for desired duty cycle
        htim2.Instance->CCR1 = 700;

      }
      else if (*Test == 'T')
      {
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
      }

      // Echo Character
      HAL_UART_Transmit(&huart2, Test, 1, 200);

    }

#ifdef _OS_MESSAGE_
    // Get the message sent from the main task
    evt = osMessageGet(UartRxDataHandle,10);

    if (evt.status == osEventMessage)
    {
      // Get the value here Val = *(DataType*)&messageHandler.value;
      // do something here
      // Echo Character
      pMessage = (MessageUart *)evt.value.p;
      HAL_UART_Transmit(&huart2, (const uint8_t *)pMessage->pData, pMessage->Size, 200);
    }
#endif

  }
  /* USER CODE END TSK_UartManager */
}

/**
* @brief Function implementing the UartTask thread.
* @param argument: Not used
* @retval None
*/
void URT_Read(uint8_t * pData, uint16_t Size, URT_ReadCallBack_TP MyCallBack)
{

}


