#include "Server.h"

#pragma comment(lib, "ws2_32.lib")

int main() 
{
    // Start the server on port 8080
    startServer(8080);
    return 0;
}