#include <stdio.h>
#include <stdlib.h>

#include "FileProcess.h"

/*
 * Carrega um arquivo no modo de leitura de acordo com o seu caminho.
 *
 * @param file_path		representa o caminho do arquivo a ser carregado.
 */
void readFile(char file_path[100]) {

	FILE* fp = fopen(file_path, "r");

	char buffer[256];

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		printf("%s", buffer);
	}

	fclose(fp);
}

/*
 * Carrega um arquivo no modo de escrita de acordo com o seu caminho.
 *
 * @param file_path		representa o caminho do arquivo a ser carregado.
 * @param input			mensagem que será registrada no aqruivo.
 */
void writeFile(char file_path[100], char input[]) {

	FILE* fp = fopen(file_path, "w");

	fputs(input, fp);

	fclose(fp);
}

/*
 * Carrega um arquivo no modo de anexo de acordo com o seu caminho.
 *
 * @param file_path		representa o caminho do arquivo a ser carregado.
 * @param input			mensagem que será registrada no aqruivo.
 */
void appendToFile(char file_path[100], char input[100]) {
	char input_ref[100];

	FILE* fp = fopen(file_path, "a");

	fprintf(fp, input);

	fclose(fp);
}

/*
 * Registra um usuário no sistema.
 *
 * @param name		representa o nome do usuário a ser registrado
 */
void registerUser(char name[100]) {
	char user[100];
	sprintf(user, "%s\n", name);
	appendToFile(CSV_USERS, user);
}
