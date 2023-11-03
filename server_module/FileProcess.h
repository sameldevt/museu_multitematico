#pragma once
#ifndef FILE_PROCESS
#define FILE_PROCESS

#define FEEDBACK_DB "server_resources\\art_feedback\\art_feedback.txt"
#define TICKET_DB "..\\receipt_gen\\receipt_gen_resources\\ticket_db\\ticket_list.txt"
#define CSV_USERS "server_resources\\csv_files\\register\\users.csv"
#define CSV_USER_FEEDBACK "server_resources\\csv_files\\register\\user_feedback.csv"
#define CSV_SURVEY_SUMARY "server_resources\\csv_files\\register\\survey_sumary.csv"

/* Função para abrir e ler um arquivo a partir do caminho informado */
void readFile(char file_path[100]);

/* Função para abrir e reescrever um arquivo a partir do caminho informado */
void writeFile(char file_path[100], char input[]);

/* Função para acrescentar uma linha ao arquivo informado */
void appendToFile(char file_path[100], char input[]);

void registerUser(char name[100]);

#endif
