#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PaymentProcess.h"
#include "FileProcess.h"
#include "EntryProcess.h"
#include "Server.h"

void* terminalHandler(void* arg) {
	SOCKET clientSocket = (SOCKET*)arg;
	int iResult, pos = 0;

	while (1) {
		if (entryHandler(clientSocket) == 0) {
			if (paymentHandler(clientSocket) == 0) {
				continue;
			}
		}
	}
}