#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <cstring>
// This line is very important for Windows
#pragma comment(lib, "ws2_32.lib")
int main() 
{
    // 1. Initialize Winsock (Mandatory for Windows!)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Winsock initialization failed" << std::endl;
        return 1;
    }
    // 1. Create the socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) 
    {
        perror("Socket creation failed");
        return 1;
    }

    // 2. Define the address
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080); // We will use port 8080

    // 3. Bind the socket to the port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) 
    {
        perror("Bind failed");
        return 1;
    }

    // 4. Start listening for "calls"
    if (listen(server_fd, 5) < 0) 
    {
        perror("Listen failed");
        return 1;
    }
    // ... (previous socket, bind, and listen code) ...

    std::cout << "Server is listening on port 8080..." << std::endl;

    // This loop keeps the server running so it doesn't just exit!
    while (true) {
        int addrlen = sizeof(address);
        
        // The program will PAUSE here until you visit localhost:8080
        int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        if (client_socket < 0) {
            perror("Accept failed");
            continue; // Try again if one connection fails
        }

        std::cout << "Connection established! Someone visited the server." << std::endl;

        // Close the connection to this specific client so we can wait for the next one
        closesocket(client_socket); 
    }

    // This part is now technically "unreachable" but good practice
    closesocket(server_fd);
    return 0;
}