#include "Uart_lib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dht11_lib.h"
#include "CLCD_I2C.h"
extern UART_HandleTypeDef huart1;
extern uint16_t adc_mq2_index;
extern dht11_index index11;
extern uint32_t time_delay_dht;
extern uint32_t time_delay_gas;
extern uint8_t tem_threshold;
extern uint8_t hum_threshold;
extern uint16_t gas_threshold;
extern enum_string string_handle;
extern uart_receive_state uart_state;
extern CLCD_I2C_Name LCD1;
extern mode_button mode_bt;
extern uint8_t mode_computer;
void float_to_string(float number,char*string)
{
	int a = (int)number;
	string[0] = '\0';
	char x[10];
	sprintf(x,"%d",a);
	strcpy(string,x);
	strcat(string,".");
	a = (number - a)*100;
	if(a == 0)
	strcat(string,"00");
	else if(a<10)
	{
		sprintf(x,"%d",a);
		strcat(string,"0");
		strcat(string,x);
	}
	else
	{
		sprintf(x,"%d",a);
		strcat(string,x);
	}
	strcat(string,"\n");
}
uint8_t check_number(char *number_string)
{
	uint8_t result = 1;
	uint8_t n = strlen(number_string);
	uint8_t i;
	for(i = 0;i<n;i++)
	{
		if(number_string[i] > '9'||number_string[i]<'0')
		{
			result = 0;
			i = n;
		}
	}
	return result;
}
void tem_string_handle(enum_string string_handle,uint8_t *dt)
{
	uint8_t i = 0; 
	if(string_handle == freq_dht)
	{
		char tem_data[10]="\0"; 
		strcpy((char*)dt,(char*)(dt+18));
		while(dt[i]!='O'&&dt[i+1]!='K')
		{
			tem_data[i]=dt[i];
			tem_data[i+1]='\0';
			i++;
		}
		if(check_number(tem_data)==1)
		{
			int tem_value = atoi(tem_data);
			if(tem_value < 10||tem_value > 90)
			{
				HAL_UART_Transmit(&huart1,"\nfail to set dht frequency value",32,100);
				HAL_UART_Transmit(&huart1,"\ndht frequency value ranges from 10 to 90\n",42,100);
			}
			else
			{
				time_delay_dht = tem_value * 1000;
				HAL_UART_Transmit(&huart1,"\ndht frequency is set\n",22,100);
			}
		}
		else
			HAL_UART_Transmit(&huart1,"\nsyntax error\n",14,100);
	}
	else if(string_handle == freq_gas)
	{
		char tem_data[10]="\0"; 
		strcpy((char*)dt,(char*)(dt+18));
		while(dt[i]!='O'&&dt[i+1]!='K')
		{
			tem_data[i]=dt[i];
			tem_data[i+1]='\0';
			i++;
		}
		if(check_number(tem_data)==1)
		{
			int tem_value = atoi(tem_data);
			if(tem_value < 10||tem_value > 90)
			{
				HAL_UART_Transmit(&huart1,"\nfail to set gas frequency value",32,100);
				HAL_UART_Transmit(&huart1,"\ngas frequency value ranges from 10 to 90\n",42,100);
			}
			else
			{
				time_delay_gas = tem_value * 1000;
				HAL_UART_Transmit(&huart1,"\ngas frequency is set\n",22,100);
			}
		}
		else
			HAL_UART_Transmit(&huart1,"\nsyntax error\n",14,100);
	}
	else if(string_handle == tem_str)
	{
		char tem_data[10]="\0"; 
		strcpy((char*)dt,(char*)(dt+18));
		while(dt[i]!='O'&&dt[i+1]!='K')
		{
			tem_data[i]=dt[i];
			tem_data[i+1]='\0';
			i++;
		}
		if(check_number(tem_data)==1)
		{
			int tem_value = atoi(tem_data);
			if(tem_value < 10||tem_value > 45)
			{
				HAL_UART_Transmit(&huart1,"\nfail to set temperature threshold",34,100);
				HAL_UART_Transmit(&huart1,"\ntemperature threshold ranges from 10 to 45\n",44,100);
			}
			else
			{
				tem_threshold = tem_value;
				HAL_UART_Transmit(&huart1,"\ntemperature threshold is set\n",30,100);
			}
		}
		else
			HAL_UART_Transmit(&huart1,"\nsyntax error\n",14,100);
	}
	else if(string_handle == hum_str)
	{ 
		char tem_data[10]="\0"; 
		strcpy((char*)dt,(char*)(dt+18));
		while(dt[i]!='O'&&dt[i+1]!='K')
		{
			tem_data[i]=dt[i];
			tem_data[i+1]='\0';
			i++;
		}
		if(check_number(tem_data)==1)
		{
			int tem_value = atoi(tem_data);
			if(tem_value < 30||tem_value > 95)
			{
				HAL_UART_Transmit(&huart1,"\nfail to set humidity threshold",31,100);
				HAL_UART_Transmit(&huart1,"\nhumidity threshold ranges from 30 to 95\n",41,100);
			}
			else
			{
				hum_threshold = tem_value;
				HAL_UART_Transmit(&huart1,"\nhumidity threshold is set\n",27,100);
			}
		}
		else
			HAL_UART_Transmit(&huart1,"\nsyntax error\n",14,100);
	}
	else if(string_handle == gas_str)
	{
		char tem_data[10]="\0"; 
		strcpy((char*)dt,(char*)(dt+18));
		while(dt[i]!='O'&&dt[i+1]!='K')
		{
			tem_data[i]=dt[i];
			tem_data[i+1]='\0';
			i++;
		}
		if(check_number(tem_data)==1)
		{
			int tem_value = atoi(tem_data);

			if(tem_value < 100||tem_value > 4000)
			{
				HAL_UART_Transmit(&huart1,"\nfail to set gas threshold",26,100);
				HAL_UART_Transmit(&huart1,"\ngas threshold ranges from 100 to 4000\n",41,100);
			}
			else
			{
				gas_threshold = tem_value;
				HAL_UART_Transmit(&huart1,"\ngas threshold is set\n",22,100);
			}
		}
		else
			HAL_UART_Transmit(&huart1,"\nsyntax error\n",14,100);
	}
	else if(string_handle == get_tem_str)
	{
		if(mode_computer == 1)
		{
			char send_mode_error[100] = "\ncannot get temperature index because of mode 1\n";
			HAL_UART_Transmit(&huart1,(uint8_t*)send_mode_error,strlen(send_mode_error),100);
		}
		else if (mode_computer == 2)
		{
			get_temperature();
		}
	}
	else if(string_handle == get_hum_str)
	{
		if(mode_computer == 1)
		{
			char send_mode_error[100] = "\ncannot get humidity index because of mode 1\n";
			HAL_UART_Transmit(&huart1,(uint8_t*)send_mode_error,strlen(send_mode_error),100);
		}
		else if (mode_computer == 2)
		{
			get_humidity();
		}
	}
	else if(string_handle == get_gas_str)
	{
		if(mode_computer == 1)
		{
			char send_mode_error[100] = "\ncannot get gas index because of mode 1\n";
			HAL_UART_Transmit(&huart1,(uint8_t*)send_mode_error,strlen(send_mode_error),100);
		}
		else if (mode_computer == 2)
		{
			get_gas();
		}
	}
	else if(string_handle == set_mode1)
	{
		 mode_computer = 1;
			char mode1_send_string[100] = "\nmode1 is set successfully\n";
			HAL_UART_Transmit(&huart1,(uint8_t*)mode1_send_string,strlen(mode1_send_string),100);
	}
	else if(string_handle == set_mode2)
	{
		 mode_computer = 2;
				char mode2_send_string[100] = "\nmode2 is set successfully\n";
			HAL_UART_Transmit(&huart1,(uint8_t*)mode2_send_string,strlen(mode2_send_string),100);
	}
	else if(string_handle == error_str)
	{
		HAL_UART_Transmit(&huart1,"\nsyntax error\n",14,100);
	}
	dt[0] = '\0';
}
void uart_receive_handle(uint8_t *dt)
{
	if(uart_state == success)
	{
		uart_state = not_success;
		char dht_freq_string[20] = "set dht frequency ";
		char gas_freq_string[20] = "set gas frequency ";
		char tem_string[25] = "set tem threshold ";
		char hum_string[25] = "set hum threshold ";
		char gas_string[25] = "set gas threshold ";
		char get_tem_string[20] = "get tem";
		char get_hum_string[20] = "get hum";
		char get_gas_string[20] = "get gas";
		char mode_1[15] = "set mode1";
		char mode_2[15] = "set mode2";
		char *check;
		check = strstr((char*)dt,dht_freq_string);
		if(check == NULL)		
		{
			check = strstr((char*)dt,gas_freq_string);
			if(check == NULL)
			{
				check = strstr((char*)dt,tem_string);
				if(check == NULL)
				{
					check = strstr((char*)dt,hum_string);
					if(check == NULL)
					{
						check = strstr((char*)dt,gas_string);
						if(check == NULL)
						{
							check = strstr((char*)dt,get_tem_string);
							if((check == NULL)||(!(dt[7]=='O'&&dt[8]=='K')))
							{
								check = strstr((char*)dt,get_hum_string);
								if((check == NULL)||(!(dt[7]=='O'&&dt[8]=='K')))
								{
									check = strstr((char*)dt,get_gas_string);
									if((check == NULL)||(!(dt[7]=='O'&&dt[8]=='K')))
									{
										check = strstr((char*)dt,mode_1);
										if((check == NULL)||(!(dt[9]=='O'&&dt[10]=='K')))
										{
											check = strstr((char*)dt,mode_2);
											if((check == NULL)||(!(dt[9]=='O'&&dt[10]=='K')))
												string_handle = error_str;
											else
												string_handle = set_mode2;
										}
										else
											string_handle = set_mode1;
									}
									else
										string_handle = get_gas_str;
								}
								else
									string_handle = get_hum_str;
							}
							else
								string_handle = get_tem_str;
						}
						else
							string_handle = gas_str;
					}
					else
						string_handle = hum_str;
				}
				else
					string_handle = tem_str;
			}
			else 
				string_handle = freq_gas;
		}
		else
			string_handle = freq_dht;
		tem_string_handle(string_handle, dt);
	}
}
void threshold_handle(void)
{
	if(index11.tCelsius >= tem_threshold)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	}
	else
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
	if(index11.RH >= hum_threshold)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
	}
	else
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
	if(adc_mq2_index>= gas_threshold)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
	}
	else
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
}
void send_message_to_computer(void)
{
	if(index11.tCelsius >= tem_threshold)
	{
		HAL_UART_Transmit(&huart1,"\nWarning: temperature exceeds the threshold\n",44,100);
	}
	if(index11.RH >= hum_threshold)
	{
		HAL_UART_Transmit(&huart1,"\nWarning: humidity exceeds the threshold\n",41,100);
	}
	if(adc_mq2_index>= gas_threshold)
	{
		HAL_UART_Transmit(&huart1,"\nWarning: gas concentration exceeds the threshold\n",50,100);
	}
}
void send_message_to_LCD(void)
{
	char LCD_string[100];
	if(mode_bt == mode_display)
	{
		CLCD_I2C_SetCursor(&LCD1, 4, 0);
		float_to_string(index11.tCelsius,LCD_string);
		LCD_string[strlen(LCD_string)-1] = '\0';
		LCD_string[strlen(LCD_string)-1] = '\0';
		if(strlen(LCD_string) <4)
			strcat(LCD_string," ");
		CLCD_I2C_WriteString(&LCD1,LCD_string);
		LCD_string[0] = '\0';
		
		CLCD_I2C_SetCursor(&LCD1,12, 0);
		float_to_string(index11.RH,LCD_string);
			LCD_string[strlen(LCD_string)-1] = '\0';
		LCD_string[strlen(LCD_string)-1] = '\0';
		if(strlen(LCD_string) <4)
			strcat(LCD_string," ");
		CLCD_I2C_WriteString(&LCD1,LCD_string);
		LCD_string[0] = '\0';
		
		CLCD_I2C_SetCursor(&LCD1,4, 1);
		sprintf(LCD_string, "%d", adc_mq2_index);
		while(strlen(LCD_string)<4)
			strcat(LCD_string," ");
		CLCD_I2C_WriteString(&LCD1,LCD_string);
		LCD_string[0] = '\0';
	}
	else if(mode_bt == mode_dht)
	{
		CLCD_I2C_SetCursor(&LCD1, 10, 0);
		sprintf(LCD_string, "%d", time_delay_dht);
		CLCD_I2C_WriteString(&LCD1,LCD_string);
		LCD_string[0] = '\0';
	}
	else if(mode_bt == mode_gas)
	{
		CLCD_I2C_SetCursor(&LCD1, 10, 0);
		sprintf(LCD_string, "%d", time_delay_gas);
		CLCD_I2C_WriteString(&LCD1,LCD_string);
		LCD_string[0] = '\0';
	}
}
void get_temperature(void)
{
	char send_temperature[30] = "\ntemperature is: ";
	char temperature_number[10];
	float_to_string(index11.tCelsius,temperature_number);
	strcat(send_temperature,temperature_number);
	HAL_UART_Transmit(&huart1,(uint8_t*)send_temperature,strlen(send_temperature),100);
}
void get_humidity(void)
{
	char send_humidity[30] = "\nhumidity is: ";
	char humidity_number[10];
	float_to_string(index11.RH,humidity_number);
	strcat(send_humidity,humidity_number);
	HAL_UART_Transmit(&huart1,(uint8_t*)send_humidity,strlen(send_humidity),100);
}
void get_gas(void)
{
	char send_gas[30] = "\ngas concentration is: ";
	char gas_number[10];
	float_to_string(adc_mq2_index,gas_number);
	strcat(send_gas,gas_number);
	HAL_UART_Transmit(&huart1,(uint8_t*)send_gas,strlen(send_gas),100);
}
