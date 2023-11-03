#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>

#include "FileProcess.h"
#include "Server.h"

/* Arquivo com funções para verificação de pagamento */

int verifyCardPayment(SOCKET clientSocket) {
	int iResult, count = 0;
	char payment_info[100], ticket_info[100], * tokens[3], * token, card_number[53], id[53];
	float discount = 0;

	while (1) {
		iResult = recv(clientSocket, card_number, sizeof(card_number), 0);
		if (iResult == SOCKET_ERROR) {
			sendFailMessage(clientSocket);
			continue;
		}

		card_number[iResult] = '\0';

		if (strlen(card_number) == 12) {
			iResult = send(clientSocket, "success", 8, 0);
			break;
		}
		else {
			iResult = send(clientSocket, "failed", 7, 0);
			continue;
		}
		break;
	}

	while (1) {
		iResult = recv(clientSocket, id, sizeof(id), 0);
		if (iResult == SOCKET_ERROR) {
			sendFailMessage(clientSocket);
			continue;
		}

		id[iResult] = '\0';

		if (strlen(id) == 3) {
			iResult = send(clientSocket, "success", 8, 0);
			break;
		}
		else {
			iResult = send(clientSocket, "failed", 7, 0);
			continue;
		}
		break;
	}

	iResult = recv(clientSocket, ticket_info, sizeof(ticket_info), 0);

	ticket_info[iResult] = '\0';
	token = strtok(ticket_info, ",");

	while (token != NULL && count < 3) {
		tokens[count] = token;
		count++;
		token = strtok(NULL, ",");
	}

	float amount = atof(tokens[0]);
	int entry = atoi(tokens[1]);
	int ticket_count = atoi(tokens[2]);

	switch (entry) {
	case 0:
		discount = 0;
		break;
	case 1:
		discount = 50;
		break;
	case 2:
		discount = 100;
		break;
	}

	sprintf(ticket_info, ",%d,%.2f,%.2f,Cartao\0", ticket_count, amount, discount);
	sprintf(payment_info, "%d,%d,%.2f", entry, ticket_count, amount - (amount * (discount / 100)));

	appendToFile("server_resources\\ticket_info_temp.csv", ticket_info);

	appendToFile("server_resources\\stats\\daily\\gate_stats.txt", payment_info);
	appendToFile("server_resources\\stats\\all\\gate_stats.txt", payment_info);

	return 0;
}

int verifyPixPayment(SOCKET clientSocket) {
	int iResult, count = 0, ticket_count, entry;
	char payment_info[100], info[30], ticket_info[100], * tokens[3], * token;
	float discount = 0.0, amount = 0.0, total;

	while (1) {
		iResult = recv(clientSocket, info, sizeof(info), 0);

		token = strtok(info, ",");

		while (token != NULL && count < 3) {
			tokens[count] = token;
			count++;
			token = strtok(NULL, ",");
		}

		amount = atof(tokens[0]);
		entry = atoi(tokens[1]);
		ticket_count = atoi(tokens[2]);

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

		if (amount < total) {
			iResult = send(clientSocket, "failed", 7, 0);
			continue;
		}

		sprintf(ticket_info, ",%d,%.2f,%.2f,   Pix\0", ticket_count, amount, discount);

		break;
	}

	iResult = send(clientSocket, "success", 8, 0);
	if (iResult == SOCKET_ERROR) {
		sendFailMessage(clientSocket);
	}

	appendToFile("server_resources\\ticket_info_temp.csv", ticket_info);

	sprintf(payment_info, "%d,%d,%.2f", entry, ticket_count, amount - (amount * (discount / 100)));

	appendToFile("server_resources\\stats\\daily\\gate_stats.txt", payment_info);
	appendToFile("server_resources\\stats\\all\\gate_stats.txt", payment_info);

	return 0;
}

int paymentHandler(SOCKET clientSocket) {
	int iResult;
	char payment[11];

	while (1) {
		iResult = recv(clientSocket, payment, sizeof(payment), 0);
		if (iResult == SOCKET_ERROR) {
			logError("Invalid socket");
			continue;
		}
		payment[iResult] = '\0';

		if (iResult > 0) {
			if (strcmp(payment, "payment1") == 0) {
				if (verifyCardPayment(clientSocket) == 0) {
					return 0;
				}
			}
			else if (strcmp(payment, "payment2") == 0) {
				if (verifyPixPayment(clientSocket) == 0) {
					return 0;
				}
			}
		}
	}

	return 1;
}
