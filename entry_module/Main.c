#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "Misc.h"
#include "Files.h"
#include "Payment.h"
#include "Entry.h"

int main() {
	int result, entry, key, ticket_count, pos = 0;
	SOCKET clientSocket = socketSetup();
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	//	Tenta se conectar ao servidor.
	while (1) {
		result = connectSocket(clientSocket);
		if (result != 0) {
			Sleep(1000);
			continue;
		}
		break;
	}

	//	Define o tamanho da janela do console.
	setHorizontalSize();

	while (1) {
		switch (pos) {
		case 0:
			//	Desativa o cursor do console.
			setCursorView(0);
			if (_kbhit() == NULL) {
				loadScreen(WELCOME_SCREEN);
				if (key = _getch()) {
					pos = 1;
					break;
				}
			}
		case 1:
			//	Desativa o cursor do console.
			setCursorView(0);
			if (_kbhit() == NULL) {
				loadScreen(MENU_MAIN);
				key = _getch();
				switch (key) {
				case 49:
					pos = 2;
					break;
				case 50:
					if (_kbhit() == NULL) {
						loadScreen(MENU_THEMES);
						if (_getch()) {
							break;
						}
					}
				case 81:
				case 113:
					loadScreen(FINAL_SCREEN);
					Sleep(1500);
					pos = 0;
					break;
				default:
					continue;
				}
			}
			break;

		case 2:
			//	Desativa o cursor do console.
			setCursorView(0);
			loadScreen(TICKET_MENU);

			/*
			 *	Solicita e recebe uma entrada de usuário e atribui ao ponteiro "aux".
			 *
			 *  @param 52								tamanho máximo da string.
			 *  @param 69								coordenada "x" onde será posicionado o cursor.
			 *  @param 22								coordenada "y" onde será posicionado o cursor.
			 */
			char* aux = enterInput(52, 69, 22);
			ticket_count = atoi(aux);
			pos = 3;
			break;
		case 3:
			//	Desativa o cursor do console.
			setCursorView(0);

			/*
			 *	Verifica qual entrada o usuário escolheu.
			 *
			 *	@param clientSocket						soquete do cliente a ser verificado.
			 *
			 *  @return									Retorna o valor correspondente a entrada escolhida
			 *											caso a verificação seja concluída com êxito.
			 */
			entry = selectEntry(clientSocket);
			if (entry == 2) {
				pos = 5;
				break;
			}
			pos = 4;
			break;
		case 4:
			//	Desativa o cursor do console.
			setCursorView(0);

			/*
			 *	Verifica qual pagamento o usuário escolheu.
			 *
			 *	@param clientSocket						soquete do cliente a ser verificado.
			 *  @param entry							inteiro que representa o tipo de entrada do usuário.
			 *  @param ticket_count						inteiro que representa a quantidade de ingressos comprados pelo usuário.
			 *
			 *  @return	0								caso o pagamento seja verificado com êxito.
			 */
			result = selectPayment(clientSocket, entry, ticket_count);
			pos = 5;
			break;
		case 5:
			loadScreen(THANK_YOU_SCREEN);

			// Executa o programa "receipt_gen" para geração de relatório de venda de ingresso.
			system("start ..\\receipt_gen\\receipt_gen.exe");
			Sleep(5000);
			pos = 0;
			break;
		default:
			continue;
		}
	}
}