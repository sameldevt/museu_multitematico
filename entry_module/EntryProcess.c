#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "EntryProcess.h"
#include "Server.h"

#define DEFAULT_RECVBUF_LEN 100

/* Arquivo com funções de verificação de entrada de usuários */

char recvName(SOCKET clientSocket) {
	int iResult;
	char name[100];

	do {
		iResult = recv(clientSocket, name, sizeof(name), 0);
	} while (iResult > 0);

	return name;
}

int verifyUserEntry(SOCKET clientSocket) {
	int iResult;
	char name[100];

	strcpy(name, recvName(clientSocket));

	registerUser(name);
	sendOk(clientSocket);
	printf("User registered.\n");
	return 0;
}

int verifyStudentEntry(SOCKET clientSocket) {
	int iResult;
	char name[100], student_id[9];
	
	strcpy(name, recvName(clientSocket));

	do {
		iResult = recv(clientSocket, student_id, sizeof(student_id), 0);
	} while (iResult > 0);

	if (sizeof(student_id) > 8 || sizeof(student_id) < 8) {
		sendError(clientSocket);
		printf("Invalid student_id.\n");
		return 1;
	}

	registerUser(name);
	sendOk(clientSocket);
	printf("Student registered.\n");
	return 0;
}

int verifySeniorEntry(SOCKET clientSocket) {
	int iResult, age;
	char name[100];

	strcpy(name, recvName(clientSocket));

	do {
		iResult = recv(clientSocket, age, sizeof(age / 4), 0);
	} while (iResult > 0);

	if (age < 60) {
		sendError(clientSocket);
		printf("Invalid senior age.\n");
		return 1;
	}

	registerUser(name);
	sendOk(clientSocket);
	printf("Senior registered.\n");
	return 0;
}

int verifyJuniorEntry(SOCKET clientSocket) {
	int iResult, age;
	char name[100];

	strcpy(name, recvName(clientSocket));

	do {
		iResult = recv(clientSocket, age, sizeof(age / 4), 0);
	} while (iResult > 0);

	if (age > 5) {
		sendError(clientSocket);
		printf("INvalid junior age.\n");
		return 1;
	}
	
	registerUser(name);
	sendOk(clientSocket);
	printf("Junior registered.\n");
	return 0;
}

int verifyDisabledPersonEntry(SOCKET clientSocket) {
	int iResult;
	char name[100], id[12];

	strcpy(name, recvName(clientSocket));

	do {
		iResult = recv(clientSocket, id, sizeof(id), 0);
	} while (iResult > 0);
	
	if (sizeof(id) > 11 || sizeof(id) < 11) {
		sendError(clientSocket);
		printf("Invalid disabled person id.\n");
		return 1;
	}
	
	registerUser(name);
	sendOk(clientSocket);
	printf("Disabled person registered.\n");
	return 0;
}

int entryHandler(SOCKET clientSocket) {
	int iResult, request;

	iResult = recv(clientSocket, request, sizeof(request / 4), 0);
	if (iResult > 0) {
		switch (request) {
		case 1:
			if (verifyUserEntry(clientSocket) == 0) {
				break;
			}
		case 2:
			if (verifyStudentEntry(clientSocket) == 0) {
				break;
			}
		case 3:
			if (verifyDisabledPersonEntry(clientSocket) == 0) {
				break;
			}
		case 4:
			if (verifyJuniorEntry(clientSocket) == 0) {
				break;
			}
		case 5:
			if (verifySeniorEntry(clientSocket) == 0) {
				break;
			}
		}
	}

	return 0;
}