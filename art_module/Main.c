#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define INITIAL_SCREEN "art_resources\\initialscreen.ans"

void loadScreen(char screen_path[100]) {
	char buffer[256];
	system("cls");
	
	FILE* fp = fopen(screen_path, "r");
	if (fp == NULL) {
		exit(102);
	}
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

int main() {
	system("vsdevcmd.bat");
	system("cls");


	int result;
	char request[10], art[256], desc[256], bnum[10];
	SOCKET clientSocket = socketSetup();
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	switchCursorView(console, 0);
	loadScreen(INITIAL_SCREEN);

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

	char art_path[100];
	for (int i = 1; i < 16; i++) {
		if (inum == i) {
			sprintf(art_path, "art_resources\\art%d.ans", i);
			break;
		}
	}

	loadScreen(art_path);

	_getch();
	return 0;
}