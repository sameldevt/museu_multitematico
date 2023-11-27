#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <time.h>

#include "FileProcess.h"
#include "Server.h"

/*
 * Verifica as informações do cartão do usuário.
 *
 * @param clientSocket		soquete do cliente a ser verificado.
 * @param entry				inteiro que representa o tipo de entrada do usuário.
 * @param ticket_count		inteiro que representa a quantidade de ingressos comprados pelo usuário.
 *
 * @return 0				caso as informações do cartão forem válidas.
 */
int verifyCardPayment(SOCKET clientSocket) {
	int iResult, count = 0;
	char payment_info[100], ticket_info[100], * tokens[3], * token, card_number[53], id[53];
	float discount = 0;

	while (1) {
		/*
		 * Recebe do cliente o número do cartão do usuário.
		 *
		 * @param clientSocket				soquete do cliente para receber o número do cartão do usuário.
		 * @param card_number				string que representa o número do cartão do cliente.
		 * @param strlen(card_number)		tamanho da string.
		 * @param 0							flag que modifica o comportamento da função "recv()".
		 */
		iResult = recv(clientSocket, card_number, sizeof(card_number), 0);

		card_number[iResult] = '\0';

		// Verifica o número do cartão recebido.
		if (strlen(card_number) == 12) {
			/*
			 * Envia ao cliente a resposta da verificação.
			 *
			 * @param clientSocket			soquete do cliente para enviar a resposta da verificação.
			 * @param succss				string que representa a resposta do servidor.
			 * @param 8						tamanho da string.
			 * @param 0						flag que modifica o comportamento da função "send()".
			 */
			iResult = send(clientSocket, "success", 8, 0);
			break;
		}
		else {
			/*
			 * Envia ao cliente a resposta da verificação.
			 *
			 * @param clientSocket			soquete do cliente para enviar a resposta da verificação.
			 * @param failed				string que representa a resposta do servidor.
			 * @param 7						tamanho da string.
			 * @param 0						flag que modifica o comportamento da função "send()".
			 */
			iResult = send(clientSocket, "failed", 7, 0);
			continue;
		}
		break;
	}

	while (1) {
		/*
		 * Recebe do cliente o número de verificação do cartão do cliente.
		 *
		 * @param clientSocket		soquete do cliente para receber o número de verificação do cartão do usuário.
		 * @param id				string que representa o número de verificação do cartão do usuário.
		 * @param sizeof(id)		tamanho da string.
		 * @param 0					flag que modifica o comportamento da função "recv()".
		 */
		iResult = recv(clientSocket, id, sizeof(id), 0);
		if (iResult == SOCKET_ERROR) {
			continue;
		}

		id[iResult] = '\0';

		// Verifica o ID recebido.
		if (strlen(id) == 3) {
			/*
			 * Envia ao cliente a resposta da verificação.
			 *
			 * @param clientSocket			soquete do cliente para enviar a resposta da verificação.
			 * @param succss				string que representa a resposta do servidor.
			 * @param 8						tamanho da string.
			 * @param 0						flag que modifica o comportamento da função "send()".
			 */
			iResult = send(clientSocket, "success", 8, 0);
			break;
		}
		else {
			/*
			 * Envia ao cliente a resposta da verificação.
			 *
			 * @param clientSocket			soquete do cliente para enviar a resposta da verificação.
			 * @param failed				string que representa a resposta do servidor.
			 * @param 7						tamanho da string.
			 * @param 0						flag que modifica o comportamento da função "send()".
			 */
			iResult = send(clientSocket, "failed", 7, 0);
			continue;
		}
		break;
	}

	/*
	 * Recebe do cliente as informações do ingresso.
	 *
	 * @param clientSocket				soquete do cliente receber as informações do ingresso.
	 * @param ticket_info				string que representa as informações do ingresso.
	 * @param sizeof(ticket_info)		tamanho da string.
	 * @param 0							flag que modifica o comportamento da função "recv()".
	 */
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

	char date[10];
	char path[100];

	struct tm* timeinfo;
	time_t rawtime;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(date, "%02d-%02d", timeinfo->tm_mday, timeinfo->tm_mon + 1);

	sprintf(ticket_info, ",%d,%.2f,%.2f,Cartao\0", ticket_count, amount, discount);
	sprintf(payment_info, "%d,%d,%.2f\n", entry, ticket_count, amount - (amount * (discount / 100)));
	sprintf(path, "server_resources\\stats\\daily\\payment_stats\\%s.txt", date);

	// Registra as informações do ingresso no sistema.
	appendToFile("server_resources\\ticket_info_temp.csv", ticket_info);

	appendToFile(path, payment_info);
	appendToFile("server_resources\\stats\\all\\payment_stats.txt", payment_info);

	return 0;
}

/*
 * Verifica as informações do pagamento por pix do usuário.
 *
 * @param clientSocket		soquete do cliente a ser verificado.
 * @param entry				inteiro que representa o tipo de entrada do usuário.
 * @param ticket_count		inteiro que representa a quantidade de ingressos comprados pelo usuário.
 *
 * @return 0				caso as informações do usuários forem válidas.
 */
int verifyPixPayment(SOCKET clientSocket) {
	int iResult, count = 0, ticket_count, entry;
	char payment_info[100], info[30], ticket_info[100], * tokens[3], * token;
	float discount = 0.0, amount = 0.0, total;

	while (1) {
		/*
		 * Recebe do cliente as informações do pagamento por pix.
		 *
		 * @param clientSocket		soquete do cliente para receber as informações do pagamento por pix do usuário.
		 * @param info				string que representa as informações do pagamento por pix do usuário.
		 * @param sizeof(info)		tamanho da string.
		 * @param 0					flag que modifica o comportamento da função "recv()".
		 */
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
			discount = 0;
			break;
		case 1:
			total = (5 * ticket_count) * (50 / 100);
			discount = 50;
			break;
		case 2:
			total = 0;
			discount = 100;
			break;
		}

		// Verifica se o valor do pagamento é o suficiente.
		if (amount < total) {
			/*
			 * Envia ao cliente a resposta da verificação.
			 *
			 * @param clientSocket			soquete do cliente para enviar a resposta da verificação.
			 * @param failed				string que representa a resposta do servidor.
			 * @param 7						tamanho da string.
			 * @param 0						flag que modifica o comportamento da função "send()".
			 */
			iResult = send(clientSocket, "failed", 7, 0);
			continue;
		}

		sprintf(ticket_info, ",%d,%.2f,%.2f,   Pix\0", ticket_count, amount, discount);

		break;
	}

	/*
	 * Envia ao cliente a resposta da verificação.
	 *
	 * @param clientSocket			soquete do cliente para enviar a resposta da verificação.
	 * @param succss				string que representa a resposta do servidor.
	 * @param 8						tamanho da string.
	 * @param 0						flag que modifica o comportamento da função "send()".
	 */
	iResult = send(clientSocket, "success", 8, 0);

	// Registra as informações do pagamento no sistema.

	char date[10];
	char path[100];

	struct tm* timeinfo;
	time_t rawtime;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(date, "%02d-%02d", timeinfo->tm_mday, timeinfo->tm_mon + 1);
	sprintf(path, "server_resources\\stats\\daily\\payment_stats\\%s.txt", date);
	sprintf(payment_info, "%d,%d,%.2f\n", entry, ticket_count, amount - (amount * (discount / 100)));

	// Registra as informações do ingresso no sistema.
	appendToFile("server_resources\\ticket_info_temp.csv", ticket_info);

	appendToFile(path, payment_info);
	appendToFile("server_resources\\stats\\all\\payment_stats.txt", payment_info);

	return 0;
}

/*
 * Verifica qual pagamento o usuário escolheu.
 *
 * @param clientSocket		soquete do cliente a ser verificado.
 *
 * @return 0				caso o pagamento seja verificado com êxito.
 */
int paymentHandler(SOCKET clientSocket) {
	int iResult;
	char payment[11];

	while (1) {
		/*
		 * Recebe do cliente o tipo de pagamento a ser verificado.
		 *
		 * @param clientSocket			soquete do cliente para receber o tipo de pagamento.
		 * @param payment				string que representa o tipo de pagamento.
		 * @param sizeof(payment)		tamanho da string.
		 * @param 0						flag que modifica o comportamento da função "recv()".
		 */
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
