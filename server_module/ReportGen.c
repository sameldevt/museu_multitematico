#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char art_list[16][20] = {
	"A NEGRA",
	"ABAPORU",
	"ESTRADA DE FERRO",
	"OPERARIOS",
	"SANTOS DUMONT",
	"14 BIS",
	"TORRE EIFFEL",
	"DIRIGIVEL",
};

typedef struct {
	int ticket_type[10000];
	int ticket_count;
	float total_sold_value;
} PaymentStats;

typedef struct {
	int visited_theme[10000];
} GateStats;

typedef struct {
	int visited_art[10000];
} ArtStats;

/*
 * Calcula qual a arte mais visitada.
 *
 * @param artStats				struct que representa as informações de artes.
 *
 * @return most_visited_art		o nome da arte mais visitada.
 */
char* calculateMostVisitedArt(ArtStats* artStats) {
	int art_count[16];
	char* most_visited_art = (char*)malloc(50);

	for (int i = 0; i < 16; i++) {
		art_count[i] = 0;
	}

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 10000; j++) {
			if (artStats->visited_art[j] == i) {
				art_count[i]++;
			}
			if (artStats->visited_art[j] == NULL) {
				break;
			}
		}
	}

	int higher = 0;
	for(int i = 0; i < 16; i++){
		if (art_count[i] > higher) {
			higher = i;
		}
	}

	strcpy(most_visited_art, art_list[higher]);

	return most_visited_art;
}

/*
 * Calcula qual o tema mais visitada.
 *
 * @param gateStats					struct que representa as informações dos temas.
 *
 * @return most_visited_theme		o nome do tema mais visitada.
 */
char* calculateMostVisitedTheme(GateStats* gateStats) {
	char* most_visited_theme = (char*)malloc(50);
	int i = 0, theme_1 = 0, theme_2 = 0, theme_3 = 0, theme_4 = 0;

	while (1) {
		switch (gateStats->visited_theme[i]) {
		case 1:
			theme_1++;
			i++;
			continue;
		case 2:
			theme_2++;
			i++;
			continue;
		case 3:
			theme_3++;
			i++;
			continue;
		case 4:
			theme_4++;
			i++;
			continue;
		case NULL:
			break;
		}
		break;
	}

	if (theme_1 > theme_2 && theme_1 > theme_3 && theme_1 > theme_4) {
		strcpy(most_visited_theme, "150 anos de Santos Dumont\0");

	}
	else if (theme_2 > theme_1 && theme_2 > theme_3 && theme_2 > theme_4) {
		strcpy(most_visited_theme, "100 anos da Semana Da Arte Moderna\0");

	}
	else if (theme_3 > theme_1 && theme_3 > theme_2 && theme_3 > theme_4) {
		strcpy(most_visited_theme, "Jogos Olímpicos Paris 2024\0");

	}
	else if (theme_4 > theme_1 && theme_4 > theme_2 && theme_4 > theme_3) {
		strcpy(most_visited_theme, "Segurança Cibernética\0");
	}
	
	return most_visited_theme;
}

/*
 * Calcula qual tipo de ingresso mais vendido.
 *
 * @param paymentStats					struct que representa as informações de pagamento.
 *
 * @return most_purchased_ticket		o tipo de ingresso mais vendido.
 */
char* calculateMostPurchasedTicket(PaymentStats* paymentStats) {
	char* most_purchased_ticket = (char*)malloc(20);
	int i = 0, fullentry = 0, halfentry = 0, freeentry = 0;

	while (1) {
		switch (paymentStats->ticket_type[i]) {
		case 1:
			fullentry++;
			i++;
			continue;
		case 2:
			halfentry++;
			i++;
			continue;
		case 3:
			freeentry++;
			i++;
			continue;
		case NULL:
			break;
		}
		break;
	}

	if (fullentry > halfentry && fullentry > freeentry) {
		strcpy(most_purchased_ticket, "Entrada cheia\0");
	}
	else if (halfentry > fullentry && halfentry > freeentry) {
		strcpy(most_purchased_ticket, "Meia entrada\0");
	}
	else if (freeentry > fullentry && fullentry > halfentry) {
		strcpy(most_purchased_ticket, "Entrada gratuita\0");
	}

	return most_purchased_ticket;
}

/*
 * Carrega as informações de pagamento na memória.
 *
 * @param date				inteiro que representa a data de onde os dados serão carregados, sendo 0
 *							os dados do dia e 1 todos os dados.
 *
 * @return paymentStats		um ponteiro para a struct "PaymentStats".
 */
PaymentStats* getPaymentStats(int date) {
	PaymentStats* paymentStats = (PaymentStats*)malloc(sizeof(PaymentStats));
	int* token, * tokens[3], count = 0, i = 0, ticket_type = 0, ticket_count = 0, ticket_value = 0;
	char path[100];
	
	if (date == 0) {
		strcpy(path, "server_resources\\stats\\daily\\payment_stats.txt");
	}
	else {
		strcpy(path, "server_resources\\stats\\all\\payment_stats.txt");
	}

	FILE* fp = fopen(path, "r");
	
	char line[30];
	while (fgets(line, sizeof(line), fp) != NULL) {
		token = strtok(line, ",");
		while (token != NULL && count < 3) {
			tokens[count] = token;
			count++;
			token = strtok(NULL, ",");
		}

		ticket_type = atoi(tokens[0]);
		ticket_count = atoi(tokens[1]);
		ticket_value = atoi(tokens[2]);

		paymentStats->ticket_type[i] = ticket_type;
		paymentStats->ticket_count += ticket_count;
		paymentStats->total_sold_value += ticket_value;
		
		i++;
	}

	return paymentStats;
}

/*
 * Carrega as informações de arte na memória.
 *
 * @param date				inteiro que representa a data de onde os dados serão carregados, sendo 0
 *							os dados do dia e 1 todos os dados.
 *
 * @return paymentStats		um ponteiro para a struct "ArtStats".
 */
ArtStats* getArtStats(int date) {
	ArtStats* artStats = (ArtStats*)malloc(sizeof(artStats));
	int i = 0;
	char path[100];
	
	if (date == 0) {
		strcpy(path, "server_resources\\stats\\daily\\art_stats.txt");
	}
	else {
		strcpy(path, "server_resources\\stats\\all\\art_stats.txt");
	}

	FILE* fp = fopen(path, "r");

	char line[30];
	while (fgets(line, sizeof(line), fp) != NULL) {
		artStats->visited_art[i] = atoi(line);
		i++;
	}

	return getArtStats;
}

/*
 * Carrega as informações do terminal de catraca na memória.
 *
 * @param date				inteiro que representa a data de onde os dados serão carregados, sendo 0
 *							os dados do dia e 1 todos os dados.
 *
 * @return paymentStats		um ponteiro para a struct "GateStats".
 */
GateStats* getGateStats(int date) {
	GateStats* gateStats = (GateStats*)malloc(sizeof(GateStats));
	int* token, * tokens[3], count = 0, i = 0;
	char path[100];

	if (date == 0) {
		strcpy(path, "server_resources\\stats\\daily\\gate_stats.txt");
	}
	else {
		strcpy(path, "server_resources\\stats\\all\\gate_stats.txt");
	}

	FILE* fp = fopen(path, "r");

	char line[30];
	while (fgets(line, sizeof(line), fp) != NULL) {
		gateStats->visited_theme[i] = atoi(line);
		i++;
	}

	return gateStats;
}

// Gera o relatório diário do museu.
void generateDailyReport() {
	char date[30];
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	PaymentStats *paymentStats = getPaymentStats(0);
	GateStats *gateStats = getGateStats(0);
	ArtStats *artStats = getArtStats(0);

	sprintf(date, "[%02d-%02d-%04d %02d:%02d:%02d]\n",
		timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900,
		timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	system("cls");
	printf("RELATÓRIO DIÁRIO\n\n");
	printf("Data e hora do relatorio: %s\n", date);
	printf("Quantidade de ingressos vendidos: %d\n", paymentStats->ticket_count);
	printf("Valor faturado: R$%.2f\n", paymentStats->total_sold_value);
	printf("Ingresso mais comprado: %s\n", calculateMostPurchasedTicket(&paymentStats));
	printf("Arte mais visitada: %s\n", calculateMostVisitedArt(&artStats));
	printf("Tema mais visitado: %s\n", calculateMostVisitedTheme(&gateStats));
}

// Gera o relatório completo do museu.
void generateCompleteReport() {
	char date[30];
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	PaymentStats* paymentStats = getPaymentStats(1);
	GateStats* gateStats = getGateStats(1);
	ArtStats* artStats = getArtStats(1);

	sprintf(date, "[%02d-%02d-%04d %02d:%02d:%02d]\n",
		timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900,
		timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	system("cls");
	printf("RELATÓRIO COMPLETO\n\n");
	printf("Data e hora do relatorio: %s\n", date);
	printf("Quantidade de ingressos vendidos: %d\n", paymentStats->ticket_count);
	printf("Valor faturado: R$%.2f\n", paymentStats->total_sold_value);
	printf("Ingresso mais vendido: %s\n", calculateMostPurchasedTicket(&paymentStats));
	printf("Arte mais visitada: %s\n", calculateMostVisitedArt(&artStats));
	printf("Tema mais visitado: %s\n", calculateMostVisitedTheme(&gateStats));
}

// Imprime o log do servidor.
int printServerLog() {
	FILE* fp = fopen("server_resources\\server_log.txt", "r");

	char buffer[100];

	system("cls");
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		printf("%s", buffer);
	}

	fclose(fp);
}