#include <stdio.h>
#include <stdlib.h>

#include "WinsockSetup.h"

#define PROXIMITY "ticket_resources\\proximity.ans"
#define SUCCESS "ticket_resources\\success.ans"
#define EXPIRED "ticket_resources\\expired.ans"
#define INVALID "ticket_resources\\invalid.ans"
#define LOADING "ticket_resources\\loading.ans"

void loadScreen(char screen_path[100]) {
	system("cls");
	FILE* fp = fopen(screen_path, "r");

	char buffer[256];

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		printf("%s", buffer);
	}

	fclose(fp);
}

void setScreenSize() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	if (console != INVALID_HANDLE_VALUE) {
		// Define o tamanho do buffer de tela
		COORD bufferSize;
		bufferSize.X = 75; // 100 colunas
		bufferSize.Y = 40;  // 50 linhas

		SetConsoleScreenBufferSize(console, bufferSize);

		// Define o tamanho da janela do console
		SMALL_RECT windowSize;
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = 74;  // 100 colunas - 1
		windowSize.Bottom = 39; // 50 linhas - 1

		SetConsoleWindowInfo(console, TRUE, &windowSize);
	}

	// Obtém um identificador para o console atual
	HWND consoleWindow = GetConsoleWindow();

	if (consoleWindow != NULL) {
		// Obtém informações sobre a tela
		RECT consoleRect;
		GetWindowRect(consoleWindow, &consoleRect);

		// Obtém as dimensões da tela
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Calcula as coordenadas para centralizar o console
		int consoleWidth = consoleRect.right - consoleRect.left;
		int consoleHeight = consoleRect.bottom - consoleRect.top;
		int consoleX = (screenWidth - consoleWidth) / 2;
		int consoleY = (screenHeight - consoleHeight) / 2;

		// Move o console para as coordenadas calculadas
		MoveWindow(consoleWindow, consoleX, consoleY, consoleWidth, consoleHeight, TRUE);
	}
}

int main() {
	char ticket_id[8], response[8];
	int result;
	SOCKET clientSocket = socketSetup();

	setScreenSize();

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

	result = send(clientSocket, "client3\0", 8, 0);

	while (1) {
		system("cls");

		loadScreen(PROXIMITY);

		printf("Digite o ID do seu ingresso: ");
		scanf("%s", ticket_id);
	
		system("cls");

		result = send(clientSocket, ticket_id, sizeof(ticket_id), 0);
;		loadScreen(LOADING);
		Sleep(1000);
		result = recv(clientSocket, response, sizeof(response), 0);

		if(strcmp(response, "success") == 0){
			loadScreen(SUCCESS);
			Sleep(3000);
			continue;
		}
		else if (strcmp(response, "expired") == 0) {
			loadScreen(EXPIRED);
			Sleep(3000);
			continue;
		}
		else if (strcmp(response, "invalid") == 0) {
			loadScreen(INVALID);
			Sleep(3000);
			continue;
		}
	}
}