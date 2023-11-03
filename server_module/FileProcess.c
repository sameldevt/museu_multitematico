#include <stdio.h>
#include <stdlib.h>

#include "FileProcess.h"

void readFile(char file_path[100]) {

	FILE* fp = fopen(file_path, "r");

	char buffer[256];

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		printf("%s", buffer);
	}

	fclose(fp);
}

void writeFile(char file_path[100], char input[]) {

	FILE* fp = fopen(file_path, "w");

	fputs(input, fp);

	fclose(fp);
}

void appendToFile(char file_path[100], char input[100]) {
	char input_ref[100];

	FILE* fp = fopen(file_path, "a");

	fprintf(fp, input);

	fclose(fp);
}

void registerUser(char name[100]) {
	char user[100];
	sprintf(user, "%s\n", name);
	appendToFile(CSV_USERS, user);
}
