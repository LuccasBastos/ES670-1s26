/**
******************************************************************************
* @file    : communicationStateMachine.c
* @brief   : Realiza a comunicacao com o sistema. E possivel ver a
* quantidade de po de cada bebida, vendas de cada bebida e
* vendas totais.
******************************************************************************
* @author  : Luccas Bastos e Joao Vitor Roque
* @date    : 12-Mar-2026
* @version : 19-Mar-2026
******************************************************************************
*/

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usart.h"

#define IDLE '0'
#define READY '1'
#define RESET '2'
#define GET '3'
#define SET '4'
#define PARAM '5'
#define VALUE '6'

#define MAX_VALUE_LENGTH 7

unsigned char ucUartState = IDLE;
unsigned char ucValueCount;

// struct que possui todas as variaveis necessarias
extern MaquinaCafe_t xMaquinaDados;

/*
******************************************************************************
* @method : vCommunicationStateMachineReturnParam
* @brief  : Retorna o valor de um parametro especifico via UART
* @param  : ucParam - caractere que representa o parametro desejado
* @return : void
******************************************************************************
*/
static void vCommunicationStateMachineReturnParam(unsigned char ucParam) {
  char cBuffer[128];

  switch (ucParam) {
    case 'e':
      //Vendas de expresso
      sprintf(cBuffer, " #a%u;\r\n", xMaquinaDados.ucVendasExp);
      break;
    case 't':
      //Vendas de cappuccino
      sprintf(cBuffer, " #a%u;\r\n", xMaquinaDados.ucVendasCap);
      break;
    case 'm':
      //Vendas de mocha
      sprintf(cBuffer, " #a%u;\r\n", xMaquinaDados.ucVendasMoc);
      break;
    case 'c':
      //Vendas de chocolate
      sprintf(cBuffer, " #a%u;\r\n", xMaquinaDados.ucVendasCho);
      break;
    case 'x':
      //Quantidade de pó de expresso
      sprintf(cBuffer, " #a%u;\r\n", xMaquinaDados.usPoAtualExp);
      break;
    case 'h':
      //Quantidade de pó de chocolate
      sprintf(cBuffer, " #a%u;\r\n", xMaquinaDados.usPoAtualCho);
      break;
    case 'o':
      //Quantidade de pó de mocha
      sprintf(cBuffer, " #a%u;\r\n", xMaquinaDados.usPoAtualMoc);
      break;
    case 'a':
      //Quantidade de pó de cappuccino
      sprintf(cBuffer, " #a%u;\r\n", xMaquinaDados.usPoAtualCap);
      break;
    case 'l':
      //Vendas totais
      sprintf(cBuffer, "\n\r#a Vendas Totais:\n\rCappucino: %u\r\n"
              "Moccha: %u\r\nChocolate: %u\r\nExpresso: %u\r\n",
              xMaquinaDados.ucVendasCap, xMaquinaDados.ucVendasMoc,
              xMaquinaDados.ucVendasCho, xMaquinaDados.ucVendasExp);
      break;
    default:
      return;
  }
  HAL_UART_Transmit(&hlpuart1, (uint8_t *) cBuffer, strlen(cBuffer),
                    HAL_MAX_DELAY);
}

/*
******************************************************************************
* @method : vCommunicationStateMachineSetParam
* @brief  : Configura o valor de um parametro especifico
* @param  : ucParam - parametro a ser configurado
* @param  : pucValue - ponteiro para o valor a ser atribuido
* @return : void
******************************************************************************
*/
static void vCommunicationStateMachineSetParam(unsigned char ucParam,
                                               unsigned char *pucValue) {
  int iSetValue;
  char cConvertedValue[MAX_VALUE_LENGTH + 1];
  char cConfirmarMensagem[] = " #a\r\n";
  char cErroMensagem[] = "capacidade excedida, coloque de 0 a 1000g\r\n";

  strcpy(cConvertedValue, (char*) pucValue);

  for (int i = 0; cConvertedValue[i] != '\0'; i++) {
    if (cConvertedValue[i] == ',') {
      cConvertedValue[i] = '.';
    }
  }
  iSetValue = atof(cConvertedValue);

  if (iSetValue > xMaquinaDados.usCapacidadeMaxPo) {
    // Envia a mensagem de erro pro PuTTY
    HAL_UART_Transmit(&hlpuart1, (uint8_t*) cErroMensagem,
                      strlen(cErroMensagem), HAL_MAX_DELAY);
    // O "return" faz a funcao ser encerrada aqui mesmo
    return;
  }

  switch(ucParam) {
    case 'e':
      //Quantidade de pó de expresso
      xMaquinaDados.usPoAtualExp = iSetValue;
      HAL_UART_Transmit(&hlpuart1, (uint8_t*) cConfirmarMensagem,
                        strlen(cConfirmarMensagem), HAL_MAX_DELAY);
      break;
    case 't':
      //Quantidade de pó de cappuccino
      xMaquinaDados.usPoAtualCap = iSetValue;
      HAL_UART_Transmit(&hlpuart1, (uint8_t*) cConfirmarMensagem,
                        strlen(cConfirmarMensagem), HAL_MAX_DELAY);
      break;
    case 'm':
      //Quantidade de pó de mocha
      xMaquinaDados.usPoAtualMoc = iSetValue;
      HAL_UART_Transmit(&hlpuart1, (uint8_t*) cConfirmarMensagem,
                        strlen(cConfirmarMensagem), HAL_MAX_DELAY);
      break;
    case 'c':
      //Quantidade de pó de chocolate
      xMaquinaDados.usPoAtualCho = iSetValue;
      HAL_UART_Transmit(&hlpuart1, (uint8_t*) cConfirmarMensagem,
                        strlen(cConfirmarMensagem), HAL_MAX_DELAY);
      break;
    default:
      return;
  }
}

/*
******************************************************************************
* @method : vCommunicationStateMachineReset
* @brief  : Zera os contadores de vendas e confirma a operacao
* @param  : void
* @return : void
******************************************************************************
*/
static void vCommunicationStateMachineReset(void) {
  // Zera os contadores de vendas da struct global
  xMaquinaDados.ucVendasExp = 0;
  xMaquinaDados.ucVendasCap = 0;
  xMaquinaDados.ucVendasMoc = 0;
  xMaquinaDados.ucVendasCho = 0;

  // Mensagem de confirmacao padrao
  char cConfirmarMensagem[] = "#a;\r\n";

  // Transmite a confirmacao para o PuTTY
  HAL_UART_Transmit(&hlpuart1, (uint8_t*)cConfirmarMensagem,
                    strlen(cConfirmarMensagem), HAL_MAX_DELAY);
}

/*
******************************************************************************
* @method : vCommunicationStateMachineProcessByteCommunication
* @brief  : Processa a maquina de estados recebendo byte a byte
* @param  : ucByte - byte recebido pela UART
* @return : void
******************************************************************************
*/
void vCommunicationStateMachineProcessByteCommunication(unsigned char ucByte) {
  static unsigned char ucParam;
  static unsigned char ucValue[MAX_VALUE_LENGTH + 1];

  if ('#' == ucByte) {
    ucUartState = READY;
  } else {
    if (IDLE != ucUartState) {
      switch (ucUartState) {
        case READY:
          switch (ucByte) {
            case 'z':
              ucUartState = RESET;
              break;
            case 'g':
              ucUartState = GET;
              break;
            case 's':
              ucUartState = SET;
              break;
            default:
              ucUartState = IDLE;
              break;
          }
          break;

        case RESET:
          if (ucByte == ';') {
            vCommunicationStateMachineReset();
            ucUartState = IDLE;
          } else {
            ucUartState = IDLE;
          }
          break;

        case GET:
          if ('e' == ucByte || 't' == ucByte || 'm' == ucByte ||
              'c' == ucByte || 'l' == ucByte || 'x' == ucByte ||
              'h' == ucByte || 'o' == ucByte || 'a' == ucByte) {
            ucParam = ucByte;
            ucUartState = PARAM;
          } else {
            ucUartState = IDLE;
          }
          break;

        case SET:
          if ('e' == ucByte || 't' == ucByte || 'm' == ucByte ||
              'c' == ucByte) {
            ucParam = ucByte;
            ucValueCount = 0;
            ucUartState = VALUE;
          } else {
            ucUartState = IDLE;
          }
          break;

        case PARAM:
          if (';' == ucByte) {
            vCommunicationStateMachineReturnParam(ucParam);
            ucUartState = IDLE;
          }
          break;

        case VALUE:
          if ((ucByte >= '0' && ucByte <= '9') || ',' == ucByte) {
            if (ucValueCount < MAX_VALUE_LENGTH) {
              ucValue[ucValueCount++] = ucByte;
            }
          } else {
            if (';' == ucByte) {
              ucValue[ucValueCount] = '\0';
              vCommunicationStateMachineSetParam(ucParam, ucValue);
            } else {
              ucUartState = IDLE;
            }
          }
          break;
      }
    }
  }
}
