#pragma once
#ifndef PAYMENT_PROCESS
#define PAYMENT_PROCESS

#include "Server.h"

void sendFailMessage(SOCKET clientSocket);

/* Fun��o que verifica a entrada de informa��es do cart�o do usu�rio */
int verifyCardPayment(SOCKET clientSocket);

/* Fun��o que verifica a entrada de valor do usu�rio */
int verifyPixPayment(SOCKET clientSocket);

int paymentHandler(SOCKET clientSocket);

#endif