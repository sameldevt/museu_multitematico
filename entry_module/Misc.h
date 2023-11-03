#pragma once
#ifndef MISC
#define MISC

void setDefaultConsoleTextColor();

char* enterInput(int input_size, int cloc_x, int cloc_y);

void setCursorView(int condition);

void setCursorLocation(int cord_x, int cord_y);

void setHorizontalSize();

void setVerticalSize();

/* Função para verificar a entrada do usuário */
int keyboardCheck();

/* Função para iniciar o programa */
void startProgram();

/* Função para encerrar o programa */
void endProgram();

#endif