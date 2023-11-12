#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include "Files.h"

//	Carrega a tela de carregamento.
void loadLoadingScreen() {
	system("cls");
	FILE* fp1 = fopen(LOADING_SCREEN, "r");

	char loading_screen[100];

	while (fgets(loading_screen, sizeof(loading_screen), fp1) != NULL) {
		printf(loading_screen);
	}

	fclose(fp1);
	Sleep(500);
	system("cls");
}

/*
 *	Carrega uma tela de acordo com o seu caminho.
 *
 *	@param screen_path									representa o caminho da tela a ser carregada.
*/
void loadScreen(char screen_path[100]) {
	system("cls");
	FILE* fp = fopen(screen_path, "r");

	char buffer[256];

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		printf("%s", buffer);
	}

	fclose(fp);
}

/*
 *	Carrega um arquivo no modo de leitura de acordo com o seu caminho.
 *
 *	@param file_path									representa o caminho do arquivo a ser carregado.
*/
void readFile(char file_path[100]) {
	FILE* fp = fopen(file_path, "r");

	char buffer[256];

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		printf("%s", buffer);
	}

	fclose(fp);
}