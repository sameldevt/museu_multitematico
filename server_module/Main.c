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

struct tm* timeinfo;
struct ArtThreadStruct {
	int num;
	SOCKET clientSocket;
};

struct ThemeThreadStruct {
	int num;
	SOCKET clientSocket;
};

/* 
 *	A fun��o abaixo tem como par�metro um identificador para o console do Windows
 *  e � respons�vel por ajustar o tamanho da mesma bem como reposicion�-la no lado 
 *  esquerdo da tela.
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

		// Obt�m informa��es sobre a tela
		RECT consoleRect;
		GetWindowRect(consoleWindow, &consoleRect);

		// Obt�m as dimens�es da tela
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

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
 *	A fun��o abaixo n�o possui valores como par�metro e � respons�vel por gerenciar
 *	threads criadas pelo programa.
 */
void* threadHandler() {
	int result, thread_count = 0, art_count = 0, theme_count = 0;
	char buffer[100], message[50], client[8], init_time[54];
	pthread_t *client_threads[100];
	time_t rawtime;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(init_time, "\nSERVER INITIALIZED AT [%04d-%02d-%02d %02d:%02d:%02d]\n",
		timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
		timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	FILE* fp = fopen("server_resources\\server_log.txt", "a");
	fprintf(fp, init_time);
	fclose(fp);

	// Criar um soquete para o servidor.
	SOCKET serverSocket = serverSetup();

	// Loop para aceitar conex�es de clientes no servidor.
	while (1) {

		// Aguarda a conex�o de um cliente.
		if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
			WSACleanup();
			continue;
		}

		// Aceita uma conex�o do cliente e cria um soquete para o mesmo.
		SOCKET clientSocket = accept(serverSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			closesocket(clientSocket);
			WSACleanup();
			continue;
		}

		// Recebe uma mensagem que representa o tipo de cliente conectado.
		result = recv(clientSocket, client, 8, 0);
		logServer("Client connected - ");

		// Caso a mensagem recebida seja "client1", a fun��o "pthread_create()" cria uma thread
		// que � repons�vel por gerenciar as fun��es do "terminal de entrada".
		if (strcmp(client, "client1") == 0) {
			logServer(" ENTRY MODULE CONNECTED.\n");
			if (pthread_create(&client_threads[thread_count], NULL, terminalHandler, clientSocket) != 0) {
				logServer("Erro ao criar a thread do cliente");
				closesocket(clientSocket);
			}
		}

		// Caso a mensagem recebida seja "client2", a fun��o "pthread_create()" cria uma thread
		// que � repons�vel por gerenciar as fun��es do "terminal de arte". 
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
		// Caso a mensagem recebida seja "client3", a fun��o "pthread_create()" cria uma thread
		// que � repons�vel por gerenciar as fun��es do "terminal de catraca" 
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

	char date[10];
	int key;

	setWindowSize(console);

	// Loop que mant�m o fluxo de menu funcionando.
	while(1){
		system("cls");
		printf("MENU DO SERVIDOR\n\n");
		printf("1. Gerar relatorio do museu\n");
		printf("2. Imprimir log do servidor\n");

		if (_kbhit() == NULL) {
			key = _getch();
			switch (key) {
			case 49:
				system("cls");
				printf("RELATORIO\n");
				printf("\n1. Relatorio diario\n");
				printf("2. Relatorio completo\n");

				key = _getch();
				switch (key) {
				case 49:
					system("cls");
					printf("RELATORIO\n");
					printf("\nDigite a data (dd-mm): ");
					scanf("%s", date);

					generateReport(date);
					printf("\nPressione alguma tecla para continuar...\n");
					_getch();
					system("cls");
					continue;

				case 50:
					system("cls");
					generateReport("00-00");
					printf("\nPressione alguma tecla para continuar...\n");
					_getch();
					system("cls");
					continue;
				}
			case 50:
				printServerLog();
				printf("\nPressione alguma tecla para continuar...\n");
				_getch();
				system("cls");
				break;	
			default:
				continue;
			}
		}	
	}
}