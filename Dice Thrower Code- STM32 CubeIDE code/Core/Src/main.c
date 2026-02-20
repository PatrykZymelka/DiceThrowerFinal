/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fonts.h"
#include "ssd1306.h"
#include "lis3dh_driver.h"
#include <math.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t seed = 128758;

uint32_t RandomNumber(uint32_t min, uint32_t max) {
    seed = (1664525 * seed + 1013904223) % 0xFFFFFFFF;
    return (seed % (max - min + 1)) + min;
}
//Licznik dla każdej kości ile rzutów trzeba wykonać, zwiększa się wraz z klikaniem tego guzika
uint8_t d4Number = 0;
uint8_t d6Number = 0;
uint8_t d8Number = 0;
uint8_t d10Number = 0;
uint8_t d12Number = 0;
uint8_t d20Number = 0;
uint8_t d100Number = 0;

//Licznik dla każdej z kości jaka jest suma rzutów w danej "epoce"
uint32_t d4Counter = 0;
uint32_t d6Counter = 0;
uint32_t d8Counter = 0;
uint32_t d10Counter = 0;
uint32_t d12Counter = 0;
uint32_t d20Counter = 0;
uint32_t d100Counter = 0;

//Czyszczenie bufferu
void ClearBuffer(char* buffer, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        buffer[i] = '\0';
    }
}


void HomeScreen(){
	char buffer[40];
	SSD1306_Clear();
	//Po kolei pójście w daną część wyświetlacza i pisanie tam danej kości i ile rzutów wybrał użytkownik
	SSD1306_GotoXY(5, 10);
	SSD1306_Puts("D20:", &Font_7x10, 1);
	sprintf(buffer, "%u",d20Number);
	SSD1306_Puts(buffer, &Font_7x10, 1);

	SSD1306_GotoXY(48, 10);
	SSD1306_Puts("D12:", &Font_7x10, 1);
	sprintf(buffer, "%u",d12Number);
	SSD1306_Puts(buffer, &Font_7x10, 1);

	SSD1306_GotoXY(91, 10);
	SSD1306_Puts("D8:", &Font_7x10, 1);
	sprintf(buffer, "%u",d8Number);
	SSD1306_Puts(buffer, &Font_7x10, 1);

	SSD1306_GotoXY(5, 30);
	SSD1306_Puts("D10:", &Font_7x10, 1);
	sprintf(buffer, "%u",d10Number);
	SSD1306_Puts(buffer, &Font_7x10, 1);

	SSD1306_GotoXY(48, 30);
	SSD1306_Puts("D6:", &Font_7x10, 1);
	sprintf(buffer, "%u",d6Number);
	SSD1306_Puts(buffer, &Font_7x10, 1);

	SSD1306_GotoXY(91, 30);
	SSD1306_Puts("D4:", &Font_7x10, 1);
	sprintf(buffer, "%u",d4Number);
	SSD1306_Puts(buffer, &Font_7x10, 1);

	SSD1306_GotoXY(42, 50);
	SSD1306_Puts("D100:", &Font_7x10, 1);
	sprintf(buffer, "%u",d100Number);
	SSD1306_Puts(buffer, &Font_7x10, 1);
	//Po napisaniu wszystkich elementów na ekranie trzeba pamiętać by cały ekran zupdateować
	SSD1306_UpdateScreen();
	//Delay dla pełnego przepływu danych
	HAL_Delay(200);
}


void Display(uint16_t end, uint8_t i){
	//Czyści co jest teraz
	SSD1306_Clear();
	//Pozycjunuje kursor w danym miejscu ekranu
	SSD1306_GotoXY(5, 0);

	//Przemiana z int na char
	char buffer[40];
	sprintf(buffer, "Dice D%u:", end);

	//Pisze wynik na ekranie
	SSD1306_Puts(buffer, &Font_7x10, 1);

	SSD1306_GotoXY(5, 20);

	for(int j = 0; j < i; j++){
	//Losuje numer z zakresu kości i go wstawia na ekran
		int n = RandomNumber(1, end);
		sprintf(buffer, "%lu ", n);
		SSD1306_Puts(buffer, &Font_7x10, 1);
		//Switch case by dodać dany losowy rzut do countera czyli sumy, każda kość ma osobną sumę
		switch(end){
			case 4:
				d4Counter = d4Counter + n;
				break;
			case 6:
				d6Counter = d6Counter + n;
				break;
			case 8:
				d8Counter = d8Counter + n;
				break;
			case 10:
				d10Counter = d10Counter + n;
				break;
			case 12:
				d12Counter = d12Counter + n;
				break;
			case 20:
				d20Counter = d20Counter + n;
				break;
			case 100:
				d100Counter = d100Counter + n;
				break;

			}
	}
	SSD1306_GotoXY(5, 40);
	//Po pokazaniu wszystkich wyników i zliczeniu sum, pokazujemy sumę danej kości
	switch(end){
		case 4:
			sprintf(buffer, "Sum: %lu", d4Counter);
			SSD1306_Puts(buffer, &Font_7x10, 1);
			break;
		case 6:
			sprintf(buffer, "Sum: %lu", d6Counter);
			SSD1306_Puts(buffer, &Font_7x10, 1);
			break;
		case 8:
			sprintf(buffer, "Sum: %lu", d8Counter);
			SSD1306_Puts(buffer, &Font_7x10, 1);
			break;
		case 10:
			sprintf(buffer, "Sum: %lu", d10Counter);
			SSD1306_Puts(buffer, &Font_7x10, 1);
			break;
		case 12:
			sprintf(buffer, "Sum: %lu", d12Counter);
			SSD1306_Puts(buffer, &Font_7x10, 1);
			break;
		case 20:
			sprintf(buffer, "Sum: %lu", d20Counter);
			SSD1306_Puts(buffer, &Font_7x10, 1);
			break;
		case 100:
			sprintf(buffer, "Sum: %lu", d100Counter);
			SSD1306_Puts(buffer, &Font_7x10, 1);
			break;
		}
	//Oczywiście na końcu musimy wyzerować sumę dla wszystkich kości by kolejna "epoka" zaczynała od zera
	switch(end){
		case 4:
			d4Counter = 0;
			break;
		case 6:
			d6Counter = 0;
			break;
		case 8:
			d8Counter = 0;
			break;
		case 10:
			d10Counter = 0;
			break;
		case 12:
			d12Counter = 0;
			break;
		case 20:
			d20Counter = 0;
			break;
		case 100:
			d100Counter = 0;
			break;
		}
	//Pokazanie napianych rzeczy na ekranie
	SSD1306_UpdateScreen();

	HAL_Delay(1000);

}


void Buzz(){
	//Wysyłamy sygnał PWM na port gdzie jest połączony nasz głośnik
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	int x;
	//Wysyłamy po kolei od niższych do wyższych częstotliwości i odwrotnie by dać ciekawy dźwięk
	for(x = 30; x < 40; x++) {
	    __HAL_TIM_SET_AUTORELOAD(&htim2, x * 2);
	    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, x);
	    HAL_Delay(50);
	  }
	for(x = 40; x < 30; x++) {
		__HAL_TIM_SET_AUTORELOAD(&htim2, x * 2);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, x);
		HAL_Delay(50);
	  }
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
}

void DisplayResult(){
	//Funkcja która pokazuje po kolei wyniki od najniższego do najwyższego rodzaju kości
	if(d4Number != 0){
		Display(4, d4Number);
		d4Number = 0;
	}
	else if(d6Number != 0){
		Display(6, d6Number);
		d6Number = 0;
	}
	else if(d8Number != 0){
		Display(8, d8Number);
		d8Number = 0;
	}
	else if(d10Number != 0){
		Display(10, d10Number);
		d10Number = 0;
	}
	else if(d12Number != 0){
		Display(12, d12Number);
		d12Number = 0;
	}
	else if(d20Number != 0){
		Display(20, d20Number);
		d20Number = 0;
	}
	else if(d100Number != 0){
		Display(100, d100Number);
		d100Number = 0;
	}

}
void RollDice(){
	//Głośnik gra muzykę by pokazać że wynik jest gotowy
	Buzz();
	//Pokazujemy pierwszą kość
	DisplayResult();
	while(d4Number != 0 || d6Number != 0 || d8Number != 0 || d10Number != 0 || d12Number != 0 || d20Number != 0 || d100Number != 0){

		if (HAL_GPIO_ReadPin(A0_GPIO_Port, A0_Pin) == 0) {
				DisplayResult();
		}
	}
	//Czekamy na ostatnie kliknięcie guzika funkcyjnego by przejść do menu głównego
	while(HAL_GPIO_ReadPin(A0_GPIO_Port, A0_Pin) != 0){}
		HomeScreen();


}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  SSD1306_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //Na początek pokazujemy home screen by zawsze był on pierwszą rzeczą jaką będzie widział użytkownik
  HomeScreen();
  int x = 0;
  //Inicjalizujemy przyśpieszomierz
  LIS3DH_Init(&hi2c2);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  //Czytamy i sumujemy dane z przyśpieszomierza zamieniając je na ogólne przyśpieszenie urządzenia
	  int16_t x_raw, y_raw, z_raw;
	  LIS3DH_ReadRawAcc(&hi2c2, &x_raw, &y_raw, &z_raw);

	  float ax = x_raw * 0.001f * 9.81f;
	  float ay = y_raw * 0.001f * 9.81f;
	  float az = z_raw * 0.001f * 9.81f;

	  float magnitude = sqrt(ax * ax + ay * ay + az * az);
	  float net_accel = fabs(magnitude - 9.81f);
	  //Jeżeli przyśpieszomierz odzyczyta odpowiednią szybkość rzucamy kośćmi które przygotowaliśmy wcześniej
	  if (net_accel > 10.0f) {
		  RollDice();
	  }


	  //Sprawdzamy czy jakikolwiek z guzików został kliknięty i update'ujemy home screen by dać instant feedback użytkownikowi
	if (HAL_GPIO_ReadPin(A1_GPIO_Port, A1_Pin) == 0) {
			d4Number++;
			HomeScreen();
	}
	if (HAL_GPIO_ReadPin(A2_GPIO_Port, A2_Pin) == 0) {
			d6Number++;
			HomeScreen();
	}
	if (HAL_GPIO_ReadPin(A3_GPIO_Port, A3_Pin) == 0) {
			d8Number++;
			HomeScreen();
	}
	if (HAL_GPIO_ReadPin(A4_GPIO_Port, A4_Pin) == 0) {
			d10Number++;
			HomeScreen();
	}
	if (HAL_GPIO_ReadPin(A5_GPIO_Port, A5_Pin) == 0) {
			d12Number++;
			HomeScreen();
	}
	if (HAL_GPIO_ReadPin(A6_GPIO_Port, A6_Pin) == 0) {
			d20Number++;
			HomeScreen();
	}
	if (HAL_GPIO_ReadPin(A7_GPIO_Port, A7_Pin) == 0) {
			d100Number++;
			HomeScreen();
	}
	HAL_Delay(200);
  }

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 127;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 20;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_HalfDuplex_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : A0_Pin A1_Pin A2_Pin A3_Pin
                           A4_Pin A5_Pin A6_Pin A7_Pin */
  GPIO_InitStruct.Pin = A0_Pin|A1_Pin|A2_Pin|A3_Pin
                          |A4_Pin|A5_Pin|A6_Pin|A7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : IA9_Pin */
  GPIO_InitStruct.Pin = IA9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IA9_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
