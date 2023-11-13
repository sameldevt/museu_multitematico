#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

// Ajusta o tamanho e centraliza a janela de console.
int setWindowSize() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	if (console != INVALID_HANDLE_VALUE) {

		// Define o tamanho do buffer de tela
		COORD bufferSize;
		bufferSize.X = 200;
		bufferSize.Y = 200;

		SetConsoleScreenBufferSize(console, bufferSize);

		// Define o tamanho da janela do console
		SMALL_RECT windowSize;
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = 50;  
		windowSize.Bottom = 46; 

		SetConsoleWindowInfo(console, TRUE, &windowSize);
	}

	// Obtém um identificador para o console atual
	HWND consoleWindow = GetConsoleWindow();

	if (consoleWindow != NULL) {

		// Obtém informações sobre a tela
		RECT consoleRect;
		GetWindowRect(consoleWindow, &consoleRect);

		// Obtém as dimensões da tela
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Calcula as coordenadas para centralizar o console
		int consoleWidth = consoleRect.right - consoleRect.left;
		int consoleHeight = consoleRect.bottom - consoleRect.top;
		int consoleX = (screenWidth - consoleWidth) / 2;
		int consoleY = (screenHeight - consoleHeight) / 2;

		// Move o console para as coordenadas calculadas
		MoveWindow(consoleWindow, consoleX, consoleY, consoleWidth, consoleHeight, TRUE);
	}
}

int main() {
	char ticket_id[10], ticket[100];
	char item_code[4] = "001", product_code[10] = "123.456.7", item_desc[18] = "INGRESSO MPB 2023";
	float item_value = 5.00;
	char ticketpath[100];
	int count = 0;
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	srand(time(NULL));

	system("vsdevcmd.bat");
	system("cls");

	// Ajusta o tamanho e centraliza a janela de console.
	setWindowSize();
	
	FILE * fp = fopen("..\\server_module\\server_resources\\ticket_info_temp.csv", "r");

	char buf[100], * tokens[6], * token;

	if (fp == NULL) {
		perror("Error");
	}

	fgets(buf, sizeof(buf), fp);

	fclose(fp);
	fp = NULL;

	token = strtok(buf, ",");

	while (token != NULL && count < 6) {
		tokens[count] = token;
		count++;
		token = strtok(NULL, ",");
	}

	char ticket_type[100];
	strcpy(ticket_type, tokens[0]);
	int item_qtt = atoi(tokens[1]);
	float paid_amount = atof(tokens[2]);
	float discount = atof(tokens[3]);
	char paymentbuf[100];
	strcpy(paymentbuf, tokens[4]);

	int rand_number = rand() % 9000 + 1000;

	sprintf(ticket_id, "MPB%04d", rand_number);

	char item[100];
	sprintf(item, "%s  %s      %s    %7.2f\0", item_code, product_code, item_desc, item_value);

	char time1[50];
	sprintf(time1, "%02d/%02d/%04d  %02d:%02d:%02d\0", timeinfo->tm_mday, timeinfo->tm_mon + 1, 
		timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	printf("										     	 \n");
	printf("             MUSEU POPULAR BRASILEIRO\n");
	printf("           AV. PAULISTA, 900, - BELA VISTA\n");
	printf("          CEP: 01311-000 - SAO PAULO - SP\n");
	printf("            CNPJ:12.345.678 / 0001 - 55\n");
	printf("                                                 \n");
	printf("-------------------------------------------------\n");
	printf(" %s               COD:123456789\n", time1);
	printf("                    CUPOM FISCAL\n");
	printf(" ITEM CODIGO          DESCRICAO                 \n");
	printf(" QTD UN. VL UNIT( R$)   ST           VL ITEM( R$)\n");
	printf("-------------------------------------------------\n");
	printf(" %s                                           \n", item);
	printf(" %4d UN X 5, 00 %32.2f \n", item_qtt, (item_qtt * item_value));
	printf(" Descontos R$  %34.2f\n", -(item_qtt * item_value) * (discount / 100));
	printf("                                 ----------------\n");
	printf(" TOTAL R$%40.2f\n", (item_qtt * item_value) - (item_qtt * item_value) * (discount / 100));
	printf(" %s%42.2f\n", paymentbuf, paid_amount);
	printf("-------------------------------------------------\n");
	printf("                  TIPO DE INGRESSO\n");
	printf("                %s                         \n", ticket_type);
	printf("										     	 \n");
	printf("                   ID DO INGRESSO\n");
	printf("                      %s\n", ticket_id);
	printf("										     	 \n");

	fp = fopen("..\\receipt_gen\\receipt_gen_resources\\qrcode.ans", "r");

	char buf1[100];

	if (fp == NULL) {
		perror("Error");
		_getch();
	}

	while (fgets(buf1, sizeof(buf1), fp) != NULL) {
		printf("%s", buf1);
	}

	fclose(fp);
	fp = NULL;

	count = 0;

	sprintf(ticketpath, "..\\receipt_gen\\receipt_gen_resources\\ticket_db\\%s.txt", ticket_id);

	fp = fopen(ticketpath, "w");

	while (count != item_qtt) {
		fputs("01", fp);
		fputs("\n", fp);
		count++;
	}

	fclose(fp);
	fp = NULL;

	fp = fopen("..\\receipt_gen\\receipt_gen_resources\\ticket_db\\ticket_list.txt", "a");

	char ticket_id_list[10];
	sprintf(ticket_id_list, "%s\n", ticket_id);
	fprintf(fp, ticket_id_list);

	fclose(fp);

	_getch();
	return 0;
}