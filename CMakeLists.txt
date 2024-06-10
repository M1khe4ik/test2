cmake_minimum_required(VERSION 3.10) // Требуемая версия CMake
project(redis-killer) // Название проекта

set(CMAKE_CXX_STANDARD 17) // Установка стандарта C++

add_subdirectory(spdlog) // Добавление поддиректории spdlog

// Добавление исполняемого файла и зависимостей
add_executable(redis-killer src/main.cpp src/server.cpp src/storage.cpp)
target_link_libraries(redis-killer PRIVATE spdlog::spdlog) // Линковка с spdlog

enable_testing() // Включение тестирования
add_subdirectory(googletest) // Добавление поддиректории googletest
add_executable(runUnitTests tests/main_test.cpp src/storage.cpp)
target_link_libraries(runUnitTests gtest gtest_main pthread) // Линковка с googletest и pthread
add_test(runUnitTests runUnitTests) // Добавление теста
