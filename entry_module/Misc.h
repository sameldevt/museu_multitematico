#pragma once
#ifndef MISC
#define MISC

void setDefaultConsoleTextColor();

char* enterInput(int input_size, int cloc_x, int cloc_y);

void setCursorView(int condition);

void setCursorLocation(int cord_x, int cord_y);

void setHorizontalSize();

void setVerticalSize();

/* Fun��o para verificar a entrada do usu�rio */
int keyboardCheck();

/* Fun��o para iniciar o programa */
void startProgram();

/* Fun��o para encerrar o programa */
void endProgram();

#endif