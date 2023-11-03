#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#pragma once
#ifndef WINSOCK_SETUP
#define WINSOCK_SETUP

SOCKET socketSetup();

int connectSocket(SOCKET clientSocket);

#endif