#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

#include "Files.h"
#include "Misc.h"

int verifyUserInfo(SOCKET clientSocket) {
	int iResult;
	char response[8], entry[7] = "entry1";

	while (1) {
		iResult = send(clientSocket, entry, sizeof(entry), 0);
		if (iResult == SOCKET_ERROR) {
			continue;
		}

		loadLoadingScreen();
		loadScreen(MENU_NORMAL_ENTRY);
		char *name = enterInput(52, 69, 22);
		
		iResult = send(clientSocket, name, strlen(name), 0);

		iResult = recv(clientSocket, response, sizeof(response), 0);

		if (strcmp(response, "failed") == 0) {
			continue;
		}

		free(name);
		break;
	}

	setCursorView(0);
	loadScreen(MENU_NORMAL_ENTRY_OK);
	Sleep(1500);
	return 0;
}

int verifyStudentInfo(SOCKET clientSocket) {
	int iResult;
	char info[120], response[8], entry[7] = "entry2";

	loadLoadingScreen();
	loadScreen(MENU_STUDENT_ENTRY);
	char* name = enterInput(52, 69, 22);

	while (1) {
		iResult = send(clientSocket, entry, sizeof(entry), 0);
		if (iResult == SOCKET_ERROR) {
			continue;
		}

		loadScreen(MENU_STUDENT_ENTRY);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
		setCursorLocation(69, 22);
		printf("%s", name);

		char *student_id = enterInput(52, 69, 26);

		sprintf(info, "%s,%s", name, student_id);

		iResult = send(clientSocket, info, sizeof(info), 0);

		iResult = recv(clientSocket, response, 8, 0);

		if (strcmp(response, "failed") == 0) {
			setCursorView(0);
			loadScreen(MENU_STUDENT_ENTRY_FAIL);
			Sleep(1500);
			free(student_id);
			continue;
		}

		free(name);
		free(student_id);
		break;
	}

	setCursorView(0);
	loadScreen(MENU_STUDENT_ENTRY_OK);
	Sleep(1500);
	return 0;
}

int verifyDisabledPersonInfo(SOCKET clientSocket) {
	int iResult;
	char info[120], response[8], entry[7] = "entry3";

	loadLoadingScreen();
	loadScreen(MENU_DISABLED_PERSON_ENTRY);
	char* name = enterInput(52, 69, 22);

	while (1) {
		iResult = send(clientSocket, entry, sizeof(entry), 0);
		if (iResult == SOCKET_ERROR) {
			continue;
		}

		loadScreen(MENU_DISABLED_PERSON_ENTRY);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
		setCursorLocation(69, 22);
		printf("%s", name);

		char* disabled_person_id = enterInput(52, 69, 26);

		sprintf(info, "%s,%s", name, disabled_person_id);

		iResult = send(clientSocket, info, sizeof(info), 0);

		iResult = recv(clientSocket, response, sizeof(response), 0);

		if (strcmp(response, "failed") == 0) {
			setCursorView(0);
			loadScreen(MENU_DISABLED_PERSON_ENTRY_FAIL);
			Sleep(1500);
			free(disabled_person_id);
			continue;
		}

		free(name);
		free(disabled_person_id);
		break;
	}

	setCursorView(0);
	loadScreen(MENU_DISABLED_PERSON_ENTRY_OK);
	Sleep(1500);
	return 0;
}

int verifyJuniorInfo(SOCKET clientSocket) {
	int iResult;
	char response[8], info[110], entry[7] = "entry4";

	loadLoadingScreen();
	loadScreen(MENU_JUNIOR_ENTRY);
	char* name = enterInput(52, 69, 22);

	while (1) {
		iResult = send(clientSocket, entry, sizeof(entry), 0);
		if (iResult == SOCKET_ERROR) {
			continue;
		}

		loadScreen(MENU_JUNIOR_ENTRY);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
		setCursorLocation(69, 22);
		printf("%s", name);
		char* age = enterInput(52, 69, 26);

		sprintf(info, "%s,%s", name, age);

		iResult = send(clientSocket, info, sizeof(info), 0);

		iResult = recv(clientSocket, response, sizeof(response), 0);

		if (strcmp(response, "failed") == 0) {
			setCursorView(0);
			loadScreen(MENU_JUNIOR_ENTRY_FAIL);
			Sleep(1500);
			free(age);
			continue;
		}

		free(age);
		free(name);
		break;
	}

	setCursorView(0);
	loadScreen(MENU_JUNIOR_ENTRY_OK);
	Sleep(1500);
	return 0;
}

int verifySeniorInfo(SOCKET clientSocket) {
	int iResult;
	char info[120], response[8], entry[7] = "entry5";

	loadLoadingScreen();
	loadScreen(MENU_SENIOR_ENTRY);
	char* name = enterInput(52, 69, 22);

	while (1) {
		iResult = send(clientSocket, entry, sizeof(entry), 0);
		if (iResult == SOCKET_ERROR) {
			continue;
		}

		loadScreen(MENU_SENIOR_ENTRY);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
		setCursorLocation(69, 22);
		printf("%s", name);
		char* age = enterInput(52, 69, 26);

		sprintf(info, "%s,%s", name, age);

		iResult = send(clientSocket, info, sizeof(info), 0);

		iResult = recv(clientSocket, response, sizeof(response), 0);

		if (strcmp(response, "failed") == 0) {
			setCursorView(0);
			loadScreen(MENU_SENIOR_ENTRY_FAIL);
			Sleep(1500);
			free(age);
			continue;
		}

		free(age);
		free(name);
		break;
	}

	setCursorView(0);
	loadScreen(MENU_SENIOR_ENTRY_OK);
	Sleep(1500);
	return 0;
}

int selectEntry(SOCKET clientSocket) {
	int key, iResult;

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