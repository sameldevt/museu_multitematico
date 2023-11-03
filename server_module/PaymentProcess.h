#pragma once
#ifndef PAYMENT_PROCESS
#define PAYMENT_PROCESS

#include "Server.h"

void sendFailMessage(SOCKET clientSocket);

/* Função que verifica a entrada de informações do cartão do usuário */
int verifyCardPayment(SOCKET clientSocket);

/* Função que verifica a entrada de valor do usuário */
int verifyPixPayment(SOCKET clientSocket);

int paymentHandler(SOCKET clientSocket);

#endif