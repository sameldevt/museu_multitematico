#pragma once
#ifndef ENTRY
#define ENTRY

int verifyUserInfo(SOCKET clientSocket);

int verifyStudentInfo(SOCKET clientSocket);

int verifyDisabledPersonInfo(SOCKET clientSocket);

int verifyJuniorInfo(SOCKET clientSocket);

int verifySeniorInfo(SOCKET clientSocket);

int setCustomTicketCount();

int selectEntry(SOCKET clientSocket);

#endif