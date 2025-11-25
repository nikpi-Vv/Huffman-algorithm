#include "codec.h"
#include "treelist.h"
#include <stdio.h>
// Кодирует входной файл с помощью таблицы кодов и записывает в выходной бинарный
int encode_file(const char *input_filename, const char *output_filename, 
                code_table_t *table, unsigned char *frequencies) {

    FILE *in = fopen(input_filename, "rb");   // rb = read binary
    FILE *out = fopen(output_filename, "wb"); // wb = write binary

    if (!in || !out) {
        fprintf(stderr, "Could not open input or output file.\n");
        if (in) fclose(in);
        if (out) fclose(out);
        return 0;
    }
    
    // --- 1. Сохраняем частоты (для восстановления дерева на декодировании) ---
    fwrite(frequencies, sizeof(unsigned char), 256, out);

    // --- 2. Кодируем данные ---
    unsigned char buffer = 0;
    int bits_filled = 0;
    int bytes_written = 0;

    int c;
    while ((c = fgetc(in)) != EOF) {
        code_t code = table->codes[c];
        // Для каждого бита в коде символа
        for (int i = 0; i < code.length; i++) {
            buffer <<= 1;              // Сдвигаем левее
            buffer |= code.bits[i];    // Добавляем бит
            bits_filled++;
            if (bits_filled == 8) {
                fputc(buffer, out);    // Записали байт!
                bytes_written++;
                buffer = 0;
                bits_filled = 0;
            }
        }
    }

    // --- Если остались биты в буфере (меньше 8), дополняем нулями ---
    if (bits_filled > 0) {
        buffer <<= (8 - bits_filled); // дополнить нулями
        fputc(buffer, out);
        bytes_written++;
    }

    fclose(in);
    fclose(out);
    return bytes_written;
}

// Восстанавливает дерево по таблице частот
struct node_t * restore_tree_from_freqs(unsigned char *freqs) {
    struct node_t *root = list_from_freq_array(freqs);
    return make_tree(root);
}

// Декодирует файл с помощью восстановленного дерева, пишет результат в другой файл
int decode_file(const char *input_filename, const char *output_filename) {
    // 1. Открыть входной файл для чтения "rb"
    FILE *in = fopen(input_filename, "rb");   // rb = read binary
    FILE *out = fopen(output_filename, "wb");

    if (!in || !out) {
        fprintf(stderr, "Could not open input or output file.\n");
        if (in) fclose(in);
        if (out) fclose(out);
        return 0;
    }

    // --- 1. Считываем таблицу частот из файла ---
    unsigned char frequencies[256];
    if (fread(frequencies, 1, 256, in) != 256) {
        fprintf(stderr, "Failed to read frequency table.\n");
        fclose(in); fclose(out);
        return 0;
    }

    // --- 2. Восстанавливаем дерево по таблице частот ---
    struct node_t *tree = restore_tree_from_freqs(frequencies);

    // --- 3. Декодируем файл ---
    struct node_t *node = tree;
    int byte;
    int bits_left = 0;
    unsigned char current_byte = 0;

    while ((byte = fgetc(in)) != EOF) {
        current_byte = (unsigned char)byte;
        bits_left = 8;
        while (bits_left > 0) {
            unsigned char bit = (current_byte & 0x80) ? 1 : 0; // Смотрим старший бит
            current_byte <<= 1; // Сдвигаем на бит
            bits_left--;

            // Движение по дереву: 0 — влево, 1 — вправо
            if (bit == 0 && node->left)
                node = node->left;
            else if (bit == 1 && node->right)
                node = node->right;

            // Если дошли до листа
            if (node->left == NULL && node->right == NULL) {
                fputc(node->symbol, out);
                node = tree; // возвращаемся на вершину
            }
        }
    }

    fclose(in);
    fclose(out);
    return 1;

}