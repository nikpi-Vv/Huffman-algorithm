// huffman.c
#include <stdio.h>
#include <string.h>
#include "treelist.h"
#include "codec.h"
#include "utils.h"

// Вспомогательная функция: режим encode
static int run_encode(const char *input_name,
                      const char *compressed_name,
                      const char *decoded_name) {
    // 1. Подсчёт частот символов в input_name
    unsigned char frequencies[256] = {0};
    long input_len = 0;
    FILE *in = fopen(input_name, "rb");
    if (!in) {
        printf("Cannot open input file: %s\n", input_name);
        return 1;
    }
    int ch;
    while ((ch = fgetc(in)) != EOF) {
        frequencies[(unsigned char)ch]++;
        input_len++;
    }
    fclose(in);

    // 2. Строим список, дерево, таблицу кодов
    struct node_t *list = list_from_freq_array(frequencies);
    if (!list) {
        printf("No symbols in input file.\n");
        return 1;
    }
    struct node_t *tree = make_tree(list);

    code_table_t table;
    make_code_table(tree, &table);

    // 3. Кодируем в compressed_name
    int written = encode_file(input_name, compressed_name, &table, frequencies, input_len);
    if (!written) {
        printf("Encode failed.\n");
        return 1;
    }

    // 4. Декодируем в decoded_name для проверки
    if (!decode_file(compressed_name, decoded_name)) {
        printf("Decode failed.\n");
        return 1;
    }

    // 5. Проверка совпадения файлов
    if (files_equal(input_name, decoded_name)) {
        printf("OK: decoded file matches original.\n");
    } else {
        printf("WARNING: decoded file differs from original.\n");
    }

    // 6. Статистика размеров
    long size_in  = get_file_size(input_name);
    long size_cmp = get_file_size(compressed_name);
    long size_dec = get_file_size(decoded_name);
    if (size_in >= 0 && size_cmp > 0) {
        double ratio = (double)size_in / (double)size_cmp;
        printf("Input size:      %ld bytes\n", size_in);
        printf("Compressed size: %ld bytes\n", size_cmp);
        printf("Decoded size:    %ld bytes\n", size_dec);
        printf("Compression ratio: %.2f\n", ratio);
    }

    return 0;
}

// Вспомогательная функция: режим decode
static int run_decode(const char *compressed_name,
                      const char *decoded_name) {
    if (!decode_file(compressed_name, decoded_name)) {
        printf("Decode failed.\n");
        return 1;
    }
    printf("Decode finished.\n");
    return 0;
}

int main(int argc, char *argv[]) {
    // Примеры запуска:
    //   ./hahman encode input.txt compressed.bin decoded.txt
    //   ./hahman decode compressed.bin output.txt
    printf("main started, argc = %d\n", argc);

    if (argc > 1) {
        if (strcmp(argv[1], "encode") == 0) {
            if (argc != 5) {
                printf("Usage: %s encode <input> mpressed> <decoded>\n", argv[0]);
                return 1;
            }
            const char *input_name      = argv[2];
            const char *compressed_name = argv[3];
            const char *decoded_name    = argv[4];
            return run_encode(input_name, compressed_name, decoded_name);
        } else if (strcmp(argv[1], "decode") == 0) {
            if (argc != 4) {
                printf("Usage: %s decode mpressed> <output>\n", argv[0]);
                return 1;
            }
            const char *compressed_name = argv[2];
            const char *decoded_name    = argv[3];
            return run_decode(compressed_name, decoded_name);
        } else {
            printf("Unknown mode: %s\n", argv[1]);
            printf("Usage:\n");
            printf("  %s encode <input> mpressed> <decoded>\n", argv[0]);
            printf("  %s decode mpressed> <output>\n", argv[0]);
            return 1;
        }
    }

    // Если аргументов нет — просто подсказка и выход
    printf("Usage:\n");
    printf("  %s encode <input> mpressed> <decoded>\n", argv[0]);
    printf("  %s decode mpressed> <output>\n", argv[0]);
    return 1;
}

