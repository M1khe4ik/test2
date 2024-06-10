#!/bin/bash
mkdir -p build // Создание директории build, если она не существует
cd build // Переход в директорию build
cmake .. // Генерация файлов сборки с помощью CMake
make // Компиляция проекта
./redis-killer --port 8080 --max_clients 10 --load dump_file.txt // Запуск программы с указанными аргументами
