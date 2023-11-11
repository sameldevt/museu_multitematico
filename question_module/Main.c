#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define ARE_YOU_SURE "question_resources\\screen_template.ans"
#define INITIAL_SCREEN "question_resources\\initialscreen.ans"
#define TEMP_DESC "question_resources\\description_template.ans"
#define THANK_YOU "question_resources\\thank_you.ans"

void loadScreen(char screen_path[100]) {
	system("cls");
	FILE* fp = fopen(screen_path, "r");

	char buffer[256];

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		printf("%s", buffer);
	}

	fclose(fp);
}

void switchCursorView(HANDLE console, int condition) {
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(console, &cursorInfo);

	cursorInfo.bVisible = condition;

	SetConsoleCursorInfo(console, &cursorInfo);
}

void setWindowSize(HANDLE console) {
	if (console != INVALID_HANDLE_VALUE) {
		// Define o tamanho do buffer de tela
		COORD bufferSize;
		bufferSize.X = 200; // 100 colunas
		bufferSize.Y = 100;  // 50 linhas

		SetConsoleScreenBufferSize(console, bufferSize);

		// Define o tamanho da janela do console
		SMALL_RECT windowSize;
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = 110;  // 100 colunas - 1
		windowSize.Bottom = 39; // 50 linhas - 1

		SetConsoleWindowInfo(console, TRUE, &windowSize);
	}

	Sleep(100);

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
		int consoleX = (screenWidth - consoleWidth) / 1.04;
		int consoleY = (screenHeight - consoleHeight) / 2;

		// Move o console para as coordenadas calculadas
		MoveWindow(consoleWindow, consoleX, consoleY, consoleWidth, consoleHeight, TRUE);
	}

	Sleep(100);
}

int main() {
	int result, key, pos = 0, rate = 0;
	char art_feedback[100], bnum[10], desc_path[100];
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
	
	result = recv(clientSocket, bnum, sizeof(bnum), 0);

	int inum = atoi(bnum);
	
	for (int i = 0; i < 15; i++) {
		if (inum == i) {	
			sprintf(desc_path, "..\\question_module\\question_resources\\desc%d.ans", i);
			break;
		}
	}
	
	system("start ..\\art_module\\art_module.exe");

	SOCKET tempSocket = serverSetup();
	while (1) {
		if (listen(tempSocket, SOMAXCONN) == SOCKET_ERROR) {
			perror("Listen failed with error: %ld\n", WSAGetLastError());
			closesocket(tempSocket);
			WSACleanup();
			continue;
		}

		SOCKET artSocket = accept(tempSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			perror("accept failed: %d\n", WSAGetLastError());
			closesocket(tempSocket);
			WSACleanup();
			continue;
		}

		send(artSocket, bnum, sizeof(bnum), 0);
		closesocket(tempSocket);
		closesocket(artSocket);
		WSACleanup();
		break;
	}

	system("vsdevcmd.bat");
	system("cls");
	setWindowSize(GetStdHandle(STD_OUTPUT_HANDLE));
	switchCursorView(GetStdHandle(STD_OUTPUT_HANDLE), 0);

	while (1) {
		switch (pos) {
		case 0:
			if (_kbhit() == NULL) {
				loadScreen(desc_path);
				if (key = _getch()) {
					pos = 1;
					break;
				}
			}
		case 1:	
			for (int i = 1; i < 6; i++) {
				if (i == 5) {
					pos = 2;
					break;
				}
				if (_kbhit() == NULL) {
					char temppath[100];
					sprintf(temppath, "..\\question_module\\question_resources\\question%d.ans", i);
					loadScreen(temppath);
					
					key = _getch();
					switch (key) {
					case 49:
						rate += 50;
						continue;
					case 50:
						rate += 40;
						continue;
					case 51:
						rate += 30;
						continue;
					case 52:
						rate += 20;
						continue;
					case 53:
						rate += 10;
						continue;
					default:
						i--;
						continue;
					}
				}
			}
		case 2:
			sprintf(art_feedback, "%d,%d", inum, rate);
			send(clientSocket, art_feedback, sizeof(art_feedback), 0);
			loadScreen(THANK_YOU);
			Sleep(2000);
			pos = 0;
			break;
		}
	}
}