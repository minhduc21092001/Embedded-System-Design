#include "dht11_lib.h"
uint32_t pMillis;
uint32_t cMillis;

void microDelay (TIM_HandleTypeDef *htim, uint16_t delay)
{
	__HAL_TIM_SetCounter(htim,0);
	__HAL_TIM_SetAutoreload(htim,65535);
	HAL_TIM_Base_Start(htim);
	while(__HAL_TIM_GetCounter(htim)<delay);
	HAL_TIM_Base_Stop(htim);
}
uint8_t DHT11_Start (TIM_HandleTypeDef *htim, GPIO_TypeDef* DHT11_PORT, uint16_t DHT11_PIN)
{
  uint8_t Response = 0;
  GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
  GPIO_InitStructPrivate.Pin = DHT11_PIN;
  GPIO_InitStructPrivate.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructPrivate); // set the pin as output
  HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);   // pull the pin low
  microDelay (htim, 20000);  // wait for 20ms
  HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);   // pull the pin high
  microDelay (htim, 30);   // wait for 30us
  GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructPrivate.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructPrivate); // set the pin as input
  microDelay (htim, 40);
  if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
  {
    microDelay (htim, 80);
    if ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) Response = 1;
  }
  pMillis = HAL_GetTick();
  cMillis = HAL_GetTick();
  while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)) && pMillis + 2 > cMillis)
  {
    cMillis = HAL_GetTick();
  }
  return Response;
}
uint8_t DHT11_Read (TIM_HandleTypeDef *htim, GPIO_TypeDef* DHT11_PORT, uint16_t DHT11_PIN)
{
  uint8_t a,b;
  for (a=0;a<8;a++)
  {
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)) && pMillis + 2 > cMillis)
    {  // wait for the pin to go high
      cMillis = HAL_GetTick();
    }
    microDelay (htim, 40);   // wait for 40 us
    if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
      b&= ~(1<<(7-a));
    else
      b|= (1<<(7-a));
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)) && pMillis + 2 > cMillis)
    {  // wait for the pin to go low
      cMillis = HAL_GetTick();
    }
  }
  return b;
}
dht11_index read_3_dht11(TIM_HandleTypeDef *htim, GPIO_TypeDef* DHT11_PORT, uint16_t DHT11_PIN)
{
		dht11_index index = {1,0,0};
		uint8_t RHI = DHT11_Read(htim,DHT11_PORT,DHT11_PIN); // Relative humidity integral
		uint8_t RHD = DHT11_Read(htim,DHT11_PORT,DHT11_PIN); // Relative humidity decimal
		uint8_t TCI = DHT11_Read(htim,DHT11_PORT,DHT11_PIN); // Celsius integral
		uint8_t TCD = DHT11_Read(htim,DHT11_PORT,DHT11_PIN); // Celsius decimal
		uint8_t SUM = DHT11_Read(htim,DHT11_PORT,DHT11_PIN); // Check sum
			if (RHI + RHD + TCI + TCD == SUM)
		{
			// Can use RHI and TCI for any purposes if whole number only needed
			index.tCelsius = (float)TCI + (float)(TCD/10.0);
			index.tFahrenheit = index.tCelsius * 9/5 + 32;
			index.RH = (float)RHI + (float)(RHD/10.0);
			// Can use tCelsius, tFahrenheit and RH for any purposes
		}
		return index;
}

