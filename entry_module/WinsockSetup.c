#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#include "Files.h"

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
		WSACleanup();
		return 1;
	}

	// Cria um soquete usando as informações definidas anteriormente.
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	
	return clientSocket;
}

// Se conecta a um soquete de servidor e envia uma mensagem referente ao cliente conectado.
int connectSocket(SOCKET clientSocket) {
	int iResult;
	char client[8] = "client1\0";

	/*
	 * Se conecta a um servidor.
	 *
	 * @param clientSocket					soquete do cliente para se conectar ao servidor.
	 * @param result->ai_addr				endereço ip contido em "ai_addr" na struct "result".
	 * @param (int)result->ai_addrlen)		casting do tamanho do endereço ip contido em "ai_addrlen" na struct "result" para inteiro.
	 */
	iResult = connect(clientSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(clientSocket);
		clientSocket = INVALID_SOCKET;
	}

	/*
	 * Envia ao servidor o tipo de cliente conectado.
	 *
	 * @param clientSocket		soquete do cliente para se conectar ao servidor.
	 * @param client			string que representa o tipo de cliente.
	 * @param 8					tamanho da string.
	 * @param 0					flag que modifica o comportamento da função "send()".
	 */
	iResult = send(clientSocket, client, 8, 0);
	if (iResult == SOCKET_ERROR) {
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	return 0;
}