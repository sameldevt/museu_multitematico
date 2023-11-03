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

	while (1) {
		result = connectSocket(clientSocket);
		if (result != 0) {
			perror("Error connecting socket.\n");
			perror("Trying again...\n");
			Sleep(3000);
			continue;
		}
		break;
	}

	setHorizontalSize();

	while (1) {
		switch (pos) {
		case 0:
			setCursorView(0);
			if (_kbhit() == NULL) {
				loadScreen(WELCOME_SCREEN);
				if (key = _getch()) {
					pos = 1;
					break;
				}
			}
		case 1:
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
			setCursorView(0);
			loadScreen(TICKET_MENU);
			char* aux = enterInput(53, 69, 22);
			ticket_count = atoi(aux);
			pos = 3;
			break;
		case 3:
			setCursorView(0);
			entry = selectEntry(clientSocket);
			if (entry == 2) {
				pos = 5;
				break;
			}
			pos = 4;
			break;
		case 4:
			setCursorView(0);
			result = selectPayment(clientSocket, entry, ticket_count);
			if (result != 0) {
				printf("Error.\n");
				exit(1);
			}
			pos = 5;
			break;
		case 5:
			loadScreen(THANK_YOU_SCREEN);
			system("start ..\\receipt_gen\\receipt_gen.exe");
			Sleep(5000);
			pos = 0;
			break;
		default:
			continue;
		}
	}
}