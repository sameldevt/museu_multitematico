#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

#include "Files.h"

/*
 * Define a visibilidade do cursor no console.
 *
 * @param condition		condição para definição da visibilidade.
 */
void setCursorView(int condition) {
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	// Define a visibilidade. Caso seja 1, o cursor está ativado. Caso seja 0, o cursor está desativado.
	cursorInfo.bVisible = condition;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

/*
 * Define a posição do cursor do console.
 *
 * @param cord_x	coordenada "x" onde será posicionado o cursor.
 * @param cord_y	coordenada "y" onde será posicionado o cursor.
 */
void setCursorLocation(int cord_x, int cord_y) {
	COORD cords;
	cords.X = cord_x;	// coordenada "x".
	cords.Y = cord_y;	// coordenada "y".

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cords);
}

/*
 * Solicita e recebe uma entrada de usuário.
 *
 * @param input_size	tamanho máximo da string.
 * @param cloc_x		coordenada "x" onde será posicionado o cursor.
 * @param cloc_y		coordenada "y" onde será posicionado o cursor.
 */
char* enterInput(int input_size, int cloc_x, int cloc_y) {
	int key, count = 0;
	char* input = (char*)malloc(input_size - 1);
	
	/*
	 * Define a cor de fundo e do texto do console.
	 *
	 * @param GetStdHandle(STD_OUTPUT_HANDLE)		saída padrão (console).
	 * @param 240									valor que representa as cores do console.
	 */
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

	// Ativa o cursor do console.
	setCursorView(1);

	/*
	 * Define a posição do cursor do console.
	 *
	 * @param cloc_x	coordenada "x" onde será posicionado o cursor.
	 * @param cloc_y	coordenada "y" onde será posicionado o cursor.
	 */
	setCursorLocation(cloc_x, cloc_y);

	while (1) {
		key = _getch();
		if (key == 13) {			// Tecla "Enter"
			input[count] = '\0';
			break;
		}
		else if (key == 8) {		// Tecla "Backspace".
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

	/*
	 * Define a cor de fundo e do texto do console.
	 *
	 * @param GetStdHandle(STD_OUTPUT_HANDLE)		saída padrão (console).
	 * @param 15									valor que representa as cores do console.
	 */
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	return input;
}

// Ajusta o tamanho e centraliza a janela de console horizontalmente.
void setHorizontalSize() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	if (console != INVALID_HANDLE_VALUE) {

		// Define o tamanho do buffer de tela
		COORD bufferSize;
		bufferSize.X = 170;		
		bufferSize.Y = 40;		

		SetConsoleScreenBufferSize(console, bufferSize);

		// Define o tamanho da janela do console
		SMALL_RECT windowSize;
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = 169;  
		windowSize.Bottom = 39; 

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

// Ajusta o tamanho e centraliza a janela de console verticalmente.
void setVerticalSize() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	if (console != INVALID_HANDLE_VALUE) {
		// Define o tamanho do buffer de tela
		COORD bufferSize;
		bufferSize.X = 70; 
		bufferSize.Y = 40;  

		SetConsoleScreenBufferSize(console, bufferSize);

		// Define o tamanho da janela do console
		SMALL_RECT windowSize;
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = 69;  
		windowSize.Bottom = 39; 

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
