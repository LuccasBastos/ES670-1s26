/**
********************************************************************************
 * @file    : communicationStateMachine.c
 * @brief   : Realiza a comunicação com o sistema. É possível ver a
  * 		  quantidade de pó de cada bebida, vendas de cada bebida e
  * 		  vendas totais.
 * @author  : João Vitor Roque Ribeiro & Luccas Pereira Bastos
 * @date    : 17-Mar-2026
 * @version : 1.0
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

#define MAX_VALUE_LENGHT 7

unsigned char UartState = IDLE;
unsigned char ValueCount;

// struct que possui todas as variáveis necessárias
extern MaquinaCafe_t xMaquinaDados;

static void CommunicationStateMachineReturnParam(unsigned char param) {
	char buffer [128];

	switch (param){
	case 'e':
		sprintf(buffer, " #a%u;\r\n", xMaquinaDados.ucVendasExp);
		break;
	case 't':
		sprintf(buffer, " #a%u;\r\n", xMaquinaDados.ucVendasCap);
		break;
	case 'm':
		sprintf(buffer, " #a%u;\r\n", xMaquinaDados.ucVendasMoc);
		break;
	case 'c':
		sprintf(buffer, " #a%u;\r\n", xMaquinaDados.ucVendasCho);
		break;
	case 'x':
		sprintf(buffer, " #a%u;\r\n", xMaquinaDados.usPoAtualExp);
		break;
	case 'h':
		sprintf(buffer, " #a%u;\r\n", xMaquinaDados.usPoAtualCho);
		break;
	case 'o':
		sprintf(buffer, " #a%u;\r\n", xMaquinaDados.usPoAtualMoc);
		break;
	case 'a':
		sprintf(buffer, " #a%u;\r\n", xMaquinaDados.usPoAtualCap);
		break;
	case 'l':
		sprintf(buffer, "\n\r#a Vendas Totais:\n\rCappucino: %u\r\nMoccha: %u\r\nChocolate: %u\r\nExpresso: %u\r\n",
                xMaquinaDados.ucVendasCap,
                xMaquinaDados.ucVendasMoc,
                xMaquinaDados.ucVendasCho,
                xMaquinaDados.ucVendasExp);
		break;

	default:
		return;
	}
	HAL_UART_Transmit(&hlpuart1, (uint8_t *) buffer, strlen(buffer), HAL_MAX_DELAY);
}

static void CommunicationStateMachineSetParam(unsigned char param, unsigned char *value) {
	int SetValue;
	char ConvertedValue[MAX_VALUE_LENGHT + 1];
	char cConfirmarMensagem[] = " #a\r\n";
	char cErroMensagem[] = "capacidade excedida, coloque de 0 a 1000g\r\n";

	strcpy(ConvertedValue, (char*) value);

	for (int i = 0; ConvertedValue[i] != '\0'; i++){
		if (ConvertedValue[i] == ',') {
			ConvertedValue[i] = '.';
		}
	}
	SetValue = atof(ConvertedValue);

	if (SetValue > xMaquinaDados.usCapacidadeMaxPo) {
	        // Envia a mensagem de erro pro PuTTY
	        HAL_UART_Transmit(&hlpuart1, (uint8_t*) cErroMensagem, strlen(cErroMensagem), HAL_MAX_DELAY);

	        // O "return" faz a função ser encerrada aqui mesmo,
	        return;
	    }

	switch(param){
	case 'e':
		xMaquinaDados.usPoAtualExp = SetValue;
		HAL_UART_Transmit(&hlpuart1, (uint8_t*) cConfirmarMensagem, strlen(cConfirmarMensagem), HAL_MAX_DELAY);
		break;
	case 't':
		xMaquinaDados.usPoAtualCap = SetValue;
		HAL_UART_Transmit(&hlpuart1, (uint8_t*) cConfirmarMensagem, strlen(cConfirmarMensagem), HAL_MAX_DELAY);
		break;
	case 'm':
		xMaquinaDados.usPoAtualMoc = SetValue;
		HAL_UART_Transmit(&hlpuart1, (uint8_t*) cConfirmarMensagem, strlen(cConfirmarMensagem), HAL_MAX_DELAY);
		break;
	case 'c':
		xMaquinaDados.usPoAtualCho = SetValue;
		HAL_UART_Transmit(&hlpuart1, (uint8_t*) cConfirmarMensagem, strlen(cConfirmarMensagem), HAL_MAX_DELAY);
		break;
	default:
		return;
	}
}

/*
******************************************************************************
* @method : CommunicationStateMachineReset
* @brief  : Zera os contadores de vendas e confirma a operacao
* @param  : void
* @return : void
******************************************************************************
*/
static void CommunicationStateMachineReset(void) {
    // Zera os contadores de vendas da struct global
    xMaquinaDados.ucVendasExp = 0;
    xMaquinaDados.ucVendasCap = 0;
    xMaquinaDados.ucVendasMoc = 0;
    xMaquinaDados.ucVendasCho = 0;
    xMaquinaDados.ucVendasTotais = 0; // ADICIONADO: Zerando também o total

    // Mensagem de confirmacao padrão
    char cConfirmarMensagem[] = "#a;\r\n";

    // Transmite a confirmacao para o PuTTY
    HAL_UART_Transmit(&hlpuart1, (uint8_t*)cConfirmarMensagem, strlen(cConfirmarMensagem), HAL_MAX_DELAY);
}


void CommunicationStateMachineProcessByteCommunication(unsigned char Byte){
	static unsigned char Param;
	static unsigned char Value[MAX_VALUE_LENGHT + 1];

	if ('#' == Byte){
		UartState = READY;
	} else {
		if (IDLE != UartState){
			switch (UartState){
			case READY:
				switch (Byte){
				case 'z':
					UartState = RESET;
					break;
				case 'g':
					UartState = GET;
					break;
				case 's':
					UartState = SET;
					break;
				default:
					UartState = IDLE;
					break;
				}
				break;

			case RESET:
 				if (Byte == ';') {
       				CommunicationStateMachineReset();
       				UartState = IDLE;
   				} else {
       				UartState = IDLE;
   				}
 				break;

			case GET:
				if('e' == Byte || 't' == Byte || 'm' == Byte || 'c' == Byte || 'l' == Byte || 'x' == Byte || 'h' == Byte || 'o' == Byte || 'a' == Byte){
					Param = Byte;
					UartState = PARAM;
				} else {
					UartState = IDLE;
				}
				break;

			case SET:
				if('e' == Byte || 't' == Byte || 'm' == Byte || 'c' == Byte || 'z' == Byte){
					Param = Byte;
					ValueCount = 0;
					UartState = VALUE;
				} else {
					UartState = IDLE;
				}
				break;

			case PARAM:
				if (';' == Byte){
					CommunicationStateMachineReturnParam(Param);
				    UartState = IDLE;
				}
				break;

			case VALUE:
				if ((Byte >= '0' && Byte <= '9') || ',' == Byte) {
					if(ValueCount < MAX_VALUE_LENGHT)
						Value[ValueCount++] = Byte;
				} else {
					if(';' == Byte) {
						Value[ValueCount] = '\0';
						CommunicationStateMachineSetParam(Param, Value);
					} else {
						UartState = IDLE;
					}
				}
				break;
			}
		}
	}
}
