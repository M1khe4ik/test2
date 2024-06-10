#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <unordered_map>
#include <mutex>

class Storage {
public:
    std::string put(const std::string& key, const std::string& value); // Метод для добавления или обновления значения по ключу
    std::string get(const std::string& key); // Метод для получения значения по ключу
    std::string del(const std::string& key); // Метод для удаления значения по ключу
    std::string count(); // Метод для получения количества ключей
    std::string dump(const std::string& filename); // Метод для сохранения данных в файл
    bool load(const std::string& filename); // Метод для загрузки данных из файла

private:
    std::unordered_map<std::string, std::string> data_; // Контейнер для хранения данных
    std::mutex mutex_; // Мьютекс для синхронизации доступа к данным
};

#endif
