#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PaymentProcess.h"
#include "FileProcess.h"
#include "EntryProcess.h"
#include "Server.h"

/* Arquivo com funções de verificação de entrada de usuários */

int verifyUserEntry(SOCKET clientSocket) {
	char name[100], * token, *tokens[2];
	int iResult, count = 0;

	while (1) {
		iResult = recv(clientSocket, name, sizeof(name), 0);
		if (iResult == SOCKET_ERROR) {
			sendFailMessage(clientSocket);
			continue;
		}

		name[iResult] = '\0';

		registerUser(name);

		iResult = send(clientSocket, "success", 8, 0);
		if (iResult == SOCKET_ERROR) {
			sendFailMessage(clientSocket);
			continue;
		}
		break;
	}

	writeFile("server_resources\\ticket_info_temp.csv", "  ENTRADA CHEIA \0");
	return 0;
}

int verifyStudentEntry(SOCKET clientSocket) {
	int iResult, count = 0;
	char info[120], * tokens[2], * token, name[101], student_id[9];

	while (1) {
		iResult = recv(clientSocket, info, sizeof(info), 0);
		if (iResult == SOCKET_ERROR) {
			sendFailMessage(clientSocket);
		}

		token = strtok(info, ",");
		
		while (token != NULL && count < 2) {
			tokens[count] = token;
			count++;
			token = strtok(NULL, ",");
		}

		strcpy(name, tokens[0]);
		strcpy(student_id, tokens[1]);

		if (strlen(student_id) == 8) {
			iResult = send(clientSocket, "success", 8, 0);
			if (iResult == SOCKET_ERROR) {
				sendFailMessage(clientSocket);
			}
			break;
		}
		else {
			iResult = send(clientSocket, "failed", 7, 0);
			if (iResult == SOCKET_ERROR) {
				sendFailMessage(clientSocket);
			}
			continue;
		}
	}

	registerUser(name);

	iResult = send(clientSocket, "success", 8, 0);
	if (iResult == SOCKET_ERROR) {
		sendFailMessage(clientSocket);
	}

	writeFile("server_resources\\ticket_info_temp.csv", "    MEIA ENTRADA  \0");

	return 0;
}

int verifySeniorEntry(SOCKET clientSocket) {
	int iResult, count = 0;
	char info[120], * token, * tokens[2], name[100];

	while (1) {
		iResult = recv(clientSocket, info, sizeof(info), 0);
		if (iResult == SOCKET_ERROR) {
			sendFailMessage(clientSocket);
		}

		token = strtok(info, ",");

		while (token != NULL && count < 2) {
			tokens[count] = token;
			count++;
			token = strtok(NULL, ",");
		}

		strcpy(name, tokens[0]);
		int age = atoi(tokens[1]);

		if (age < 60) {
			iResult = send(clientSocket, "failed", 7, 0);
			if (iResult == SOCKET_ERROR) {
				sendFailMessage(clientSocket);
			}
			continue;
		}
		break;
	}

	registerUser(name);
	
	iResult = send(clientSocket, "success", 8, 0);
	if (iResult == SOCKET_ERROR) {
		sendFailMessage(clientSocket);
	}

	char entry_info[100];

	sprintf(entry_info, "  ENTRADA GRATUITA,%d,%.2f,%.2f,      \0", 0, 0.0, 100.0);

	writeFile("server_resources\\ticket_info_temp.csv", entry_info);

	return 0;
}

int verifyJuniorEntry(SOCKET clientSocket) {
	int iResult, count = 0;
	char info[120], * token, * tokens[2], name[100];

	while (1) {
		iResult = recv(clientSocket, info, sizeof(info), 0);
		if (iResult == SOCKET_ERROR) {
			sendFailMessage(clientSocket);
		}

		token = strtok(info, ",");

		while (token != NULL && count < 2) {
			tokens[count] = token;
			count++;
			token = strtok(NULL, ",");
		}

		strcpy(name, tokens[0]);
		int age = atoi(tokens[1]);

		if (age > 5) {
			iResult = send(clientSocket, "failed", 7, 0);
			if (iResult == SOCKET_ERROR) {
				sendFailMessage(clientSocket);
			}
			continue;;
		}
		break;
	}

	registerUser(name);
	
	iResult = send(clientSocket, "success", 8, 0);
	if (iResult == SOCKET_ERROR) {
		sendFailMessage(clientSocket);
	}

	char entry_info[100];

	sprintf(entry_info, "  ENTRADA GRATUITA,%d,%.2f,%.2f,      \0", 0, 0.0, 100.0);

	writeFile("server_resources\\ticket_info_temp.csv", entry_info);
	
	return 0;
}

int verifyDisabledPersonEntry(SOCKET clientSocket) {
	int iResult, count = 0;
	char* tokens[2], * token, info[120], name[100], disabled_person_id[50];

	while (1) {
		iResult = recv(clientSocket, info, sizeof(info), 0);
		if (iResult == SOCKET_ERROR) {
			sendFailMessage(clientSocket);
		}

		token = strtok(info, ",");

		while (token != NULL && count < 2) {
			tokens[count] = token;
			count++;
			token = strtok(NULL, ",");
		}

		strcpy(name, tokens[0]);
		strcpy(disabled_person_id, tokens[1]);

		if (strlen(disabled_person_id) == 11) {
			iResult = send(clientSocket, "success", 8, 0);
			if (iResult == SOCKET_ERROR) {
				sendFailMessage(clientSocket);
			}
			break;
		}
		else {
			iResult = send(clientSocket, "failed", 7, 0);
			if (iResult == SOCKET_ERROR) {
				sendFailMessage(clientSocket);
			}
			continue;
		}
	}

	registerUser(name);
	
	iResult = send(clientSocket, "success", 8, 0);
	if (iResult == SOCKET_ERROR) {
		sendFailMessage(clientSocket);
	}

	writeFile("server_resources\\ticket_info_temp.csv", "  MEIA ENTRADA  \0");

	return 0;
}

int entryHandler(SOCKET clientSocket) {
	int iResult;
	char entry[7], buffer[10];

	iResult = recv(clientSocket, entry, sizeof(entry), 0);
	if (iResult == SOCKET_ERROR) {
		sendFailMessage(clientSocket);
	}

	if (strcmp(entry, "entry1") == 0) {
		if (verifyUserEntry(clientSocket) == 0) {
			return 0;
		}
	}
	else if (strcmp(entry, "entry2") == 0) {
		if (verifyStudentEntry(clientSocket) == 0){
			return 0;
		}
	}
	else if (strcmp(entry, "entry3") == 0) {
		if (verifyDisabledPersonEntry(clientSocket) == 0) {
			return 0;
		}
	}
	else if (strcmp(entry, "entry4") == 0) {
		if (verifyJuniorEntry(clientSocket) == 0) {
			return 0;
		}
	}
	else if (strcmp(entry, "entry5") == 0) {
		if (verifySeniorEntry(clientSocket) == 0) {
			return 0;
		}
	}

	return 1;
}