cmake_minimum_required(VERSION 3.10)
project(Huffman C)

# Устанавливаем стандарт C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Флаги компилятора
add_compile_options(-Wall -Wextra -O2)

# Создаём исполняемый файл
add_executable(huffman 
    huffman.c 
    treelist.c
)
