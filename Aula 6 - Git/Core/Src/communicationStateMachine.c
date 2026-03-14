/*
 * communicationStateMachine.c
 *
 *  Created on: Mar 12, 2026
 *      Author: aluno
 */


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

extern int getPoCap, getPoMoc, getPoCho, getPoExp; // Variaveis para verificar a quantidade disponível de cada um dos cafés
extern int getCap, getMoc, getCho, getExp; // Variaveis para verificar a quantidade, em unidades, vendidas separadamente ou juntas
extern int setZero;

static void CommunicationStateMachineReturnParam(unsigned char param) {
	char buffer [128];

	switch (param){
	case 'e':
		sprintf(buffer, " #a%u;\r\n", getExp);
		break;
	case 't':
		sprintf(buffer, " #a%u;\r\n", getCap);
		break;
	case 'm':
		sprintf(buffer, " #a%u;\r\n", getMoc);
		break;
	case 'c':
		sprintf(buffer, " #a%u;\r\n", getCho);
		break;
	case 'x':
		sprintf(buffer, " #a%u;\r\n", getPoExp);
		break;
	case 'h':
		sprintf(buffer, " #a%u;\r\n", getPoCho);
		break;
	case 'o':
		sprintf(buffer, " #a%u;\r\n", getPoMoc);
		break;
	case 'a':
		sprintf(buffer, " #a%u;\r\n", getPoCap);
		break;
	case 'l':
		sprintf(buffer, "\n\r#a Vendas Totais:\n\rCappucino: %u\r\nMoccha: %u\r\nChocolate: %u\r\nExpresso: %u\r\n", getCap, getMoc, getCho, getExp);
		break;

	default:
		return;
	}
	HAL_UART_Transmit(&hlpuart1, (uint8_t *) buffer, strlen(buffer), HAL_MAX_DELAY);
}

static void CommunicationStateMachineSetParam(unsigned char param, unsigned char *value) {
	int SetValue;
	char ConvertedValue[MAX_VALUE_LENGHT + 1];
	char ConfirmarMensagem[] = " Parametro atualizado!\r\n";

	strcpy(ConvertedValue, (char*) value);

	for (int i = 0; ConvertedValue[i] != '\0'; i++){
		if (ConvertedValue[i] == ',') {
			ConvertedValue[i] = '.';
		}
	}
	SetValue = atof(ConvertedValue);

	switch(param){
	case 'e':
		getPoExp = SetValue;
		HAL_UART_Transmit(&hlpuart1, (uint8_t*) ConfirmarMensagem, strlen(ConfirmarMensagem), HAL_MAX_DELAY);
		break;
	case 't':
		getPoCap = SetValue;
		HAL_UART_Transmit(&hlpuart1, (uint8_t*) ConfirmarMensagem, strlen(ConfirmarMensagem), HAL_MAX_DELAY);
		break;
	case 'm':
		getPoMoc = SetValue;
		HAL_UART_Transmit(&hlpuart1, (uint8_t*) ConfirmarMensagem, strlen(ConfirmarMensagem), HAL_MAX_DELAY);
		break;
	case 'c':
		getPoCho = SetValue;
		HAL_UART_Transmit(&hlpuart1, (uint8_t*) ConfirmarMensagem, strlen(ConfirmarMensagem), HAL_MAX_DELAY);
		break;

	default:
		return;
	}
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
