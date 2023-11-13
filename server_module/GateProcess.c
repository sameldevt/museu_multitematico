#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileProcess.h"
#include "Server.h"

struct ThemeThreadStruct {
	int num;
	SOCKET clientSocket;
};

/*
 * Verifica se o ingresso do usuário é válido.
 *
 * @param recv_ticket_id	string que representa o ID do ingresso para verificação.
 * 
 * @return 0				caso a verificação seja feita com êxito.
 * @return 1				caso a verificação não seja feita com êxito. Esse caso representa que
 *							o ingresso está expirado.
 * @return 2				caso a verificação não seja feita com êxito. Esse caso representa que
 *							o ingresso não é válido.
 */
int verifyTicketId(char recv_ticket_id[8]) {
	char id[8];

	FILE* fp = fopen(TICKET_DB, "r");

	if (fp == NULL) {
		exit(101);
	}

	while (fgets(id, sizeof(id), fp) != NULL) {
		size_t length = strlen(id);
		if (length > 0 && id[length - 1] == '\n') {
			id[length - 1] = '\0';
		}

		if (strcmp(id, recv_ticket_id) == 0) {
			fclose(fp);		
			char ticket_path[77], buf[3];
			int count00 = 0, count01 = 0;

			sprintf(ticket_path, "..\\receipt_gen\\receipt_gen_resources\\ticket_db\\%s.txt", recv_ticket_id);

			FILE* fp2 = fopen(ticket_path, "a+");

			if (fp2 == NULL) {
				exit(101);
			}

			fprintf(fp2, "00\n");

			fseek(fp, 0, SEEK_SET);

			while (fgets(buf, sizeof(buf), fp) != NULL) {
				size_t length = strlen(buf);
				if (length > 0 && buf[length - 1] == '\n') {
					buf[length - 1] = '\0';
				}

				if (strcmp(buf,"00") == 0) {
					count00++ ;
					continue;
				}
				else if (strcmp(buf, "01") == 0) {
					count01++;
					continue;
				}
			}

			if (count00 > count01) {
				fclose(fp2);
				return 1;
			}
		
			fclose(fp2);
			return 0;
		}
	}
	fclose(fp);

	return 2;
}

/*
 * Gerencia a thread do terminal de catraca. Esse terminal é reponsável pela
 * verificação do ingresso do usuário.
 *
 * @param arg	ponteiro para void que representa os argumentos usados na criação da thread.
 */
void* gateHandler(void* arg) {
	struct ThemeThreadStruct* params = (struct ThemeThreadStruct*)arg;
	int iResult;
	char ticket_id[8], theme_num[10];

	sprintf(theme_num, "%d\n", params->num);

	// Loop que mantém o fluxo de verificação ativo.
	while (1) {
		/*
		 * Recebe do cliente o ID do ingresso a ser verificado.
		 *
		 * @param params->clientSocket		soquete do cliente para receber o nome do usuário.
		 * @param ticket_id					string que representa o nome do usuário.
		 * @param sizeof(ticket_id)			tamanho da string.
		 * @param 0							flag que modifica o comportamento da função "recv()".
		 */
		iResult = recv(params->clientSocket, ticket_id, sizeof(ticket_id), 0);

		if (verifyTicketId(ticket_id) == 0) {
			/*
			 * Envia ao cliente o resultado da verificação.
			 *
			 * @param params->clientSocket		soquete do cliente para enviar a resposta da verificação.
			 * @param success					string que representa o resultado da avaliação.
			 * @param 8							tamanho da string.
			 * @param 0							flag que modifica o comportamento da função "send()".
			 */
			send(params->clientSocket, "success", 8, 0);

			// Registra as informações do ingresso no sistema
			appendToFile("server_resources\\stats\\daily\\gate_stats.txt", theme_num);
			appendToFile("server_resources\\stats\\all\\gate_stats.txt", theme_num);
			continue;
		}
		else if (verifyTicketId(ticket_id) == 1) {
			/*
			 * Envia ao cliente o resultado da verificação.
			 *
			 * @param params->clientSocket		soquete do cliente para enviar a resposta da verificação.
			 * @param expired					string que representa o resultado da avaliação.
			 * @param 8							tamanho da string.
			 * @param 0							flag que modifica o comportamento da função "send()".
			 */
			send(params->clientSocket, "expired", 8, 0);

			// Registra as informações do ingresso no sistema
			appendToFile("server_resources\\stats\\daily\\gate_stats.txt", theme_num);
			appendToFile("server_resources\\stats\\all\\gate_stats.txt", theme_num);
			continue;
		}
		else if (verifyTicketId(ticket_id) == 2) {
			/*
			 * Envia ao cliente o resultado da verificação.
			 *
			 * @param params->clientSocket		soquete do cliente para enviar a resposta da verificação.
			 * @param invalid					string que representa o resultado da avaliação.
			 * @param 8							tamanho da string.
			 * @param 0							flag que modifica o comportamento da função "send()".
			 */
			send(params->clientSocket, "invalid", 8, 0);

			// Registra as informações do ingresso no sistema
			appendToFile("server_resources\\stats\\daily\\gate_stats.txt", theme_num);
			appendToFile("server_resources\\stats\\all\\gate_stats.txt", theme_num);
			continue;
		}
	}
}