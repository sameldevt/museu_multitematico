#pragma once
#ifndef ENTRY_PROCESS
#define ENTRY_PROCESS

#include "Server.h"

#define MENU_BUY_TICKETS "ansi\\buyticket.ans"

#define MENU_FULLPRICE "ansi\\normalentry.ans"
#define MENU_FULLPRICE_1 "ansi\\normalentry1.ans"

#define MENU_FREE_ENTRY "ansi\\freeentry.ans"
#define MENU_JUNIOR_ENTRY "ansi\\freeentryjunior.ans"
#define MENU_JUNIOR_ENTRY_1 "ansi\\freeentryjunior1.ans"
#define MENU_JUNIOR_ENTRY_2 "ansi\\freeentryjunior2.ans"
#define MENU_JUNIOR_ENTRY_2_FAIL "ansi\\freeentryjunior2fail.ans"
#define MENU_SENIOR_ENTRY "ansi\\freeentrysenior.ans"
#define MENU_SENIOR_ENTRY_1 "ansi\\freeentrysenior1.ans"
#define MENU_SENIOR_ENTRY_2 "ansi\\freeentrysenior2.ans"
#define MENU_SENIOR_ENTRY_2_FAIL "ansi\\freeentrysenior2fail.ans"

#define MENU_HALFPRICE_ENTRY "ansi\\halfentry.ans"
#define MENU_DISABLED_PERSON_ENTRY "ansi\\halfentrydisabledperson.ans"
#define MENU_DISABLED_PERSON_ENTRY_1 "ansi\\halfentrydisabledperson1.ans"
#define MENU_DISABLED_PERSON_ENTRY_2 "ansi\\halfentrydisabledperson2.ans"
#define MENU_DISABLED_PERSON_ENTRY_2_FAIL "ansi\\halfentrydisabledperson2fail.ans"
#define MENU_STUDENT_ENTRY "ansi\\halfentrystudent.ans"
#define MENU_STUDENT_ENTRY_1 "ansi\\halfentrystudent1.ans"
#define MENU_STUDENT_ENTRY_2 "ansi\\halfentrystudent2.ans"
#define MENU_STUDENT_ENTRY_2_FAIL "ansi\\halfentrystudent2fail.ans"

/* Função para verificar a entrada de uma pessoa sem necessidades especiais.*/

int verifyUserEntry(SOCKET clientSocket);

/* Função para verificar o documento do estudante */
int verifyStudentEntry(SOCKET clientSocket);

/* Função para verificar a idade do idoso */
int verifySeniorEntry(SOCKET clientSocket);

/* Função para verificar a idade da criança */
int verifyJuniorEntry(SOCKET clientSocket);

/* Função para verificar o documento da pessoa com deficiência */
int verifyDisabledPersonEntry(SOCKET clientSocket);

int entryHandler(SOCKET clientSocket);
#endif