#ifndef _DHT11_LIB_H
#define _DHT11_LIB_H
#include "stm32f1xx_hal.h"
typedef struct
{
	float tCelsius;
	float tFahrenheit;
	float RH;
}dht11_index;
void microDelay (TIM_HandleTypeDef *htim,uint16_t delay);
uint8_t DHT11_Start (TIM_HandleTypeDef *htim, GPIO_TypeDef* DHT11_PORT, uint16_t DHT11_PIN);
uint8_t DHT11_Read (TIM_HandleTypeDef *htim, GPIO_TypeDef* DHT11_PORT, uint16_t DHT11_PIN);
dht11_index read_3_dht11(TIM_HandleTypeDef *htim, GPIO_TypeDef* DHT11_PORT, uint16_t DHT11_PIN);
#endif
