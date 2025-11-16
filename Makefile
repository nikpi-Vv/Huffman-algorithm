# Компилятор
CC = gcc
# Флаги компиляции
CFLAGS = -Wall -Wextra -std=c99 -O2
# Целевой файл
TARGET = huffman
# Исходники
SOURCES = huffman.c treelist.c
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
	rm -f $(OBJECTS) $(TARGET) $(TARGET).exe

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
