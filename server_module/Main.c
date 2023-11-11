#include "Server.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "EntryTerminalHandler.h"
#include "ArtProcess.h"
#include "GateProcess.h"
#include "ReportGen.h"
#include "EventLog.h"

#pragma comment(lib, "pthreadVC2.lib")

/* 
   A função abaixo tem como parâmetro um identificador para o console do Windows
   e é responsável por ajustar o tamanho da mesma bem como reposicioná-la no lado 
   esquerdo da tela.
*/
void setWindowSize(HANDLE console) {
	if (console != INVALID_HANDLE_VALUE) {
		// Define o tamanho do buffer de tela
		COORD bufferSize;
		bufferSize.X = 200;
		bufferSize.Y = 50;

		SetConsoleScreenBufferSize(console, bufferSize);

		// Define o tamanho da janela do console
		SMALL_RECT windowSize;
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = 99; 
		windowSize.Bottom = 49; 

		SetConsoleWindowInfo(console, TRUE, &windowSize);
	}

	Sleep(100);

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
		int consoleX = (screenWidth - consoleWidth) / 20;
		int consoleY = (screenHeight - consoleHeight) / 2;

		// Move o console para as coordenadas calculadas
		MoveWindow(consoleWindow, consoleX, consoleY, consoleWidth, consoleHeight, TRUE);
	}

	Sleep(100);
}

/*
	A função abaixo não possui valores como parâmetro e é responsável por gerenciar
	threads criadas pelo programa.
*/
void* threadHandler() {
	int result, thread_count = 0, art_count = 0, theme_count = 0;
	char buffer[100], message[50], client[8], init_time[54];
	pthread_t *client_threads[100];
	time_t rawtime;
	
	struct tm* timeinfo;
	struct ArtThreadStruct {
		int num;
		SOCKET clientSocket;
	};

	struct ThemeThreadStruct {
		int num;
		SOCKET clientSocket;
	};

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(init_time, "\nSERVER INITIALIZED AT [%04d-%02d-%02d %02d:%02d:%02d]\n",
		timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
		timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	FILE* fp = fopen("server_resources\\server_log.txt", "a");
	fprintf(fp, init_time);
	fclose(fp);

	// Configura e criar um Socket para o servidor.
	SOCKET serverSocket = serverSetup();

	// Loop para aceitar conexões de clientes no servidor.
	while (1) {

		// Aguarda a conexão de um cliente.
		listen(serverSocket, SOMAXCONN);

		// Aceita uma conexão do cliente e cria um Socket para o mesmo.
		SOCKET clientSocket = accept(serverSocket, NULL, NULL);

		// Recebe uma mensagem que representa o tipo de cliente conectado.
		result = recv(clientSocket, client, 8, 0);
		logServer("Client connected - ");

		// Caso a mensagem recebida seja "client1", a função "pthread_create()" cria uma thread
		// que é reponsável por gerenciar as funções do "terminal de entrada".
		if (strcmp(client, "client1") == 0) {
			logServer(" ENTRY MODULE CONNECTED.\n");
			if (pthread_create(&client_threads[thread_count], NULL, terminalHandler, clientSocket) != 0) {
				logServer("Erro ao criar a thread do cliente");
				closesocket(clientSocket);
			}
		}

		// Caso a mensagem recebida seja "client2", a função "pthread_create()" cria uma thread
		// que é reponsável por gerenciar as funções do "terminal de arte". 
		else if (strcmp(client, "client2") == 0) {
			logServer(" ART MODULE CONNECTED.\n");

			char aux[5];

			sprintf(aux, "art%d", art_count);

			struct ArtThreadStruct params;
			params.num = art_count;
			params.clientSocket = clientSocket;
			art_count++;
			if (pthread_create(&client_threads[thread_count], NULL, artHandler, &params) != 0) {
				logServer("Erro ao criar a thread do cliente");
				closesocket(clientSocket);
			}
		}
		// Caso a mensagem recebida seja "client3", a função "pthread_create()" cria uma thread
		// que é reponsável por gerenciar as funções do "terminal de catraca" 
		else if (strcmp(client, "client3") == 0) {
			logServer(" GATE MODULE CONNECTED.\n");

			char aux[5];

			sprintf(aux, "theme%d", theme_count);

			struct ThemeThreadStruct params;
			params.num = theme_count;
			params.clientSocket = clientSocket;
			theme_count++;
			if (pthread_create(&client_threads[thread_count], NULL, gateHandler, &params) != 0) {
				logServer("Erro ao criar a thread do cliente");
				closesocket(clientSocket);
			}
		}
	}
}

int main() {
	pthread_t server_thread;
	pthread_create(&server_thread, NULL, threadHandler, NULL);
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	int key;

	setWindowSize(console);

	// Loop que mantém o fluxo de menu funcionando.
	while(1){
		system("cls");
		printf("SERVER MENU\n");
		printf("1. Gerar relatorio diario do museu\n");
		printf("2. Gerar relatorio completo do museu\n");
		printf("3. Imprimir log do servidor\n");

		if (_kbhit() == NULL) {
			key = _getch();
			switch (key) {
			case 49:
				generateDailyReport();
				printf("Pressione alguma tecla para continuar...\n");
				_getch();
				system("cls");
				break;
			case 50:
				generateCompleteReport();
				printf("Pressione alguma tecla para continuar...\n");
				_getch();
				system("cls");
				break;
			case 51:
				printServerLog();
				printf("Pressione alguma tecla para continuar...\n");
				_getch();
				system("cls");
				break;
			default:
				continue;
			}
		}	
	}
}