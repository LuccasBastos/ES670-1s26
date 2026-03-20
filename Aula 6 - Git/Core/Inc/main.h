/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file    : main.h
* @brief   : Cabecalho principal do programa da maquina de cafe
******************************************************************************
* @author  : Luccas Bastos e Joao Vitor Roque
* @date    : 12-Mar-2026
* @version : 19-Mar-2026
******************************************************************************
*/
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
// Definição da estrutura de dados da máquina de café
typedef struct {
    unsigned short int usCapacidadeMaxPo; // Capacidade máxima (1000)

    // Quantidade atual de pó
    unsigned short int usPoAtualCap;
    unsigned short int usPoAtualMoc;
    unsigned short int usPoAtualCho;
    unsigned short int usPoAtualExp;

    // Quantidade vendida individualmente
    unsigned char ucVendasCap;
    unsigned char ucVendasMoc;
    unsigned char ucVendasCho;
    unsigned char ucVendasExp;

    // Quantidade vendida no total
    unsigned char ucVendasTotais;
} MaquinaCafe_t;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define LPUART1_TX_Pin GPIO_PIN_2
#define LPUART1_TX_GPIO_Port GPIOA
#define LPUART1_RX_Pin GPIO_PIN_3
#define LPUART1_RX_GPIO_Port GPIOA
#define LED_Y_Pin GPIO_PIN_5
#define LED_Y_GPIO_Port GPIOA
#define BT_Enter_Pin GPIO_PIN_5
#define BT_Enter_GPIO_Port GPIOC
#define BT_Enter_EXTI_IRQn EXTI9_5_IRQn
#define BT_Baixo_Pin GPIO_PIN_7
#define BT_Baixo_GPIO_Port GPIOC
#define BT_Baixo_EXTI_IRQn EXTI9_5_IRQn
#define BT_Esq_Pin GPIO_PIN_8
#define BT_Esq_GPIO_Port GPIOC
#define BT_Esq_EXTI_IRQn EXTI9_5_IRQn
#define BT_Dir_Pin GPIO_PIN_9
#define BT_Dir_GPIO_Port GPIOC
#define BT_Dir_EXTI_IRQn EXTI9_5_IRQn
#define LED_R_Pin GPIO_PIN_11
#define LED_R_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_12
#define LED_G_GPIO_Port GPIOA
#define T_SWDIO_Pin GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
#define Buzzer_Pin GPIO_PIN_15
#define Buzzer_GPIO_Port GPIOA
#define T_SWO_Pin GPIO_PIN_3
#define T_SWO_GPIO_Port GPIOB
#define BT_Cima_Pin GPIO_PIN_6
#define BT_Cima_GPIO_Port GPIOB
#define BT_Cima_EXTI_IRQn EXTI9_5_IRQn
#define LED_B_Pin GPIO_PIN_8
#define LED_B_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
