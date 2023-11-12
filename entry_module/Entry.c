#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

#include "Files.h"
#include "Misc.h"

/*
 *	Verifica a entrada normal de um cliente.
 * 
 *	@param clientSocket									soquete do cliente a ser verificado.
 * 
 *  @return	0											caso as informações do usuários forem válidas.
 */
int verifyUserInfo(SOCKET clientSocket) {
	int iResult;
	char response[8], entry[7] = "entry1";

	while (1) {
		/*
		 *	Envia ao servidor o tipo de entrada a ser verificado.
		 *
		 *  @param clientSocket							soquete do cliente para enviar o tipo de entrada.
		 *  @param entry								string que representa o tipo de entrada.
		 *  @param sizeof(entry)						tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */	
		iResult = send(clientSocket, entry, sizeof(entry), 0);
		if (iResult == SOCKET_ERROR) {
			continue;
		}

		loadLoadingScreen();
		loadScreen(MENU_NORMAL_ENTRY);

		/*
		 *	Solicita e recebe uma entrada de usuário e atribui ao ponteiro "name".
		 *  
		 *  @param 52									tamanho máximo da string.
		 *  @param 69									coordenada "x" onde será posicionado o cursor.
		 *  @param 22									coordenada "y" onde será posicionado o cursor.
		 */
		char *name = enterInput(52, 69, 22);
		 
		/*
		 *	Envia ao servidor o nome entrado anteriormente.
		 *
		 *  @param clientSocket							soquete do cliente para enviar o nome do usuário.
		 *  @param name									string que representa o nome do usuário.
		 *  @param strlen(name)							tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */
		iResult = send(clientSocket, name, strlen(name), 0);

		/*
		 *	Recebe do servidor a resposta da verificação.
		 *
		 *  @param clientSocket							soquete do cliente para receber a resposta da verificação.
		 *  @param response								string que representa a resposta do servidor.
		 *  @param sizeof(response)						tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */
		iResult = recv(clientSocket, response, sizeof(response), 0);

		//	Verifica a resposta recebida. Caso seja "failed", o loop é reiniciado.
		if (strcmp(response, "failed") == 0) {
			continue;
		}

		free(name);
		break;
	}

	//	Desativa o cursor do console.
	setCursorView(0);
	loadScreen(MENU_NORMAL_ENTRY_OK);
	Sleep(1500);
	return 0;
}

/*
 *	Verifica a meia entrada de estudante de um cliente.
 *
 *	@param clientSocket									soquete do cliente a ser verificado.
 *
 *  @return 0											caso as informações do usuários forem válidas.
 */
int verifyStudentInfo(SOCKET clientSocket) {
	int iResult;
	char info[120], response[8], entry[7] = "entry2";

	loadLoadingScreen();
	loadScreen(MENU_STUDENT_ENTRY);

	/*
	 *	Solicita e recebe uma entrada de usuário e atribui ao ponteiro "name".
	 *
	 *  @param 52										tamanho máximo da string.
	 *  @param 69										coordenada "x" onde será posicionado o cursor.
	 *  @param 22										coordenada "y" onde será posicionado o cursor.
	 */
	char* name = enterInput(52, 69, 22);

	while (1) {
		/*
		 *	Envia ao servidor o tipo de entrada a ser verificado.
		 *
		 *  @param clientSocket							soquete do cliente para enviar o tipo de entrada.
		 *  @param entry								string que representa o tipo de entrada.
		 *  @param sizeof(entry)						tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */
		iResult = send(clientSocket, entry, sizeof(entry), 0);
		if (iResult == SOCKET_ERROR) {
			continue;
		}

		loadScreen(MENU_STUDENT_ENTRY);

		/*
		 *	Define a cor de fundo e do texto do console.
		 *  
		 *  @param GetStdHandle(STD_OUTPUT_HANDLE)		saída padrão (console).
		 *  @param 240									valor que representa as cores do console.
		 */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

		/*
		 *	Define a posição do cursor do console.
		 *
		 *  @param 69									coordenada "x" onde será posicionado o cursor.
		 *  @param 22									coordenada "y" onde será posicionado o cursor.
		 */
		setCursorLocation(69, 22);
		printf("%s", name);

		/*
		 *	Solicita e recebe uma entrada de usuário e atribui ao
		 *  ponteiro "student_id".
		 *
		 *  @param 52									tamanho máximo da string.
		 *  @param 69									coordenada "x" onde será posicionado o cursor.
		 *  @param 26									coordenada "y" onde será posicionado o cursor.
		 */
		char *student_id = enterInput(52, 69, 26);

		sprintf(info, "%s,%s", name, student_id);

		/*
		 *	Envia ao servidor as informações do cliente.
		 *
		 *  @param clientSocket							soquete do cliente para enviar as informações do usuário.
		 *  @param info									string que representa as informações do cliente.
		 *  @param sizeof(info)							tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */
		iResult = send(clientSocket, info, sizeof(info), 0);

		/*
		 *	Recebe do servidor a resposta da verificação.
		 *
		 *  @param clientSocket							soquete do cliente para receber a resposta da verificação.
		 *  @param response								string que representa a resposta do servidor.
		 *  @param sizeof(response)						tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */
		iResult = recv(clientSocket, response, 8, 0);

		//	Verifica a resposta recebida. Caso seja "failed", o loop é reiniciado.
		if (strcmp(response, "failed") == 0) {

			//	Desativa o cursor do console.
			setCursorView(0);
			loadScreen(MENU_STUDENT_ENTRY_FAIL);
			free(student_id);
			Sleep(1500);
			continue;
		}

		free(name);
		free(student_id);
		break;
	}

	//	Desativa o cursor do console.
	setCursorView(0);
	loadScreen(MENU_STUDENT_ENTRY_OK);
	Sleep(1500);
	return 0;
}

/*
 *	Verifica a meia entrada da pessoa com deficiência de um cliente.
 *
 *	@param clientSocket									soquete do cliente a ser verificado.
 *
 *  @return 0											caso as informações do usuários forem válidas.
 */
int verifyDisabledPersonInfo(SOCKET clientSocket) {
	int iResult;
	char info[120], response[8], entry[7] = "entry3";

	loadLoadingScreen();
	loadScreen(MENU_DISABLED_PERSON_ENTRY);

	/*
	 *	Solicita e recebe uma entrada de usuário e atribui ao
	 *  ponteiro "name".
	 *
	 *  @param 52										tamanho máximo da string.
	 *  @param 69										coordenada "x" onde será posicionado o cursor.
	 *  @param 22										coordenada "y" onde será posicionado o cursor.
	 */
	char* name = enterInput(52, 69, 22);

	while (1) {
		/*
		 *	Envia ao servidor o tipo de entrada a ser verificado.
		 *
		 *  @param clientSocket							soquete do cliente para enviar o tipo de entrada.
		 *  @param entry								string que representa o tipo de entrada.
		 *  @param sizeof(entry)						tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */
		iResult = send(clientSocket, entry, sizeof(entry), 0);
		if (iResult == SOCKET_ERROR) {
			continue;
		}

		loadScreen(MENU_DISABLED_PERSON_ENTRY);

		/*
		 *	Define a cor de fundo e do texto do console.
		 *
		 *  @param GetStdHandle(STD_OUTPUT_HANDLE)		saída padrão (console).
		 *  @param 240									valor que representa as cores do console.
		 */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

		/*
		 *	Define a posição do cursor do console.
		 *
		 *  @param 69									coordenada "x" onde será posicionado o cursor.
		 *  @param 22									coordenada "y" onde será posicionado o cursor.
		 */
		setCursorLocation(69, 22);
		printf("%s", name);

		/*
		 *	Solicita e recebe uma entrada de usuário e atribui ao
		 *  ponteiro "disabled_person_id".
		 *
		 *  @param 52									tamanho máximo da string.
		 *  @param 69									coordenada "x" onde será posicionado o cursor.
		 *  @param 26									coordenada "y" onde será posicionado o cursor.
		 */
		char* disabled_person_id = enterInput(52, 69, 26);

		sprintf(info, "%s,%s", name, disabled_person_id);

		/*
		 *	Envia ao servidor as informações do cliente.
		 *
		 *  @param clientSocket							soquete do cliente para enviar as informações do usuário.
		 *  @param info									string que representa as informações do cliente.
		 *  @param sizeof(info)							tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */
		iResult = send(clientSocket, info, sizeof(info), 0);

		/*
		 *	Recebe do servidor a resposta da verificação.
		 *
		 *  @param clientSocket							soquete do cliente para receber a resposta da verificação.
		 *  @param response								string que representa a resposta do servidor.
		 *  @param sizeof(response)						tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */
		iResult = recv(clientSocket, response, sizeof(response), 0);

		//	Verifica a resposta recebida. Caso seja "failed", o loop é reiniciado.
		if (strcmp(response, "failed") == 0) {

			//	Desativa o cursor do console.
			setCursorView(0);
			loadScreen(MENU_DISABLED_PERSON_ENTRY_FAIL);
			free(disabled_person_id);
			Sleep(1500);
			continue;
		}

		free(name);
		free(disabled_person_id);
		break;
	}

	//	Desativa o cursor do console.
	setCursorView(0);
	loadScreen(MENU_DISABLED_PERSON_ENTRY_OK);
	Sleep(1500);
	return 0;
}

/*
 *	Verifica a entra gratuita de criança de um cliente.
 *
 *	@param clientSocket									soquete do cliente a ser verificado.
 *
 *  @return 0											caso as informações do usuários forem válidas.
 */
int verifyJuniorInfo(SOCKET clientSocket) {
	int iResult;
	char response[8], info[110], entry[7] = "entry4";

	loadLoadingScreen();
	loadScreen(MENU_JUNIOR_ENTRY);

	/*
	 *	Solicita e recebe uma entrada de usuário e atribui ao ponteiro "name".
	 *
	 *  @param 52										tamanho máximo da string.
	 *  @param 69										coordenada "x" onde será posicionado o cursor.
	 *  @param 22										coordenada "y" onde será posicionado o cursor.
	 */
	char* name = enterInput(52, 69, 22);

	while (1) {
		/*
		 *	Envia ao servidor o tipo de entrada a ser verificado.
		 *
		 *  @param clientSocket							soquete do cliente para enviar o tipo de entrada.
		 *  @param entry								string que representa o tipo de entrada.
		 *  @param sizeof(entry)						tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */
		iResult = send(clientSocket, entry, sizeof(entry), 0);
		if (iResult == SOCKET_ERROR) {
			continue;
		}

		loadScreen(MENU_JUNIOR_ENTRY);

		/*
		 *	Define a cor de fundo e do texto do console.
		 *
		 *  @param GetStdHandle(STD_OUTPUT_HANDLE)		saída padrão (console).
		 *  @param 240									valor que representa as cores do console.
		 */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

		/*
		 *	Define a posição do cursor do console.
		 *
		 *  @param 69									coordenada "x" onde será posicionado o cursor.
		 *  @param 22									coordenada "y" onde será posicionado o cursor.
		 */
		setCursorLocation(69, 22);
		printf("%s", name);

		/*
		 *	Solicita e recebe uma entrada de usuário e atribui ao ponteiro "age".
		 *
		 *  @param 52									tamanho máximo da string.
		 *  @param 69									coordenada "x" onde será posicionado o cursor.
		 *  @param 26									coordenada "y" onde será posicionado o cursor.
		 */
		char* age = enterInput(52, 69, 26);

		sprintf(info, "%s,%s", name, age);

		/*
		 *	Envia ao servidor as informações do cliente.
		 *
		 *  @param clientSocket							soquete do cliente para enviar as informações do usuário.
		 *  @param info									string que representa as informações do cliente.
		 *  @param sizeof(info)							tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */
		iResult = send(clientSocket, info, sizeof(info), 0);

		/*
		 *	Recebe do servidor a resposta da verificação.
		 *
		 *  @param clientSocket							soquete do cliente para receber a resposta da verificação.
		 *  @param response								string que representa a resposta do servidor.
		 *  @param sizeof(response)						tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */
		iResult = recv(clientSocket, response, sizeof(response), 0);

		//	Verifica a resposta recebida. Caso seja "failed", o loop é reiniciado.
		if (strcmp(response, "failed") == 0) {

			//	Desativa o cursor do console.
			setCursorView(0);
			loadScreen(MENU_JUNIOR_ENTRY_FAIL);
			free(age);
			Sleep(1500);
			continue;
		}

		free(age);
		free(name);
		break;
	}

	//	Desativa o cursor do console.
	setCursorView(0);
	loadScreen(MENU_JUNIOR_ENTRY_OK);
	Sleep(1500);
	return 0;
}

/*
 *	Verifica a entrada gratuita de idoso de um cliente.
 *
 *	@param clientSocket									soquete do cliente a ser verificado.
 *
 *  @return 0											caso as informações do usuários forem válidas.
 */
int verifySeniorInfo(SOCKET clientSocket) {
	int iResult;
	char info[120], response[8], entry[7] = "entry5";

	loadLoadingScreen();
	loadScreen(MENU_SENIOR_ENTRY);

	/*
	 *	Solicita e recebe uma entrada de usuário e atribui ao
	 *  ponteiro "name".
	 *
	 *  @param 52 O tamanho máximo da string.
	 *  @param 69 Coordenada "x" onde será posicionado o cursor.
	 *  @param 22 Coordenada "y" onde será posicionado o cursor.
	 */
	char* name = enterInput(52, 69, 22);

	while (1) {
		/*
		 *	Envia ao servidor o tipo de entrada a ser verificado.
		 *
		 *  @param clientSocket							soquete do cliente para enviar o tipo de entrada.
		 *  @param entry								string que representa o tipo de entrada.
		 *  @param sizeof(entry)						tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */
		iResult = send(clientSocket, entry, sizeof(entry), 0);
		if (iResult == SOCKET_ERROR) {
			continue;
		}

		loadScreen(MENU_SENIOR_ENTRY);

		/*
		 *	Define a cor de fundo e do texto do console.
		 *
		 *  @param GetStdHandle(STD_OUTPUT_HANDLE)		saída padrão (console).
		 *  @param 240									valor que representa as cores do console.
		 */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

		/*
		 *	Define a posição do cursor do console.
		 *
		 *  @param 69									coordenada "x" onde será posicionado o cursor.
		 *  @param 22									coordenada "y" onde será posicionado o cursor.
		 */
		setCursorLocation(69, 22);
		printf("%s", name);

		/*
		 *	Solicita e recebe uma entrada de usuário e atribui ao ponteiro "age".
		 *
		 *  @param 52									tamanho máximo da string.
		 *  @param 69									coordenada "x" onde será posicionado o cursor.
		 *  @param 26									coordenada "y" onde será posicionado o cursor.
		 */
		char* age = enterInput(52, 69, 26);

		sprintf(info, "%s,%s", name, age);

		/*
		 *	Envia ao servidor as informações do cliente.
		 *
		 *  @param clientSocket							soquete do cliente para enviar as informações do usuário.
		 *  @param info									string que representa as informações do cliente.
		 *  @param sizeof(info)							tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */
		iResult = send(clientSocket, info, sizeof(info), 0);

		/*
		 *	Recebe do servidor a resposta da verificação.
		 *
		 *  @param clientSocket							soquete do cliente para receber a resposta da verificação.
		 *  @param response								string que representa a resposta do servidor.
		 *  @param sizeof(response)						tamanho da string.
		 *  @param 0									flag que modifica o comportamento da função "send()".
		 */
		iResult = recv(clientSocket, response, sizeof(response), 0);

		//	Verifica a resposta recebida. Caso seja "failed", o loop é reiniciado.
		if (strcmp(response, "failed") == 0) {

			//	Desativa o cursor do console.
			setCursorView(0);
			loadScreen(MENU_SENIOR_ENTRY_FAIL);
			free(age);
			Sleep(1500);
			continue;
		}

		free(age);
		free(name);
		break;
	}

	//	Desativa o cursor do console.
	setCursorView(0);
	loadScreen(MENU_SENIOR_ENTRY_OK);
	Sleep(1500);
	return 0;
}

/*
 *	Verifica qual entrada o usuário escolheu.
 *
 *	@param clientSocket									soquete do cliente a ser verificado.
 *
 *  @return												Retorna o valor correspondente a entrada escolhida caso a 
 *														verificação seja concluída com êxito.
 */
int selectEntry(SOCKET clientSocket) {
	int key, iResult;

	//	Verifica se as informações que o usuário entrou são validas.
	while (_kbhit() == NULL) {
		loadScreen(TICKET_TYPE_MENU);
		key = _getch();
		switch (key) {
		case 49:
			if (verifyUserInfo(clientSocket) == 0) {
				return 0;
			}
		case 50:
			loadScreen(MENU_HALFPRICE_ENTRY);
			while (_kbhit() == NULL) {
				key = _getch();
				switch (key) {
				case 49:
					if(verifyStudentInfo(clientSocket) == 0){
						return 1;
					}
				case 50:
					if (verifyDisabledPersonInfo(clientSocket) == 0) {
						return 1;
					}
				default:
					continue;
				}
				Sleep(500);
			}
		case 51:
			loadScreen(MENU_FREE_ENTRY);
			while (_kbhit() == NULL) {
				key = _getch();
				switch (key) {
				case 49:
					if (verifyJuniorInfo(clientSocket) == 0) {
						return 2;
					}
				case 50:
					if (verifySeniorInfo(clientSocket) == 0) {
						return 2;
					}
				default:
					continue;
				}
				Sleep(500);
			}
		default:
			continue;
		}
		Sleep(500);
	}
}