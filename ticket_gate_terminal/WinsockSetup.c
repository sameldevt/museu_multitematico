#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define DEFAULT_PORT "27015"

struct addrinfo* result = NULL;
struct addrinfo* ptr = NULL;
struct addrinfo hints;

/*
 * Configura um soquete para o cliente.
 *
 * @return clientSocket		caso a configuração do soquete seja feita com sucesso.
 */
SOCKET socketSetup() {

	WSADATA wsaData;
	int iResult;

	// Inicializa a biblioteca Winsock.
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	// Preenche o bloco de memória de "hints" com zeros.
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Obtem as informações associadas ao host.
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Cria um soquete usando as informações definidas anteriormente.
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	return clientSocket;
}

// Se conecta a um soquete de servidor.
int connectSocket(SOCKET clientSocket) {
	int iResult = connect(clientSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(clientSocket);
		clientSocket = INVALID_SOCKET;
	}

	return 0;
}