#include <stdio.h>

int logServer(char log[100]) {
	FILE* fp = fopen("server_resources\\server_log.txt", "a");

	fprintf(fp, log);

	fclose(fp);
}