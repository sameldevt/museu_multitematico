#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PaymentProcess.h"
#include "FileProcess.h"
#include "EntryProcess.h"
#include "Server.h"

/*
 * Verifica a entrada normal de um cliente.
 *
 * @param clientSocket		soquete do cliente a ser verificado.
 *
 * @return 0				caso as informa��es do usu�rios forem v�lidas.
 */
int verifyUserEntry(SOCKET clientSocket) {
	char name[100], * token, *tokens[2];
	int iResult, count = 0;

	while (1) {
		/*
		 * Recebe do cliente o nome do usu�rio a ser verificado.
		 *
		 * @param clientSocket		soquete do cliente para receber o nome do usu�rio.
		 * @param name				string que representa o nome do usu�rio.
		 * @param sizeof(name)		tamanho da string.
		 * @param 0					flag que modifica o comportamento da fun��o "recv()".
		 */
		iResult = recv(clientSocket, name, sizeof(name), 0);
		if (iResult == SOCKET_ERROR) {
			sendFailMessage(clientSocket);
			continue;
		}

		name[iResult] = '\0';

		// Registra o usu�rio no sistema.
		registerUser(name);

		/*
		 * Envia ao cliente o resultado da verifica��o.
		 *
		 * @param clientSocket		soquete do cliente para enviar a resposta da verifica��o.
		 * @param success			string que representa o resultado da avalia��o.
		 * @param 8					tamanho da string.
		 * @param 0					flag que modifica o comportamento da fun��o "send()".
		 */
		iResult = send(clientSocket, "success", 8, 0);
		if (iResult == SOCKET_ERROR) {
			sendFailMessage(clientSocket);
			continue;
		}
		break;
	}

	// Registra no sistema as informa��es do ingresso.
	writeFile("server_resources\\ticket_info_temp.csv", "  ENTRADA CHEIA \0");
	return 0;
}

/*
 * Verifica a meia entrada de estudante de um cliente.
 *
 * @param clientSocket		soquete do cliente a ser verificado.
 *
 * @return 0				caso as informa��es do usu�rios forem v�lidas.
 */
int verifyStudentEntry(SOCKET clientSocket) {
	int iResult, count = 0;
	char info[120], * tokens[2], * token, name[101], student_id[9];

	while (1) {
		/*
		 * Recebe do cliente as informa��es do usu�rio.
		 *
		 * @param clientSocket		soquete do cliente para receber as informa��es do usu�rio.
		 * @param info				string que representa as informa��es do cliente.
		 * @param sizeof(info)		tamanho da string.
		 * @param 0					flag que modifica o comportamento da fun��o "recv()".
		 */
		iResult = recv(clientSocket, info, sizeof(info), 0);
		if (iResult == SOCKET_ERROR) {
			sendFailMessage(clientSocket);
		}

		token = strtok(info, ",");
		
		while (token != NULL && count < 2) {
			tokens[count] = token;
			count++;
			token = strtok(NULL, ",");
		}

		strcpy(name, tokens[0]);
		strcpy(student_id, tokens[1]);

		// Verifica se o tamanho do R.A do estudante � v�lido.
		if (strlen(student_id) == 8) {

			/*
			 * Envia ao cliente o resultado da verifica��o.
			 *
			 * @param clientSocket		soquete do cliente para enviar a resposta da verifica��o.
			 * @param success			string que representa o resultado da avalia��o.
			 * @param 8					tamanho da string.
			 * @param 0					flag que modifica o comportamento da fun��o "send()".
			 */
			iResult = send(clientSocket, "success", 8, 0);
			if (iResult == SOCKET_ERROR) {
				sendFailMessage(clientSocket);
			}
			break;
		}
		else {

			/*
			 * Envia ao cliente o resultado da verifica��o.
			 *
			 * @param clientSocket		soquete do cliente para enviar a resposta da verifica��o.
			 * @param failed			string que representa o resultado da avalia��o.
			 * @param 7					tamanho da string.
			 * @param 0					flag que modifica o comportamento da fun��o "send()".
			 */
			iResult = send(clientSocket, "failed", 7, 0);
			if (iResult == SOCKET_ERROR) {
				sendFailMessage(clientSocket);
			}
			continue;
		}
	}

	// Registra o usu�rio no sistema.
	registerUser(name);

	/*
	 * Envia ao cliente o resultado da verifica��o.
	 *
	 * @param clientSocket		soquete do cliente para enviar a resposta da verifica��o.
	 * @param success			string que representa o resultado da avalia��o.
	 * @param 8					tamanho da string.
	 * @param 0					flag que modifica o comportamento da fun��o "send()".
	 */
	iResult = send(clientSocket, "success", 8, 0);
	if (iResult == SOCKET_ERROR) {
		sendFailMessage(clientSocket);
	}

	// Registra no sistema as informa��es do ingresso.
	writeFile("server_resources\\ticket_info_temp.csv", "    MEIA ENTRADA  \0");

	return 0;
}

/*
 * Verifica a entrada gratuita de idoso de um cliente.
 *
 * @param clientSocket	soquete do cliente a ser verificado.
 *
 * @return 0			caso as informa��es do usu�rios forem v�lidas.
 */
int verifySeniorEntry(SOCKET clientSocket) {
	int iResult, count = 0;
	char info[120], * token, * tokens[2], name[100];

	while (1) {
		/*
		 * Recebe do cliente as informa��es do usu�rio.
		 *
		 * @param clientSocket		soquete do cliente para receber as informa��es do usu�rio.
		 * @param info				string que representa as informa��es do cliente.
		 * @param sizeof(info)		tamanho da string.
		 * @param 0					flag que modifica o comportamento da fun��o "recv()".
		 */
		iResult = recv(clientSocket, info, sizeof(info), 0);
		if (iResult == SOCKET_ERROR) {
			sendFailMessage(clientSocket);
		}

		token = strtok(info, ",");

		while (token != NULL && count < 2) {
			tokens[count] = token;
			count++;
			token = strtok(NULL, ",");
		}

		strcpy(name, tokens[0]);
		int age = atoi(tokens[1]);

		// Verifica se a idade do usu�rio � v�lida.
		if (age < 60) {
			/*
			 * Envia ao cliente o resultado da verifica��o.
			 *
			 * @param clientSocket		soquete do cliente para enviar a resposta da verifica��o.
			 * @param failed			string que representa o resultado da avalia��o.
			 * @param 7					tamanho da string.
			 * @param 0					flag que modifica o comportamento da fun��o "send()".
			 */
			iResult = send(clientSocket, "failed", 7, 0);
			if (iResult == SOCKET_ERROR) {
				sendFailMessage(clientSocket);
			}
			continue;
		}
		break;
	}

	// Registra o usu�rio no sistema.
	registerUser(name);
	
	/*
	 * Envia ao cliente o resultado da verifica��o.
	 *
	 * @param clientSocket		soquete do cliente para enviar a resposta da verifica��o.
	 * @param success			string que representa o resultado da avalia��o.
	 * @param 8					tamanho da string.
	 * @param 0					flag que modifica o comportamento da fun��o "send()".
	 */
	iResult = send(clientSocket, "success", 8, 0);
	if (iResult == SOCKET_ERROR) {
		sendFailMessage(clientSocket);
	}

	char entry_info[100];

	sprintf(entry_info, "  ENTRADA GRATUITA,%d,%.2f,%.2f,      \0", 0, 0.0, 100.0);

	// Registra no sistema as informa��es do ingresso.
	writeFile("server_resources\\ticket_info_temp.csv", entry_info);

	return 0;
}

/*
 * Verifica a entra gratuita de crian�a de um cliente.
 *
 * @param clientSocket		soquete do cliente a ser verificado.
 *
 * @return 0				caso as informa��es do usu�rios forem v�lidas.
 */
int verifyJuniorEntry(SOCKET clientSocket) {
	int iResult, count = 0;
	char info[120], * token, * tokens[2], name[100];

	while (1) {
		/*
		 * Recebe do cliente as informa��es do usu�rio.
		 *
		 * @param clientSocket		soquete do cliente para receber as informa��es do usu�rio.
		 * @param info				string que representa as informa��es do cliente.
		 * @param sizeof(info)		tamanho da string.
		 * @param 0					flag que modifica o comportamento da fun��o "recv()".
		 */
		iResult = recv(clientSocket, info, sizeof(info), 0);
		if (iResult == SOCKET_ERROR) {
			sendFailMessage(clientSocket);
		}

		token = strtok(info, ",");

		while (token != NULL && count < 2) {
			tokens[count] = token;
			count++;
			token = strtok(NULL, ",");
		}

		strcpy(name, tokens[0]);
		int age = atoi(tokens[1]);

		// Verifica se a idade do usu�rio � v�lida.
		if (age > 5) {
			/*
			 * Envia ao cliente o resultado da verifica��o.
			 *
			 * @param clientSocket		soquete do cliente para enviar a resposta da verifica��o.
			 * @param failed			string que representa o resultado da avalia��o.
			 * @param 7					tamanho da string.
			 * @param 0					flag que modifica o comportamento da fun��o "send()".
			 */
			iResult = send(clientSocket, "failed", 7, 0);
			if (iResult == SOCKET_ERROR) {
				sendFailMessage(clientSocket);
			}
			continue;;
		}
		break;
	}

	// Registra o usu�rio no sistema.
	registerUser(name);
	
	/*
	 * Envia ao cliente o resultado da verifica��o.
	 *
	 * @param clientSocket		soquete do cliente para enviar a resposta da verifica��o.
	 * @param success			string que representa o resultado da avalia��o.
	 * @param 8					tamanho da string.
	 * @param 0					flag que modifica o comportamento da fun��o "send()".
	 */
	iResult = send(clientSocket, "success", 8, 0);
	if (iResult == SOCKET_ERROR) {
		sendFailMessage(clientSocket);
	}

	char entry_info[100];

	sprintf(entry_info, "  ENTRADA GRATUITA,%d,%.2f,%.2f,      \0", 0, 0.0, 100.0);

	// Registra no sistema as informa��es do ingresso.
	writeFile("server_resources\\ticket_info_temp.csv", entry_info);
	
	return 0;
}

/*
 * Verifica a meia entrada da pessoa com defici�ncia de um cliente.
 *
 * @param clientSocket		soquete do cliente a ser verificado.
 *
 * @return 0				caso as informa��es do usu�rios forem v�lidas.
 */
int verifyDisabledPersonEntry(SOCKET clientSocket) {
	int iResult, count = 0;
	char* tokens[2], * token, info[120], name[100], disabled_person_id[50];

	while (1) {
		/*
		 * Recebe do cliente as informa��es do usu�rio.
		 *
		 * @param clientSocket		soquete do cliente para receber as informa��es do usu�rio.
		 * @param info				string que representa as informa��es do cliente.
		 * @param sizeof(info)		tamanho da string.
		 * @param 0					flag que modifica o comportamento da fun��o "recv()".
		 */
		iResult = recv(clientSocket, info, sizeof(info), 0);
		if (iResult == SOCKET_ERROR) {
			sendFailMessage(clientSocket);
		}

		token = strtok(info, ",");

		while (token != NULL && count < 2) {
			tokens[count] = token;
			count++;
			token = strtok(NULL, ",");
		}

		strcpy(name, tokens[0]);
		strcpy(disabled_person_id, tokens[1]);

		// Verifica se o tamanho do documento da pessoa com defici�ncia � v�lido.
		if (strlen(disabled_person_id) == 11) {

			/*
			 * Envia ao cliente o resultado da verifica��o.
			 *
			 * @param clientSocket		soquete do cliente para enviar a resposta da verifica��o.
			 * @param success			string que representa o resultado da avalia��o.
			 * @param 8					tamanho da string.
			 * @param 0					flag que modifica o comportamento da fun��o "send()".
			 */
			iResult = send(clientSocket, "success", 8, 0);
			if (iResult == SOCKET_ERROR) {
				sendFailMessage(clientSocket);
			}
			break;
		}
		else {
			/*
			 * Envia ao cliente o resultado da verifica��o.
			 *
			 * @param clientSocket		soquete do cliente para enviar a resposta da verifica��o.
			 * @param failed			string que representa o resultado da avalia��o.
			 * @param 7					tamanho da string.
			 * @param 0					flag que modifica o comportamento da fun��o "send()".
			 */
			iResult = send(clientSocket, "failed", 7, 0);
			if (iResult == SOCKET_ERROR) {
				sendFailMessage(clientSocket);
			}
			continue;
		}
	}

	// Registra o usu�rio no sistema.
	registerUser(name);
	
	/*
	 * Envia ao cliente o resultado da verifica��o.
	 *
	 * @param clientSocket		soquete do cliente para enviar a resposta da verifica��o.
	 * @param success			string que representa o resultado da avalia��o.
	 * @param 8					tamanho da string.
	 * @param 0					flag que modifica o comportamento da fun��o "send()".
	 */
	iResult = send(clientSocket, "success", 8, 0);
	if (iResult == SOCKET_ERROR) {
		sendFailMessage(clientSocket);
	}

	// Registra no sistema as informa��es do ingresso.
	writeFile("server_resources\\ticket_info_temp.csv", "  MEIA ENTRADA  \0");

	return 0;
}

/*
 * Verifica qual entrada o usu�rio escolheu.
 *
 * @param clientSocket		soquete do cliente a ser verificado.
 *
 * @return 0				caso a verifica��o da entrada seja feita com �xito.
 * @return 1				caso a verifica��o da entrada n�o seja feita com �xito.
 */
int entryHandler(SOCKET clientSocket) {
	int iResult;
	char entry[7], buffer[10];

	/*
	 * Recebe do cliente o tipo de entrada a ser verificado.
	 *
	 * @param clientSocket		soquete do cliente para enviar o tipo de entrada.
	 * @param entry				string que representa o tipo de entrada.
	 * @param sizeof(entry)		tamanho da string.
	 * @param 0					flag que modifica o comportamento da fun��o "send()".
	 */
	iResult = recv(clientSocket, entry, sizeof(entry), 0);
	if (iResult == SOCKET_ERROR) {
		sendFailMessage(clientSocket);
	}

	if (strcmp(entry, "entry1") == 0) {
		if (verifyUserEntry(clientSocket) == 0) {
			return 0;
		}
	}
	else if (strcmp(entry, "entry2") == 0) {
		if (verifyStudentEntry(clientSocket) == 0){
			return 0;
		}
	}
	else if (strcmp(entry, "entry3") == 0) {
		if (verifyDisabledPersonEntry(clientSocket) == 0) {
			return 0;
		}
	}
	else if (strcmp(entry, "entry4") == 0) {
		if (verifyJuniorEntry(clientSocket) == 0) {
			return 0;
		}
	}
	else if (strcmp(entry, "entry5") == 0) {
		if (verifySeniorEntry(clientSocket) == 0) {
			return 0;
		}
	}

	return 1;
}