#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Server.h"

#define ERROR_LOG_FILE "resources\\error_log\\error_log.txt"

struct addrinfo* result = NULL, * ptr = NULL, hints;

void logError(char* error_message) {
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	FILE* log_file = fopen(ERROR_LOG_FILE, "a");

	fprintf(log_file, "[%04d-%02d-%02d %02d:%02d:%02d] Error: %s\n",
		timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
		timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, error_message);

	fclose(log_file);
}


void sendFailMessage(SOCKET clientSocket) {
	logError("winsock error %d");
	send(clientSocket, "failed", 7, 0);
}

void sendOk(SOCKET clientSocket) {
	int iResult = send(clientSocket, "sucess\0", 7, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
	}
}

void sendError(SOCKET clientSocket) {
	int iResult = send(clientSocket, "error\0", 6, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
	}
}

SOCKET serverSetup() {
	WSADATA wsaData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(serverSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}

	return serverSocket;
}