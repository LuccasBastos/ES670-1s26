/* USER CODE BEGIN Header */
/*
******************************************************************************
* @file    : main.c
* @brief   : Arquivo principal com a maquina de estados da maquina de cafe
******************************************************************************
* @author  : Lucca Bastos e Joao Vitor Roque
* @date    : 12-Mar-2026
* @version : 19-Mar-2026
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
#include <stdio.h>
#include <string.h>
#include "communicationStateMachine.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
  ESTADO_INICIAL, //IDLE
  ESTADO_SELECAO, // SELECTED
  ESTADO_PREPARANDO, //BREWING
  ESTADO_FINALIZANDO, //BIP
  ESTADO_ERRO // QUANTIDADE MÍNIMA AUSENTE
} xEstadoMaquina;

typedef enum {
  NENHUMA,
  EXPRESSO,    // CIMA: Amarelo
  CAPUCCINO,   // BAIXO: Vermelho
  MOCHA,       // ESQUERDA: Verde
  CHOCOLATE    // DIREITA: Azul
} xBebidaSelecionada;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned char ucRecBuff;
volatile xEstadoMaquina xEstadoAtual = ESTADO_INICIAL;
volatile xBebidaSelecionada xBebidaAtual = NENHUMA;
volatile uint16_t usContadorTempo = 0;

// Struct com os dados da maquina
MaquinaCafe_t xMaquinaDados = {
  .usCapacidadeMaxPo = 1000,
  .usPoAtualCap = 0,
  .usPoAtualMoc = 0,
  .usPoAtualCho = 0,
  .usPoAtualExp = 0,
  .ucVendasCap = 0,
  .ucVendasMoc = 0,
  .ucVendasCho = 0,
  .ucVendasExp = 0,
  .ucVendasTotais = 0
};
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

  // Inicia o timer base 6 com interrupcao
  HAL_TIM_Base_Start_IT(&htim6);

  // Inicia a recepção por interrupção (esperando 1 byte por vez)
  HAL_UART_Receive_IT(&hlpuart1, &ucRecBuff, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

/**
******************************************************************************
* @method : HAL_TIM_PeriodElapsedCallback
* @brief  : Gerencia o tempo de preparacao das bebidas e controle de hardware
* @param  : htim - Ponteiro para o handler do Timer
* @return : void
******************************************************************************
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  // Verifica se a interrupcao veio do TIM6
  if (htim->Instance == TIM6) {
    // Incrementa a variavel de controle de tempo em milissegundos
    usContadorTempo++;

    switch (xEstadoAtual) {
      case ESTADO_INICIAL:
        if (usContadorTempo >= 1000) {
          // Todos os leds piscam a cada 1 segundo aguardando selecao
          HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
          HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
          HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
          HAL_GPIO_TogglePin(LED_Y_GPIO_Port, LED_Y_Pin);
          // Zera o contador de tempo apos os leds piscarem
          usContadorTempo = 0;
        }
        break;

      case ESTADO_PREPARANDO:
        // Verifica se a preparacao da bebida ainda nao chegou a 20s
        if (usContadorTempo < 20000) {
          // A cada 1 segundo pisca o led correspondente
          if (usContadorTempo % 1000 == 0) {
            switch (xBebidaAtual) {
              case EXPRESSO:
                HAL_GPIO_TogglePin(LED_Y_GPIO_Port, LED_Y_Pin);
                break;
              case CAPUCCINO:
                HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
                break;
              case MOCHA:
                HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
                break;
              case CHOCOLATE:
                HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
                break;
              default:
                break;
            }
          }
        } else {
          switch (xBebidaAtual) {
            case EXPRESSO:
              xMaquinaDados.ucVendasExp++;
              xMaquinaDados.ucVendasTotais++;
              xMaquinaDados.usPoAtualExp -= 100;
              break;
            case CAPUCCINO:
              xMaquinaDados.ucVendasCap++;
              xMaquinaDados.ucVendasTotais++;
              xMaquinaDados.usPoAtualCap -= 100;
              break;
            case MOCHA:
              xMaquinaDados.ucVendasMoc++;
              xMaquinaDados.ucVendasTotais++;
              xMaquinaDados.usPoAtualMoc -= 100;
              break;
            case CHOCOLATE:
              xMaquinaDados.ucVendasCho++;
              xMaquinaDados.ucVendasTotais++;
              xMaquinaDados.usPoAtualCho -= 100;
              break;
            default:
              break;
          }

          // Apos os 20s, muda para o estado final e apaga os leds
          xEstadoAtual = ESTADO_FINALIZANDO;
          usContadorTempo = 0;
          HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET);
        }
        break;

      case ESTADO_FINALIZANDO:
        // Acionamento do buzzer simulando 3 bipes com intervalos de 500ms
        if (usContadorTempo == 1) {
          HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
        } else if (usContadorTempo == 500) {
          // Fim do primeiro bip
          HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
        } else if (usContadorTempo == 1000) {
          HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
        } else if (usContadorTempo == 1500) {
          // Fim do segundo bipe
          HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
        } else if (usContadorTempo == 2000) {
          HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
        } else if (usContadorTempo == 2500) {
          // Fim do terceiro bipe
          HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
        }
        if (usContadorTempo >= 3000) {
          // Finaliza o ciclo e retorna maquina para estado inicial
          HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
          xEstadoAtual = ESTADO_INICIAL;
          xBebidaAtual = NENHUMA;
          usContadorTempo = 0;
        }
        break;


      case ESTADO_ERRO:
        // Acionamento do buzzer 2 vezes rapidamente
        if (usContadorTempo == 1) { // Inicia bip 2
          HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
        } else if (usContadorTempo == 200) {
          HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1); // Para bip 1
        } else if (usContadorTempo == 400) {
          HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1); // Inicia bip 2
        } else if (usContadorTempo == 600) {
          HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1); // Para bip 2
        }

        // Após 1 segundo, retorna para estado inicial piscando as luzes
        if (usContadorTempo >= 1000) {
          HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
          xEstadoAtual = ESTADO_INICIAL;
          usContadorTempo = 0;
        }
        break;

      default:
        break;
    }
  }
}

/**
******************************************************************************
* @method : HAL_GPIO_EXTI_Callback
* @brief  : Identifica os botoes para selecao de bebidas e tocar o buzzer
* @param  : GPIO_Pin - Pino do microcontrolador que disparou a interrupcao
* @return : void
******************************************************************************
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  // Ignora o input dos botoes durante o preparo
  if (xEstadoAtual == ESTADO_PREPARANDO) return;

  GPIO_PinState xEstadoBotao;

  // Mapeia a leitura do botao com a respectiva porta GPIO
  switch (GPIO_Pin) {
    case BT_Cima_Pin:
      // PB6
      xEstadoBotao = HAL_GPIO_ReadPin(GPIOB, GPIO_Pin);
      break;
    default:
      // PC5, PC7, PC8, PC9
      xEstadoBotao = HAL_GPIO_ReadPin(GPIOC, GPIO_Pin);
      break;
  }

  // Aciona o buzzer enquanto o botao estiver sendo mantido pressionado
  if (xEstadoBotao == GPIO_PIN_SET) {
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
  } else {
    HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
  }

  // Verifica as funcoes de cada botao apenas no momento em que ele e acionado
  if (xEstadoBotao == GPIO_PIN_SET) {
    switch (GPIO_Pin) {
      case BT_Cima_Pin:
        // Seleciona Expresso e mantem o led amarelo aceso
        xBebidaAtual = EXPRESSO;
        xEstadoAtual = ESTADO_SELECAO;
        HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
        break;

      case BT_Baixo_Pin:
        // Seleciona Capuccino e mantem o led vermelho aceso
        xBebidaAtual = CAPUCCINO;
        xEstadoAtual = ESTADO_SELECAO;
        HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
        break;

      case BT_Esq_Pin:
        // Seleciona Mocha e mantem o led verde aceso
        xBebidaAtual = MOCHA;
        xEstadoAtual = ESTADO_SELECAO;
        HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
        break;

      case BT_Dir_Pin:
        // Seleciona Chocolate e mantem o led azul aceso
        xBebidaAtual = CHOCOLATE;
        xEstadoAtual = ESTADO_SELECAO;
        HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
        break;

      case BT_Enter_Pin:
        // Se uma bebida foi escolhida, o botao Enter inicia o preparo
        if ((xEstadoAtual == ESTADO_SELECAO) && (xBebidaAtual != NENHUMA)) {

          int poSuficiente = 0;

          if (xBebidaAtual == EXPRESSO && xMaquinaDados.usPoAtualExp >= 100) poSuficiente = 1;
          else if (xBebidaAtual == CAPUCCINO && xMaquinaDados.usPoAtualCap >= 100) poSuficiente = 1;
          else if (xBebidaAtual == MOCHA && xMaquinaDados.usPoAtualMoc >= 100) poSuficiente = 1;
          else if (xBebidaAtual == CHOCOLATE && xMaquinaDados.usPoAtualCho >= 100) poSuficiente = 1;

          if (poSuficiente == 1) {
            xEstadoAtual = ESTADO_PREPARANDO;
            usContadorTempo = 0;
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
          } else {
            // Sem pó suficiente! Cancela a operação e volta pro início.
            xEstadoAtual = ESTADO_ERRO;
            usContadorTempo = 0;
            xBebidaAtual = NENHUMA;

            // Apaga os LEDs para mostrar pro usuário que o pedido foi cancelado
            HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);

            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
          }

        }
        break;

      default:
        break;
    }
  }
}

/**
******************************************************************************
* @method : HAL_UART_RxCpltCallback
* @brief  : Callback chamado ao finalizar o recebimento por UART
* @param  : huart - Ponteiro do handler da UART
* @return : void
******************************************************************************
*/
void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart){
  if(huart == &hlpuart1)
  {
    HAL_UART_Transmit(&hlpuart1, &ucRecBuff, 1, 10);
    vCommunicationStateMachineProcessByteCommunication(ucRecBuff);
    HAL_UART_Receive_IT(&hlpuart1, &ucRecBuff, 1);
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
  * where the assert_param error has occurred.
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
