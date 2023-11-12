#include <stdio.h>
#include <stdlib.h>

#include "WinsockSetup.h"

#define PROXIMITY "ticket_resources\\proximity.ans"
#define SUCCESS "ticket_resources\\success.ans"
#define EXPIRED "ticket_resources\\expired.ans"
#define INVALID "ticket_resources\\invalid.ans"
#define LOADING "ticket_resources\\loading.ans"

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

// Ajusta o tamanho e centraliza a janela de console.
void setScreenSize() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	if (console != INVALID_HANDLE_VALUE) {

		// Define o tamanho do buffer de tela
		COORD bufferSize;
		bufferSize.X = 75; 
		bufferSize.Y = 40; 

		SetConsoleScreenBufferSize(console, bufferSize);

		// Define o tamanho da janela do console
		SMALL_RECT windowSize;
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = 74;  
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

int main() {
	char ticket_id[8], response[8];
	int result;
	SOCKET clientSocket = socketSetup();

	// Ajusta o tamanho e centraliza a janela de console.
	setScreenSize();

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
	 * Envia ao servidor o tipo de cliente conectado.
	 *
	 * @param clientSocket		soquete do cliente para se conectar ao servidor.
	 * @param client3			string que representa o tipo de cliente.
	 * @param 8					tamanho da string.
	 * @param 0					flag que modifica o comportamento da função "send()".
	 */
	result = send(clientSocket, "client3\0", 8, 0);

	while (1) {
		system("cls");

		loadScreen(PROXIMITY);

		printf("Digite o ID do seu ingresso: ");
		scanf("%s", ticket_id);
	
		system("cls");

		/*
		 * Envia ao servidor o ID do ingresso para verificação.
		 *
		 * @param clientSocket			soquete do cliente para se conectar ao servidor.
		 * @param ticket_id				string que representa o tipo de cliente.
		 * @param sizeof(ticket_id)		tamanho da string.
		 * @param 0						flag que modifica o comportamento da função "send()".
		 */
		result = send(clientSocket, ticket_id, sizeof(ticket_id), 0);
;		loadScreen(LOADING);
		Sleep(1000);

		/*
		 * Recebe do servidor a resposta da verificação.
		 *
		 * @param clientSocket			soquete do cliente para receber a resposta da verificação.
		 * @param response				string que representa a resposta do servidor.
		 * @param sizeof(response)		tamanho da string.
		 * @param 0						flag que modifica o comportamento da função "recv()".
		 */
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