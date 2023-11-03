#pragma once
#ifndef GATE_PROCESS
#define GATE_PROCESS

#include "Server.h"

int verifyTicketId(char recv_ticket_id[8]);

void* gateHandler(void * arg);

#endif 