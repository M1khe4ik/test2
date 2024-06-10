#include "storage.h"
#include "spdlog/spdlog.h"
#include <fstream>

// Метод для добавления или обновления значения по ключу
std::string Storage::put(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::string old_value;

    if (data_.find(key) != data_.end()) {
        old_value = data_[key];
    }

    data_[key] = value;
    spdlog::info("PUT key: {}, value: {}", key, value);

    return old_value.empty() ? "OK\n" : "OK " + old_value + "\n";
}

// Метод для получения значения по ключу
std::string Storage::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (data_.find(key) != data_.end()) {
        spdlog::info("GET key: {}, value: {}", key, data_[key]);
        return "OK " + data_[key] + "\n";
    } else {
        spdlog::warn("GET key: {} not found", key);
        return "NE\n";
    }
}

// Метод для удаления значения по ключу
std::string Storage::del(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::string value;

    if (data_.find(key) != data_.end()) {
        value = data_[key];
        data_.erase(key);
        spdlog::info("DEL key: {}, value: {}", key, value);
        return "OK " + value + "\n";
    } else {
        spdlog::warn("DEL key: {} not found", key);
        return "NE\n";
    }
}

// Метод для получения количества ключей
std::string Storage::count() {
    std::lock_guard<std::mutex> lock(mutex_);
    int count = data_.size();
    spdlog::info("COUNT keys: {}", count);
    return "OK " + std::to_string(count) + "\n";
}

// Метод для сохранения данных в файл
std::string Storage::dump(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::ofstream ofs(filename, std::ios::out | std::ios::trunc);

    if (!ofs.is_open()) {
        spdlog::error("DUMP to file: {} failed", filename);
        return "ERROR\n";
    }

    for (const auto& [key, value] : data_) {
        ofs << key << " " << value << "\n";
    }

    ofs.close();
    spdlog::info("DUMP to file: {} succeeded", filename);
    return "OK\n";
}

// Метод для загрузки данных из файла
bool Storage::load(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::ifstream ifs(filename);

    if (!ifs.is_open()) {
        spdlog::error("LOAD from file: {} failed", filename);
        return false;
    }

    data_.clear();
    std::string key, value;

    while (ifs >> key >> value) {
        data_[key] = value;
    }

    ifs.close();
    spdlog::info("LOAD from file: {} succeeded", filename);
    return true;
}
