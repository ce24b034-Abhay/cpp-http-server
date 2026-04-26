#pragma once
#include <winsock2.h>
// Function declarations ONLY
unsigned __stdcall handleConnection(void* lpParam);
void startServer(int port);