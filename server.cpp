#include "Server.h"
#include "HttpParser.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <process.h>
// Helper function to serve files
void serveFile(SOCKET client_socket, const std::string& filepath, const std::string& status_code) 
{
    std::ifstream file(filepath);
    if (file.is_open()) 
    {
        std::stringstream file_buffer;
        file_buffer << file.rdbuf();
        std::string body = file_buffer.str();
        std::string response = "HTTP/1.1 " + status_code + "\r\n";
        response += "Content-Type: text/html; charset=utf-8\r\n";
        response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
        response += "Connection: close\r\n\r\n";
        response += body;
        send(client_socket, response.c_str(), response.length(), 0);
    }
}
unsigned __stdcall handleConnection(void* lpParam) 
{
    SOCKET client_socket = *(SOCKET*)lpParam;
    delete (SOCKET*)lpParam; // Clean up the pointer
    char buffer[2048] = {0};
    int bytesReceived = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) 
    {
        std::string request_str(buffer);
        std::string method, route;
        // Use our new parser module
        parseHttpRequest(request_str, method, route);
        std::cout << "Method: " << method << " | Route: " << route << "\n";
        // Route Handling
        if (route == "/" || route == "/index.html") 
        {
            serveFile(client_socket, "index.html", "200 OK");
        } 
        else 
        {
            // Serve the new 404 page
            serveFile(client_socket, "404.html", "404 Not Found");
        }
    }
    std::cout << "Thread task complete. Closing connection.\n";
    closesocket(client_socket);
    return 0;
}
void startServer(int port) 
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);
    std::cout << "Multithreaded Server listening on port " << port << "...\n";
    while (true) 
    {
        int addrlen = sizeof(address);
        SOCKET client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_socket != INVALID_SOCKET) 
        {
            std::cout << "Connection accepted! Creating worker thread...\n";
            SOCKET* pClient = new SOCKET;
            *pClient = client_socket;
            _beginthreadex(NULL, 0, handleConnection, pClient, 0, NULL);
        }
    }
    closesocket(server_fd);
    WSACleanup();
}