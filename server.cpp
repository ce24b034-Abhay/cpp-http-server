#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <cstring>
#include <sstream> 
#include <fstream>
#include <process.h> // Required for _beginthreadex

#pragma comment(lib, "ws2_32.lib")

// ==========================================
// DAY 5: WINDOWS NATIVE CONNECTION HANDLER
// ==========================================
unsigned __stdcall handleConnection(void* lpParam) {
    // 1. Get the socket from the pointer and then delete the pointer
    SOCKET client_socket = *(SOCKET*)lpParam;
    delete (SOCKET*)lpParam; 

    char buffer[2048] = {0};
    int bytesReceived = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

    if (bytesReceived > 0) {
        std::cout << "\n--- New Thread Handling Request ---\n";
        
        // Parse Method and Route
        std::string request_str(buffer);
        std::istringstream iss(request_str);
        std::string method, route, http_version;
        iss >> method >> route >> http_version;

        std::cout << "Method: " << method << " | Route: " << route << "\n";

        // Route Handling (Serving index.html)
        if (route == "/" || route == "/index.html") {
            std::ifstream file("index.html");
            if (file.is_open()) {
                std::stringstream file_buffer;
                file_buffer << file.rdbuf();
                std::string body = file_buffer.str();

                std::string response = "HTTP/1.1 200 OK\r\n";
                response += "Content-Type: text/html; charset=utf-8\r\n";
                response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
                response += "Connection: close\r\n\r\n"; 
                response += body;

                send(client_socket, response.c_str(), response.length(), 0);
            } else {
                std::string not_found = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\n404 Not Found";
                send(client_socket, not_found.c_str(), not_found.length(), 0);
            }
        }
    }

    // 2. Clean up and close the thread
    std::cout << "Thread task complete. Closing connection.\n";
    closesocket(client_socket);
    return 0;
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    std::cout << "Multithreaded Server listening on port 8080...\n";

    while (true) {
        int addrlen = sizeof(address);
        SOCKET client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);

        if (client_socket != INVALID_SOCKET) {
            std::cout << "Connection accepted! Creating worker thread...\n";

            // 3. Allocate memory for the socket and pass it to the thread
            // We use 'new' so the socket persists until the thread reads it.
            SOCKET* pClient = new SOCKET;
            *pClient = client_socket;

            _beginthreadex(NULL, 0, handleConnection, pClient, 0, NULL);
        }
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}