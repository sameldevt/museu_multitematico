#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

#include "Files.h"

/* Arquivo com funções de auxílio ao sistema */
void setCursorView(int condition) {
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	cursorInfo.bVisible = condition;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void setCursorLocation(int cord_x, int cord_y) {
	COORD cords;
	cords.X = cord_x;
	cords.Y = cord_y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cords);
}

char* enterInput(int input_size, int cloc_x, int cloc_y) {
	int key, count = 0;
	char* input = (char*)malloc(input_size - 1);
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	setCursorView(1);
	setCursorLocation(cloc_x, cloc_y);

	while (1) {
		key = _getch();
		if (key == 13) {
			input[count] = '\0';
			break;
		}
		else if (key == 8) {
			if (count > 0) {
				count--;
				input[count] = '\0';
				printf("\b \b"); 
			}
		}
		else if (count < input_size - 1) {
			input[count] = key;
			count++;
			input[count] = '\0';
			printf("%c", key);
		}

	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	return input;
}

void setHorizontalSize() {
	// setlocale(LC_ALL, "Portuguese");
	// resolve o problema de acentuação porem buga todo o programa

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	if (console != INVALID_HANDLE_VALUE) {
		// Define o tamanho do buffer de tela
		COORD bufferSize;
		bufferSize.X = 170; // 100 colunas
		bufferSize.Y = 40;  // 50 linhas

		SetConsoleScreenBufferSize(console, bufferSize);

		// Define o tamanho da janela do console
		SMALL_RECT windowSize;
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = 169;  // 100 colunas - 1
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

void setVerticalSize() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	if (console != INVALID_HANDLE_VALUE) {
		// Define o tamanho do buffer de tela
		COORD bufferSize;
		bufferSize.X = 70; // 100 colunas
		bufferSize.Y = 40;  // 50 linhas

		SetConsoleScreenBufferSize(console, bufferSize);

		// Define o tamanho da janela do console
		SMALL_RECT windowSize;
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = 69;  // 100 colunas - 1
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

int keyboardCheck() {
	int key;

	while (_kbhit() != 'x') {
		key = _getch();
		return key;
	}
}

void startProgram() {
	setHorizontalSize();
	loadScreen(INITIAL_SCREEN);
	Sleep(3000);
}

void endProgram() {
	exit(0);
}
