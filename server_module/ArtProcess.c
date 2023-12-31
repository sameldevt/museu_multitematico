#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "FileProcess.h"
#include "ArtProcess.h"
#include "Server.h"

char art_name_list[16][50] = {
	"A Negra",
	"Abaporu",
	"Estrada de ferro",
	"Operarios",
	"Alberto Santo Dumont",
	"14-BIS",
	"Dirigivel N� 1",
	"Dirigivel N� 6",
	"Jogos Olimpicos na era moderna - 1896",
	"Jogos Olimpicos de Par�s - 2024",
	"Jogos Olimpicos no Brasil - 2016",
	"Olimpiadas na antiguidade",
	"Ciberataques",
	"Internet das Coisas",
	"Inteligencia Artificial",
	"Protecao de dados e privacidade",
};

struct ArtThreadStruct {
	int num;
	SOCKET clientSocket;
};

/*
 * Registra a avalia��o do usu�rio sobre determinada arte em um arquivo .CSV.
 *
 * @param art_feedback		representa a avalia��o do cliente sobre determinada arte.
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
 * Gerencia as threads criadas na fun��o main.
 * 
 * @param arg	ponteiro para void que representa os argumentos usados na cria��o da thread.
 */
void* artHandler(void *arg) {
	struct ArtThreadStruct *params = (struct ArtThreadStruct*)arg;
	char art_id[10], art_feedback[100], art_info[10];

	sprintf(art_id, "%d", params->num);

	/*	
	 * Envia ao m�dulo de question�rio o ID da arte a ser carregada.
	 *
	 * @param clientSocket		soquete do cliente para enviar o ID da arte.
	 * @param art_id			string que representa o ID da arte.
	 * @param sizeof(art_id)	tamanho da string.
	 * @param 0					flag que modifica o comportamento da fun��o "send()".
	 */	
	send(params->clientSocket, art_id, sizeof(art_id), 0);

	/*	
	 *	Recebe a avalia��o da arte correspondente ao valor de "art_id" e atribui para a vari�vel "art_feedback".
	 * 
	 * @param clientSocket				soquete do cliente para receber a avalia��o do usu�rio.
	 * @param art_feedback				string que representa a avalia��o do usu�rio.
	 * @param sizeof(art_feedback)		tamanho da string.
	 * @param 0							flag que modifica o comportamento da fun��o "recv()".
	 */
	recv(params->clientSocket, art_feedback, sizeof(art_feedback), 0);

	// Registra a avalia��o do usu�rio.
	registerArtFeedback(art_feedback);

	char date[10], path[100];
	time_t rawtime;
	struct tm* timeinfo;
	int theme_id = 0;

	if (params->num > 3 && params->num < 8) {
		theme_id = 1;
	}
	else if (params->num > 7 && params->num < 12) {
		theme_id = 2;
	}
	else if (params->num > 11 && params->num < 15) {
		theme_id = 3;
	}
	
	sprintf(art_info, "%d,%d\n", params->num, theme_id);

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(date, "%02d-%02d", timeinfo->tm_mday, timeinfo->tm_mon + 1);
	sprintf(path, "server_resources\\stats\\daily\\art_stats\\%s.txt", date);

	// Registra as informa��es sobre a arte avaliada no sistema.
	appendToFile(path, art_info);
	appendToFile("server_resources\\stats\\all\\art_stats.txt", art_info);
}