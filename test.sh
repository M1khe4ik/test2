#!/bin/bash
mkdir -p build // Создание директории build, если она не существует
cd build // Переход в директорию build
cmake .. // Генерация файлов сборки с помощью CMake
make // Компиляция проекта
ctest --output-on-failure // Запуск тестов с выводом в случае ошибки
