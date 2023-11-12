#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "FileProcess.h"
#include "ArtProcess.h"
#include "Server.h"

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
 * Registra a avaliação do usuário sobre determinada arte em um arquivo .CSV.
 *
 * @param art_feedback		representa a avaliação do cliente sobre determinada arte.
 *
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

	// Loop que atribui o nome da arte correspondente ao ID em "art_id".
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
 * Gerencia as threads criadas na função main.
 * 
 * @param arg	ponteiro para void que representa os argumentos usados na criação da thread.
 */
void* artHandler(void *arg) {
	struct ArtThreadStruct *params = (struct ArtThreadStruct*)arg;
	char art_id[10], art_feedback[100];

	sprintf(art_id, "%d", params->num);

	/*	
	 * Envia ao módulo de questionário o ID da arte a ser carregada.
	 *
	 * @param clientSocket		soquete do cliente para enviar o ID da arte.
	 * @param art_id			string que representa o ID da arte.
	 * @param sizeof(art_id)	tamanho da string.
	 * @param 0					flag que modifica o comportamento da função "send()".
	 */	
	send(params->clientSocket, art_id, sizeof(art_id), 0);

	/*	
	 *	Recebe a avaliação da arte correspondente ao valor de "art_id" e atribui para a variável "art_feedback".
	 * 
	 * @param clientSocket				soquete do cliente para receber a avaliação do usuário.
	 * @param art_feedback				string que representa a avaliação do usuário.
	 * @param sizeof(art_feedback)		tamanho da string.
	 * @param 0							flag que modifica o comportamento da função "recv()".
	 */
	recv(params->clientSocket, art_feedback, sizeof(art_feedback), 0);

	// Registra a avaliação do usuário.
	registerArtFeedback(art_feedback);

	// Registra as informações sobre a arte avaliada no sistema.
	appendToFile("server_resources\\stats\\daily\\art_stats.txt", art_id);
	appendToFile("server_resources\\stats\\all\\art_stats.txt", art_id);
}