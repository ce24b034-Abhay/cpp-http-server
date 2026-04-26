# Multithreaded C++ HTTP Server 🌐

A lightweight, multithreaded web server built from scratch in C++ to understand the underlying mechanics of HTTP, POSIX sockets, and concurrent programming.

## 🚀 Features

* **Custom Socket Implementation:** Built entirely using the Windows Socket API (<winsock2.h>) without relying on heavy external web frameworks.
* **Multithreaded Architecture:** Utilizes `<thread>` to handle multiple concurrent client connections asynchronously without blocking.
* **HTTP Protocol Parsing:** Capable of reading and parsing raw HTTP/1.1 `GET` requests.
* **Static File Serving:** Reads and serves HTML files and plain text from the local filesystem.
* **Robust Error Handling:** Includes standard HTTP responses, such as `404 Not Found` for invalid routes.

## 🛠️ Prerequisites

To build and run this project, you will need:
* A C++11 (or higher) compatible compiler (e.g., `g++` or `clang++`)
* A Windows environment (uses Winsock2 for networking)

## 💻 How to Build & Run

1. Clone the repository:
   ```bash
   git clone https://github.com/ce24b034-Abhay/cpp-http-server.git
   cd cpp-http-server