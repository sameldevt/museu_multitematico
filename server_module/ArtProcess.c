#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "FileProcess.h"
#include "ArtProcess.h"
#include "Server.h"

/* Arquivo com fun��es que envolvem todo o sistema de artes */

char art_name_list[16][20] = { 
	"A NEGRA",
	"ABAPORU",
	"ESTRADA DE FERRO",
	"OPERARIOS",
	"SANTOS DUMONT",
	"14 BIS",
	"TORRE EIFFEL",
	"DIRIGIVEL",
	};

struct ArtThreadStruct {
	int num;
	SOCKET clientSocket;
};

/*
 *	A fun��o abaixo possui uma string como par�metro, representando a avalia��o
 *	do cliente sobre determinada arte. Essa fun��o � repons�vel por registrar a 
 *	avalia��o em um arquivo .CSV.
 */
void registerArtFeedback(char art_feedback[100]) {
	char feedback_info[100], * token, * tokens[4], art_name[20];
	int count = 0;
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	token = strtok(art_feedback, ",");

	while (token != NULL && count < 5) {
		tokens[count] = token;
		count++;
		token = strtok(NULL, ",");
	}

	int art_id = atoi(tokens[0]);

	// Loop que verif�ca qual o id da arte avaliada.
	for (int i = 0; i < 16; i++) {
		if (art_id == art_name_list[i]) {
			strcpy(art_name, art_name_list[i]);
			break;
		}
	}
	float feedback_avg = atoi(tokens[1]) / 5;

	sprintf(feedback_info, "%s,%.1f,%04d-%02d-%02d,%02d:%02d:%02d\n",
		art_name, feedback_avg, timeinfo->tm_year + 1900, timeinfo->tm_mon + 1,
		timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	FILE *fp = fopen(FEEDBACK_DB, "a");

	fprintf(fp, feedback_info);

	fclose(fp);
}

/*
 *	A fun��o abaixo recebe como par�metro um ponteiro para void, que representa a struct
 *	"ArtThreadStruct" que foi passada como par�metro na fun��o "pthread_create()" chamada
 *	no arquivo "Main.c".
 */
void* artHandler(void *arg) {
	struct ArtThreadStruct *params = (struct ArtThreadStruct*)arg;
	char num[10], art_feedback[100];

	sprintf(num, "%d", params->num);

	/*	
	 *	Envia o valor da vari�vel "num" para o cliente conectado.Essa vari�vel representa
	 *	o "ID" de uma arte do museu.
	 */
	send(params->clientSocket, num, sizeof(num), 0);

	/*	
	 *	Recebe a avalia��o da arte correspondente ao valor de "num" e atribui para a vari�vel
	 *  "art_feedback".
	 */
	recv(params->clientSocket, art_feedback, sizeof(art_feedback), 0);

	registerArtFeedback(art_feedback);

	// Registra as informa��es sobre a arte avaliada no sistema.
	appendToFile("server_resources\\stats\\daily\\art_stats.txt", num);
	appendToFile("server_resources\\stats\\all\\art_stats.txt", num);
}