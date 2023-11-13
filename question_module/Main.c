#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define ARE_YOU_SURE "question_resources\\screen_template.ans"
#define INITIAL_SCREEN "question_resources\\initialscreen.ans"
#define TEMP_DESC "question_resources\\description_template.ans"
#define THANK_YOU "question_resources\\thank_you.ans"

/*
 * Carrega uma tela de acordo com o seu caminho.
 *
 * @param screen_path	representa o caminho da tela a ser carregada.
*/
void loadScreen(char screen_path[100]) {
	system("cls");
	FILE* fp = fopen(screen_path, "r");

	char buffer[256];

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		printf("%s", buffer);
	}

	fclose(fp);
}

/*
 * Define a visibilidade do cursor no console.
 *
 * @param condition		condição para definição da visibilidade.
 */
void switchCursorView(HANDLE console, int condition) {
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(console, &cursorInfo);

	// Define a visibilidade. Caso seja 1, o cursor está ativado. Caso seja 0, o cursor está desativado.
	cursorInfo.bVisible = condition;

	SetConsoleCursorInfo(console, &cursorInfo);
}

// Ajusta o tamanho e centraliza a janela de console.
void setWindowSize(HANDLE console) {
	if (console != INVALID_HANDLE_VALUE) {

		// Define o tamanho do buffer de tela
		COORD bufferSize;
		bufferSize.X = 200;
		bufferSize.Y = 100;

		SetConsoleScreenBufferSize(console, bufferSize);

		// Define o tamanho da janela do console
		SMALL_RECT windowSize;
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = 110;
		windowSize.Bottom = 39;

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
	char art_feedback[100], recv_id[10], desc_path[100];
	SOCKET clientSocket = socketSetup();

	// Tenta se conectar ao servidor.
	while (1) {
		result = connectSocket(clientSocket);
		if (result != 0) {
			Sleep(3000);
			continue;
		}
		break;
	}
	
	/*
	 * Recebe do servidor o ID da arte para ser carregada.
	 *
	 * @param clientSocket			soquete do cliente para receber o ID da arte.
	 * @param recv_id				string que representa o ID da arte.
	 * @param sizeof(recv_id)		tamanho da string.
	 * @param 0						flag que modifica o comportamento da função "recv()".
	 */
	result = recv(clientSocket, recv_id, sizeof(recv_id), 0);

	int art_id = atoi(recv_id);
	
	/*
	 * Define o caminho da descrição da arte a ser carregada de acordo com o ID recebido pelo
	 * servidor e atribui o mesmo à variável "desc_path".
	 */
	for (int i = 0; i < 16; i++) {
		if (art_id == i) {
			sprintf(desc_path, "..\\question_module\\question_resources\\desc%d.ans", i);
			break;
		}
	}
	
	// Executa o programa "art_module" para exibir a arte correspondente ao ID recebido anteriormente.
	system("start ..\\art_module\\art_module.exe");

	// Criar um soquete temporário para comunicação entre módulos de questionário e arte.
	SOCKET tempSocket = serverSetup();
	while (1) {
		if (listen(tempSocket, SOMAXCONN) == SOCKET_ERROR) {
			closesocket(tempSocket);
			WSACleanup();
			continue;
		}

		// Aceita uma conexão do cliente e cria um soquete para o mesmo.
		SOCKET artSocket = accept(tempSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			closesocket(tempSocket);
			WSACleanup();
			continue;
		}

		/*
		 * Envia ao módulo de arte o ID da arte para ser carregada.
		 *
		 * @param artSocket				soquete do módulo de arte para enviar o ID da arte.
		 * @param recv_id				string que representa o ID da arte.
		 * @param sizeof(recv_id)		tamanho da string.
		 * @param 0						flag que modifica o comportamento da função "send()".
		 */
		send(artSocket, recv_id, sizeof(recv_id), 0);
		closesocket(tempSocket);
		closesocket(artSocket);
		WSACleanup();
		break;
	}

	system("vsdevcmd.bat");
	system("cls");

	// Ajusta o tamanho e centraliza a janela de console.
	setWindowSize(GetStdHandle(STD_OUTPUT_HANDLE));

	// Desativa o cursor do console.
	switchCursorView(GetStdHandle(STD_OUTPUT_HANDLE), 0);

	// Loop que mantém o fluxo das janelas do console.
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
					char temp_path[100];
					sprintf(temp_path, "..\\question_module\\question_resources\\question%d.ans", i);
					loadScreen(temp_path);
					
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
			sprintf(art_feedback, "%d,%d", art_id, rate);
			/*
			 * Envia ao servidor a avaliação do usuário referente a arte carregada.
			 *
			 * @param clientSocket				soquete do servidor para enviar a avaliação da arte.
			 * @param art_feedback				string que representa a avaliação da arte.
			 * @param sizeof(art_feedback)		tamanho da string.
			 * @param 0							flag que modifica o comportamento da função "send()".
			 */
			send(clientSocket, art_feedback, sizeof(art_feedback), 0);
			loadScreen(THANK_YOU);
			Sleep(2000);
			pos = 0;
			break;
		}
	}
}