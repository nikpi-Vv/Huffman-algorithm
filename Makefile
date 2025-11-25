# Компилятор
CC = gcc
# Флаги компиляции
CFLAGS = -Wall -Wextra -std=c99 -O2
# Целевой файл
TARGET = hahman
# Исходники
SOURCES = hahman.c treelist.c codec.c
# Объектные файлы
OBJECTS = $(SOURCES:.c=.o)

# Основная цель
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Правило для .o файлов
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка
clean:
	del /Q hahman.o treelist.o codec.o hahman.exe
	
# Запуск программы
run: $(TARGET)
	./$(TARGET)

# Помощь
help:
	@echo "Доступные команды:"
	@echo "  make        - компилирует программу"
	@echo "  make run    - компилирует и запускает"
	@echo "  make clean  - удаляет скомпилированные файлы"

.PHONY: clean run help
