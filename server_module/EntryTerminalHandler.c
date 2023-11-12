#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PaymentProcess.h"
#include "FileProcess.h"
#include "EntryProcess.h"
#include "Server.h"

/*
 * Gerencia a thread do terminal de entrada. Esse terminal � repons�vel pela
 * verifica��o de pagamento e de usu�rio.
 *
 * @param arg	ponteiro para void que representa os argumentos usados na cria��o da thread.
 */
void* terminalHandler(void* arg) {
	SOCKET clientSocket = (SOCKET*)arg;
	int iResult, pos = 0;

	// Loop que mant�m o fluxo do terminal de entrada rodando.
	while (1) {
		if (entryHandler(clientSocket) == 0) {
			if (paymentHandler(clientSocket) == 0) {
				continue;
			}
		}
	}
}