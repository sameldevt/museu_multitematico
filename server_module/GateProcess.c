#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileProcess.h"
#include "Server.h"

struct ThemeThreadStruct {
	int num;
	SOCKET clientSocket;
};

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

void* gateHandler(void* arg) {
	struct ThemeThreadStruct* params = (struct ThemeThreadStruct*)arg;
	int iResult;
	char ticket_id[8], theme_num[10];

	sprintf(theme_num, "%d", params->num);

	while (1) {
		iResult = recv(params->clientSocket, ticket_id, sizeof(ticket_id), 0);

		if (verifyTicketId(ticket_id) == 0) {
			send(params->clientSocket, "success", 8, 0);
			appendToFile("server_resources\\stats\\daily\\gate_stats.txt", theme_num);
			appendToFile("server_resources\\stats\\all\\gate_stats.txt", theme_num);
			continue;
		}
		else if (verifyTicketId(ticket_id) == 1) {
			send(params->clientSocket, "expired", 8, 0);
			appendToFile("server_resources\\stats\\daily\\gate_stats.txt", theme_num);
			appendToFile("server_resources\\stats\\all\\gate_stats.txt", theme_num);
			continue;
		}
		else if (verifyTicketId(ticket_id) == 2) {
			send(params->clientSocket, "invalid", 8, 0);
			appendToFile("server_resources\\stats\\daily\\gate_stats.txt", theme_num);
			appendToFile("server_resources\\stats\\all\\gate_stats.txt", theme_num);
			continue;
		}
	}
}