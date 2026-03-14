/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "usart.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
	typedef enum {
		  ESTADO_INICIAL, //IDLE
		  ESTADO_SELECAO, // SELECTED
		  ESTADO_PREPARANDO, //BREWING
		  ESTADO_FINALIZANDO //BIP
	  } EstadoMaquina;

	 typedef enum {
		  NENHUMA,
		  EXPRESSO,    // CIMA: Amarelo
		  CAPUCCINO,   // BAIXO: Vermelho
		  MOCHA,       // ESQUERDA: Verde
		  CHOCOLATE    // DIREITA: Azul
	  } BebidaSelecionada;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
  volatile EstadoMaquina estado_atual = ESTADO_INICIAL;
  volatile BebidaSelecionada bebida_atual = NENHUMA;
  volatile uint16_t contador_tempo = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_LPUART1_UART_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM2_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim6);
  //HAL_TIM_Base_Start_IT(&htim7);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /*
	  Atividade 1 - Aula 1
	  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	  HAL_Delay(500);


	  //Atividade 2 - Aula 1
	  if (HAL_GPIO_ReadPin(BT_Enter_GPIO_Port, BT_Enter_Pin) == 1)
	  {
		  HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
		  HAL_Delay(500);
	  }


	  // Atividade 3 - Aula 1
	  HAL_GPIO_TogglePin(LED_Y_GPIO_Port, LED_Y_Pin);
	  HAL_Delay(500);

	  while(HAL_GPIO_ReadPin(BT_Cima_GPIO_Port, BT_Cima_Pin)==1){
		  HAL_GPIO_TogglePin(Buzzer_GPIO_Port, Buzzer_Pin);
		  HAL_Delay(2);
	  }*/

	  // Atividade 1 - Aula 2

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM6) {
        contador_tempo++;

        switch (estado_atual) {
            case ESTADO_INICIAL:
                if (contador_tempo >= 1000) {
                    HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
                    HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
                    HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
                    HAL_GPIO_TogglePin(LED_Y_GPIO_Port, LED_Y_Pin); // Todos os leds piscam ao mesmo tempo antes da seleção da bebida escolhida
                    contador_tempo = 0;
                }
                break;

            case ESTADO_PREPARANDO:
                if (contador_tempo < 20000) {
                    if (contador_tempo % 1000 == 0) {
                        switch (bebida_atual) {
                            case EXPRESSO:  HAL_GPIO_TogglePin(LED_Y_GPIO_Port, LED_Y_Pin); break;
                            case CAPUCCINO: HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin); break;
                            case MOCHA:     HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin); break;
                            case CHOCOLATE: HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin); break; // Pisca o led referente à escolha da bebida durante a confecção
                            default: break;
                        }
                    }
                } else {
                    estado_atual = ESTADO_FINALIZANDO;
                    contador_tempo = 0;
                    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET); // Desliga todos os leds ao término da preparação da bebida
                }
                break;

            case ESTADO_FINALIZANDO:
				if (contador_tempo == 1)         HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
				else if (contador_tempo == 500)  HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1); // 1º Bip (0 a 500ms)

				else if (contador_tempo == 1000) HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
				else if (contador_tempo == 1500) HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1); // 2º Bip (1000 a 1500ms)

				else if (contador_tempo == 2000) HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
				else if (contador_tempo == 2500) HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1); // 3º Bip (2000 a 2500ms)

				if (contador_tempo >= 3000) { // Fim do ciclo aos 3 segundos
					HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1); // Garantia física que o buzzer está mudo
					estado_atual = ESTADO_INICIAL;
					bebida_atual = NENHUMA;
					contador_tempo = 0;
				}
				break;

            default:
                break;
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (estado_atual == ESTADO_PREPARANDO) return;

    GPIO_PinState estado_botao;

    // Identifica qual porta ler usando switch
    switch (GPIO_Pin) {
        case BT_Cima_Pin: // PB6
            estado_botao = HAL_GPIO_ReadPin(GPIOB, GPIO_Pin);
            break;
        default:          // PC5, PC7, PC8, PC9
            estado_botao = HAL_GPIO_ReadPin(GPIOC, GPIO_Pin);
            break;
    }

    // Controle do Buzzer para quando for pressionado um botão o buzzer ser acionado junto
    if (estado_botao == GPIO_PIN_SET) {
        HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    } else {
        HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
    }

    if (estado_botao == GPIO_PIN_SET) {
        switch (GPIO_Pin) {
            case BT_Cima_Pin: // Pressiono o botão Cima e o led amarelo para de piscar, enquanto o restante fica apagado
                bebida_atual = EXPRESSO;
                estado_atual = ESTADO_SELECAO;
                HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
                break;

            case BT_Baixo_Pin: // Pressiono o botão Baixo e o led vermelho para de piscar, enquanto o restante fica apagado
                bebida_atual = CAPUCCINO;
                estado_atual = ESTADO_SELECAO;
                HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
                break;

            case BT_Esq_Pin: // Pressiono o botão Esquerda e o led verde para de piscar, enquanto o restante fica apagado
                bebida_atual = MOCHA;
                estado_atual = ESTADO_SELECAO;
                HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
                break;

            case BT_Dir_Pin: // Pressiono o botão Direita e o led azul para de piscar, enquanto o restante fica apagado
                bebida_atual = CHOCOLATE;
                estado_atual = ESTADO_SELECAO;
                HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
                break;

            case BT_Enter_Pin: // Ao pressionar o botão Enter a seleção da bebida é realizada e o buzzer é forçado a parar
                if (estado_atual == ESTADO_SELECAO && bebida_atual != NENHUMA) {
                    estado_atual = ESTADO_PREPARANDO;
                    contador_tempo = 0;

					HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
                }
                break;

            default:
                break;
        }
    }
}
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
