#include "server.h"
#include "spdlog/spdlog.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <unistd.h>

// Конструктор класса Server, инициализирующий порт и максимальное количество клиентов
Server::Server(int port, int max_clients) : port_(port), max_clients_(max_clients) {}

// Метод для запуска сервера
void Server::run() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        spdlog::critical("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Привязка сокета к порту
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        spdlog::critical("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        spdlog::critical("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Прослушивание подключений
    if (listen(server_fd, max_clients_) < 0) {
        spdlog::critical("Listen failed");
        exit(EXIT_FAILURE);
    }

    spdlog::info("Server started on port {}", port_);

    while (true) {
        int client_socket;
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            spdlog::error("Accept failed");
            continue;
        }

        spdlog::info("New client connected");

        std::thread(&Server::handle_client, this, client_socket).detach(); // Запуск обработки клиента в отдельном потоке
    }
}

// Метод для обработки подключения клиента
void Server::handle_client(int client_socket) {
    char buffer[1024] = {0};
    int valread;

    while ((valread = read(client_socket, buffer, 1024)) > 0) {
        std::string command(buffer, valread);
        std::string response = process_command(command);

        send(client_socket, response.c_str(), response.size(), 0);
        memset(buffer, 0, sizeof(buffer));
    }

    close(client_socket);
    spdlog::info("Client disconnected");
}

// Метод для обработки команды от клиента
std::string Server::process_command(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd, key, value;
    iss >> cmd;

    if (cmd == "PUT") {
        iss >> key >> value;
        return storage_.put(key, value);
    } else if (cmd == "GET") {
        iss >> key;
        return storage_.get(key);
    } else if (cmd == "DEL") {
        iss >> key;
        return storage_.del(key);
    } else if (cmd == "COUNT") {
        return storage_.count();
    } else if (cmd == "DUMP") {
        iss >> key; // Здесь key используется для хранения имени файла
        return storage_.dump(key);
    } else {
        return "ERROR unknown command\n";
    }
}
