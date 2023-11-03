#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "WinsockSetup.h"

#define INITIAL_SCREEN "C:\\ws-vs\\museu_multidisciplinar\\art_module\\resources\\initialscreen.ans"

void recvDesc(SOCKET clientSocket) {
	int iResult;
	char desc[300], desc_path[100];

	iResult = recv(clientSocket, desc_path, sizeof(desc_path), 0);

	FILE* fp = fopen(desc_path, "r");

	if (fp == NULL) {
		perror("error");
		exit(101);
	}

	while (fgets(desc, sizeof(desc), fp) != NULL) {
		printf("%s", desc);
	}

	fclose(fp);
}

void loadScreen(char screen_path[100]) {
	system("cls");
	FILE* fp = fopen(screen_path, "r");

	char buffer[256];

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		printf("%s", buffer);
	}

	fclose(fp);
}

int main() {
	int result;
	char request[10], art[256], desc[256];
	SOCKET clientSocket = socketSetup();

	while (1) {
		result = connectSocket(clientSocket);
		if (result != 0) {
			perror("Error connecting socket.\n");
			perror("Trying again...\n");
			Sleep(3000);
			continue;
		}
		break;
	}

	loadScreen(INITIAL_SCREEN);

	send(clientSocket, request, sizeof(request), 0);

	recvDesc(clientSocket);

}