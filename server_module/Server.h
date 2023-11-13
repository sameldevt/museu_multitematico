#pragma once
#ifndef SERVER_SETUP
#define SERVER_SETUP

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT "27015"

void logError(char* error_message);

SOCKET serverSetup();

#endif
