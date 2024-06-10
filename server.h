#ifndef SERVER_H
#define SERVER_H

#include "storage.h"
#include <string>
#include <thread>

class Server {
public:
    Server(int port, int max_clients);
    void run();
    Storage storage_; // Сделать этот член public для доступа в main.cpp

private:
    void handle_client(int client_socket); // Обработка клиента
    std::string process_command(const std::string& command); // Обработка команды

    int port_; // Порт для прослушивания
    int max_clients_; // Максимальное количество клиентов
};

#endif
