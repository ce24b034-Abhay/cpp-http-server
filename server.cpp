#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <cstring>
#include <sstream> // Add this for std::istringstream
#include <fstream>
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
    // This loop keeps the server running so it doesn't just exit!
    while (true) {
        int addrlen = sizeof(address);
        
        // Use SOCKET instead of int for Windows
        SOCKET client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);

        // Check against INVALID_SOCKET for Windows
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            continue; // Try again if one connection fails
        }

        std::cout << "Connection established! Someone visited the server." << std::endl;

        // ==========================================
        // DAY 3: READ AND PARSE THE HTTP REQUEST
        // ==========================================
        
        // 1. Create a buffer to hold the incoming text
        char buffer[2048] = {0}; 
        
        // 2. Read the data using recv()
        int bytesReceived = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            
            // 1. Convert the raw character buffer into a C++ string
            std::string request(buffer);
            
            // 2. Use a string stream to easily extract words separated by spaces
            std::istringstream iss(request);
            
            // 3. Declare the variables the compiler was complaining about!
            std::string method;
            std::string route;
            std::string http_version;
            
            // 4. Extract the first three words (e.g., "GET", "/", "HTTP/1.1")
            iss >> method >> route >> http_version;

            // Now the compiler knows what these are!
            std::cout << "Parsed Method: [" << method << "]\n";
            std::cout << "Parsed Route:  [" << route << "]\n\n";

            // ==========================================
            // DAY 4: SEND THE HTTP RESPONSE
            // ==========================================
            
            // ... (keep all your existing if (route == "/") code down here) ...

            // ==========================================
            // DAY 4: SEND THE HTTP RESPONSE
            // ==========================================

            // Simple routing: Check if the browser wants the root page
            if (route == "/" || route == "/index.html") {
                
                // 1. Open the HTML file
                std::ifstream file("index.html");
                
                if (file.is_open()) {
                    // 2. Read the file into a string
                    std::stringstream file_buffer;
                    file_buffer << file.rdbuf(); 
                    std::string body = file_buffer.str();

                    // 3. Construct the HTTP response headers + body
                    std::string response = "HTTP/1.1 200 OK\r\n";
                    response += "Content-Type: text/html; charset=utf-8\r\n";
                    response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
                    response += "Connection: close\r\n";
                    response += "\r\n"; // Crucial blank line separating headers from body
                    response += body;

                    // 4. Send the response over the socket
                    send(client_socket, response.c_str(), response.length(), 0);
                    std::cout << "Successfully sent index.html to client!\n";
                } else {
                    // Fallback if index.html is missing
                    std::string not_found = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\n404 Not Found";
                    send(client_socket, not_found.c_str(), not_found.length(), 0);
                    std::cout << "File not found, sent 404.\n";
                }
            } 
            // ==========================================
            
        }  else if (bytesReceived == 0) {
            std::cout << "Client closed the connection.\n";
        } else {
            std::cerr << "recv failed with error: " << WSAGetLastError() << std::endl;
        }
        
        // ==========================================

        // Close the connection to this specific client so we can wait for the next one
        closesocket(client_socket); 
    }

    // This part is now technically "unreachable" but good practice
    closesocket(server_fd);
    return 0;
}