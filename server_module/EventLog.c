#include <stdio.h>

/*
 * Registra os evento do servidor em um arquivo .txt
 * 
 * @param log	mensagem do evento a ser registrado.
 */
int logServer(char log[100]) {
	FILE* fp = fopen("server_resources\\server_log.txt", "a");

	fprintf(fp, log);

	fclose(fp);
}