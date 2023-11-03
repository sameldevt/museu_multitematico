#pragma once
#ifndef PAYMENT
#define PAYMENT

int verifyCardInfo(SOCKET clientSocket, int entry, int ticket_count);

int verifyPixInfo(SOCKET clientSocket, int entry, int ticket_count);

int selectPayment(SOCKET clientSocket, int entry, int ticket_count);
#endif