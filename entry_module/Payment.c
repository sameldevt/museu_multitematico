#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#include "Files.h"
#include "Misc.h"

int verifyCardInfo(SOCKET clientSocket, int entry, int ticket_count) {
	int iResult;
	char *card_number, *id, response[8], ticket_info[30], payment[10] = "payment1";

	loadLoadingScreen();

	iResult = send(clientSocket, payment, sizeof(payment), 0);

	while (1) {
		loadScreen(MENU_CARD_PAYMENT);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
		setCursorLocation(69, 24);
		printf("R$%d.00", ticket_count * 5);

		card_number = enterInput(53, 69, 28);

		iResult = send(clientSocket, card_number, strlen(card_number), 0);

		iResult = recv(clientSocket, response, sizeof(response), 0);

		if(strcmp(response, "failed") == 0){
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

	while (1) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
		setCursorLocation(69, 24);
		printf("R$%d.00", ticket_count * 5);
		setCursorLocation(69, 28);
		printf("%s", card_number);

		id = enterInput(53, 69, 32);

		iResult = send(clientSocket, id, sizeof(id), 0);

		iResult = recv(clientSocket, response, sizeof(response), 0);

		if (strcmp(response, "failed") == 0) {
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

	iResult = send(clientSocket, ticket_info, sizeof(ticket_info), 0);

	free(id);
	free(card_number);
	loadScreen(MENU_CARD_PAYMENT_OK);
	Sleep(1500);
	return 0;
}

int verifyPixInfo(SOCKET clientSocket, int entry, int ticket_count) {
	int iResult, discount = 0;
	char info[30], response[8], payment[10] = "payment2";
	float total;

	switch (entry) {
	case 0:
		total = (5 * ticket_count) * 1;
		discount = 100;
		break;
	case 1:
		total = (5 * ticket_count) * (50 / 100);
		discount = 50;
		break;
	case 2:
		total = 0;
		discount = 0;
		break;
	}

	loadLoadingScreen();

	iResult = send(clientSocket, payment, sizeof(payment), 0);
	
	while (1) {	
		loadScreen(MENU_PIX_PAYMENT);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
		setCursorLocation(69, 24);
		printf("R$%.2f", total);

		char* aux = enterInput(53, 69, 28);
		float amount = atoi(aux);
		sprintf(info, "%.1f,%d,%d", amount, entry, ticket_count);

		iResult = send(clientSocket, info, sizeof(info), 0);

		iResult = recv(clientSocket, response, sizeof(response), 0);

		if (strcmp(response, "failed") == 0) {
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

int selectPayment(SOCKET clientSocket, int entry, int ticket_count) {
	int key;
	loadScreen(MENU_PAYMENT);

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