#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#include "Files.h"
#include "Misc.h"

/*
 * Verifica as informa��es do cart�o do usu�rio.
 *
 * @param clientSocket		soquete do cliente a ser verificado.
 * @param entry				inteiro que representa o tipo de entrada do usu�rio.
 * @param ticket_count		inteiro que representa a quantidade de ingressos comprados pelo usu�rio.
 * 
 * @return 0				caso as informa��es do cart�o forem v�lidas.
 */
int verifyCardInfo(SOCKET clientSocket, int entry, int ticket_count) {
	int iResult;
	char *card_number, *id, response[8], ticket_info[30], payment[10] = "payment1";

	loadLoadingScreen();

	/*
	 * Envia ao servidor o tipo de pagamento a ser verificado.
	 *
	 * @param clientSocket			soquete do cliente para enviar o tipo de pagamento.
	 * @param payment				string que representa o tipo de pagamento.
	 * @param sizeof(payment)		tamanho da string.
	 * @param 0						flag que modifica o comportamento da fun��o "send()".
	 */
	iResult = send(clientSocket, payment, sizeof(payment), 0);

	// Loop para verifica��o do n�mero do cart�o.
	while (1) {
		loadScreen(MENU_CARD_PAYMENT);

		/*
		 * Define a cor de fundo e do texto do console.
		 *
		 * @param GetStdHandle(STD_OUTPUT_HANDLE)		sa�da padr�o (console).
		 * @param 240									valor que representa as cores do console.
		 */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

		/*
		 * Define a posi��o do cursor do console.
		 *
		 * @param 69	coordenada "x" onde ser� posicionado o cursor.
		 * @param 24	coordenada "y" onde ser� posicionado o cursor.
		 */
		setCursorLocation(69, 24);
		printf("R$%d.00", ticket_count * 5);

		/*
		 * Solicita e recebe uma entrada de usu�rio e atribui ao ponteiro "card_number".
		 *
		 * @param 52	tamanho m�ximo da string.
		 * @param 69	coordenada "x" onde ser� posicionado o cursor.
		 * @param 28	coordenada "y" onde ser� posicionado o cursor.
		 */
		card_number = enterInput(52, 69, 28);

		/*
		 * Envia ao servidor o n�mero do cart�o do cliente.
		 *
		 * @param clientSocket				soquete do cliente para enviar o n�mero do cart�o do usu�rio.
		 * @param card_number				string que representa o n�mero do cart�o do cliente.
		 * @param strlen(card_number)		tamanho da string.
		 * @param 0							flag que modifica o comportamento da fun��o "send()".
		 */
		iResult = send(clientSocket, card_number, strlen(card_number), 0);

		/*
		 * Recebe do servidor a resposta da verifica��o.
		 *
		 * @param clientSocket			soquete do cliente para receber a resposta da verifica��o.
		 * @param response				string que representa a resposta do servidor.
		 * @param sizeof(response)		tamanho da string.
		 * @param 0						flag que modifica o comportamento da fun��o "recv()".
		 */
		iResult = recv(clientSocket, response, sizeof(response), 0);

		// Verifica a resposta recebida. Caso seja "failed", o loop � reiniciado.
		if(strcmp(response, "failed") == 0){

			// Desativa o cursor do console.
			setCursorView(0);
			loadScreen(MENU_CARD_PAYMENT_NUMBER_FAIL);
			Sleep(1500);
			card_number = NULL;
			free(card_number);
			loadLoadingScreen();
			continue;
		}
		break;
	}

	// Loop para verifica��o do n�mero de verifica��o do cart�o.
	while (1) {
		/*
		 * Define a cor de fundo e do texto do console.
		 *
		 * @param GetStdHandle(STD_OUTPUT_HANDLE)		sa�da padr�o (console).
		 * @param 240									valor que representa as cores do console.
		 */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

		/*
		 * Define a posi��o do cursor do console.
		 *
		 * @param 69	coordenada "x" onde ser� posicionado o cursor.
		 * @param 24	coordenada "y" onde ser� posicionado o cursor.
		 */
		setCursorLocation(69, 24);
		printf("R$%d.00", ticket_count * 5);

		/*
		 * Define a posi��o do cursor do console.
		 *
		 * @param 69	coordenada "x" onde ser� posicionado o cursor.
		 * @param 28	coordenada "y" onde ser� posicionado o cursor.
		 */
		setCursorLocation(69, 28);
		printf("%s", card_number);

		/*
		 * Solicita e recebe uma entrada de usu�rio e atribui ao ponteiro "id".
		 *
		 * @param 52	tamanho m�ximo da string.
		 * @param 69	coordenada "x" onde ser� posicionado o cursor.
		 * @param 32	coordenada "y" onde ser� posicionado o cursor.
		 */
		id = enterInput(52, 69, 32);

		/*
		 * Envia ao servidor o n�mero de verifica��o do cart�o do cliente.
		 *
		 * @param clientSocket		soquete do cliente para enviar o n�mero de verifica��o do cart�o do usu�rio.
		 * @param id				string que representa o n�mero de verifica��o do cart�o do usu�rio.
		 * @param strlen(id)		tamanho da string.
		 * @param 0					flag que modifica o comportamento da fun��o "send()".
		 */
		iResult = send(clientSocket, id, sizeof(id), 0);

		/*
		 * Recebe do servidor a resposta da verifica��o.
		 *
		 * @param clientSocket			soquete do cliente para receber a resposta da verifica��o.
		 * @param response				string que representa a resposta do servidor.
		 * @param sizeof(response)		tamanho da string.
		 * @param 0						flag que modifica o comportamento da fun��o "recv()".
		 */
		iResult = recv(clientSocket, response, sizeof(response), 0);

		// Verifica a resposta recebida. Caso seja "failed", o loop � reiniciado.
		if (strcmp(response, "failed") == 0) {

			// Desativa o cursor do console.
			setCursorView(0);
			loadScreen(MENU_CARD_PAYMENT_CVC_FAIL);
			Sleep(1500);
			loadLoadingScreen();
			id = NULL;
			free(id);
			continue;
		}
		break;
	}

	sprintf(ticket_info, "%.1f,%d,%d", 5.0 * ticket_count, entry, ticket_count);

	/*
	 * Envia ao servidor as informa��es do cart�o do usu�rio.
	 *
	 * @param clientSocket				soquete do cliente para enviar as informa��es do cart�o do usu�rio.
	 * @param ticket_info				string que representa as informa��es do cliente.
	 * @param sizeof(ticket_info)		tamanho da string.
	 * @param 0							flag que modifica o comportamento da fun��o "send()".
	 */
	iResult = send(clientSocket, ticket_info, sizeof(ticket_info), 0);

	free(id);
	free(card_number);
	loadScreen(MENU_CARD_PAYMENT_OK);
	Sleep(1500);
	return 0;
}

/*
 * Verifica as informa��es do pagamento por pix do usu�rio.
 *
 * @param clientSocket		soquete do cliente a ser verificado.
 * @param entry				inteiro que representa o tipo de entrada do usu�rio.
 * @param ticket_count		inteiro que representa a quantidade de ingressos comprados pelo usu�rio.
 *
 * @return 0				caso as informa��es do usu�rios forem v�lidas.
 */
int verifyPixInfo(SOCKET clientSocket, int entry, int ticket_count) {
	int iResult, discount = 0;
	char info[30], response[8], payment[10] = "payment2";
	float total;

	// Calcula o valor total a ser pago e aplica um desconto caso houver.
	switch (entry) {
	case 0:
		total = 5 * ticket_count;
		discount = 0;
		break;
	case 1:
		total = (5 * ticket_count) * 0.5;
		discount = 50;
		break;
	case 2:
		total = 0;
		discount = 100;
		break;
	}

	loadLoadingScreen();

	/*
	 * Envia ao servidor o tipo de pagamento a ser verificado.
	 *
	 * @param clientSocket			soquete do cliente para enviar o tipo de pagamento.
	 * @param payment				string que representa o tipo de pagamento.
	 * @param sizeof(payment)		tamanho da string.
	 * @param 0						flag que modifica o comportamento da fun��o "send()".
	 */
	iResult = send(clientSocket, payment, sizeof(payment), 0);
	
	// Loop para verifica��o do pagamento por pix.
	while (1) {	
		loadScreen(MENU_PIX_PAYMENT);

		/*
		 * Define a cor de fundo e do texto do console.
		 *
		 * @param GetStdHandle(STD_OUTPUT_HANDLE)		sa�da padr�o (console).
		 * @param 240									valor que representa as cores do console.
		 */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

		/*
		 * Define a posi��o do cursor do console.
		 *
		 * @param 69	coordenada "x" onde ser� posicionado o cursor.
		 * @param 24	coordenada "y" onde ser� posicionado o cursor.
		 */
		setCursorLocation(69, 24);
		printf("R$%.2f", total);

		/*
		 * Solicita e recebe uma entrada de usu�rio e atribui ao ponteiro "aux".
		 *
		 * @param 52	tamanho m�ximo da string.
		 * @param 69	coordenada "x" onde ser� posicionado o cursor.
		 * @param 28	coordenada "y" onde ser� posicionado o cursor.
		 */
		char* aux = enterInput(52, 69, 28);
		float amount = atoi(aux);
		sprintf(info, "%.1f,%d,%d", amount, entry, ticket_count);

		/*
		 * Envia ao servidor as informa��es do pagamento por pix.
		 *
		 * @param clientSocket		soquete do cliente para enviar as informa��es do pagamento por pix do usu�rio.
		 * @param info				string que representa as informa��es do pagamento por pix do usu�rio.
		 * @param sizeof(info)		tamanho da string.
		 * @param 0					flag que modifica o comportamento da fun��o "send()".
		 */
		iResult = send(clientSocket, info, sizeof(info), 0);

		/*
		 * Recebe do servidor a resposta da verifica��o.
		 *
		 * @param clientSocket			soquete do cliente para receber a resposta da verifica��o.
		 * @param response				string que representa a resposta do servidor.
		 * @param sizeof(response)		tamanho da string.
		 * @param 0						flag que modifica o comportamento da fun��o "recv()".
		 */
		iResult = recv(clientSocket, response, sizeof(response), 0);

		// Verifica a resposta recebida. Caso seja "failed", o loop � reiniciado.
		if (strcmp(response, "failed") == 0) {

			// Desativa o cursor do console.
			setCursorView(0);
			loadScreen(MENU_PIX_PAYMENT_FAIL);
			Sleep(1500);
			loadLoadingScreen();
			continue;
		}
		break;
	}

	setCursorView(0);
	loadScreen(MENU_PIX_PAYMENT_OK);
	Sleep(1500);
	return 0;
}

/*
 * Verifica qual pagamento o usu�rio escolheu.
 *
 * @param clientSocket		soquete do cliente a ser verificado.
 * @param entry				inteiro que representa o tipo de entrada do usu�rio.
 * @param ticket_count		inteiro que representa a quantidade de ingressos comprados pelo usu�rio.
 *
 * @return 0				caso o pagamento seja verificado com �xito.
 */
int selectPayment(SOCKET clientSocket, int entry, int ticket_count) {
	int key;
	loadScreen(MENU_PAYMENT);

	// Verifica se as informa��es que o usu�rio entrou s�o validas.
	while (_kbhit() == NULL) {
		key = _getch();
		switch (key) {
		case 49:
			if (verifyCardInfo(clientSocket, entry, ticket_count) == 0) {
				break;
			}
		case 50:
			if (verifyPixInfo(clientSocket, entry, ticket_count) == 0) {
				break;
			}
		default:
			continue;
		}
		break;
	}

	return 0;
}