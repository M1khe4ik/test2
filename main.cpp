#include "server.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <iostream>
#include <string>
#include <csignal>
#include <unistd.h>

// Обработчик сигналов для корректного завершения программы при получении сигнала прерывания
void signal_handler(int signal) {
    spdlog::info("Interrupt signal ({}) received. Exiting...", signal);
    std::exit(signal);
}

int main(int argc, char* argv[]) {
    // Проверка наличия необходимых аргументов командной строки
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " --port <port> --max_clients <max_clients> [--load <dump_file>]" << std::endl;
        return 1;
    }

    int port = 0; // Порт для прослушивания
    int max_clients = 0; // Максимальное количество клиентов
    std::string dump_file; // Имя файла для загрузки данных из дампа
    bool load_dump = false; // Флаг загрузки дампа

    // Парсинг аргументов командной строки
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--port" && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        } else if (arg == "--max_clients" && i + 1 < argc) {
            max_clients = std::stoi(argv[++i]);
        } else if (arg == "--load" && i + 1 < argc) {
            dump_file = argv[++i];
            load_dump = true;
        }
    }

    // Инициализация логгера
    auto logger = spdlog::basic_logger_mt("basic_logger", "logs/redis_killer.log");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug); // Установить глобальный уровень логирования на debug

    // Установка обработчиков сигналов для корректного завершения программы
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    // Создание объекта сервера
    Server server(port, max_clients);

    // Загрузка данных из файла дампа, если это указано в аргументах командной строки
    if (load_dump) {
        spdlog::info("Loading dump from file: {}", dump_file);
        if (!server.storage_.load(dump_file)) {
            spdlog::error("Failed to load dump from file: {}", dump_file);
        } else {
            spdlog::info("Successfully loaded dump from file: {}", dump_file);
        }
    }

    // Запуск сервера
    spdlog::info("Starting server on port {} with max {} clients", port, max_clients);
    server.run();

    return 0;
}
