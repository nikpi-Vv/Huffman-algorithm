#include "codec.h"
#include "treelist.h"
#include <stdio.h>
// Кодирует входной файл с помощью таблицы кодов и записывает в выходной бинарный
int encode_file(const char *input_filename, const char *output_filename, 
                code_table_t *table, unsigned char *frequencies, long input_len) {

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

    // записываем длину исходного файла (количество символов)
    fwrite(&input_len, sizeof(long), 1, out);

    // --- 2. Кодируем данные ---
    unsigned char buffer = 0; //временный байт, в котором мы постепенно собираем биты.
    int bits_filled = 0; //сколько бит сейчас занято в buffer (от 0 до 8).
    int bytes_written = 0; //сколько байт данных

    int c;
    while ((c = fgetc(in)) != EOF) { //Читаем каждый символ из исходного файла до EOF
        code_t code = table->codes[c]; //содержит последовательность бит и длину кода
        // Для каждого бита в коде символа
        for (int i = 0; i < code.length; i++) { // Проходим по всем битам кода символа
            buffer <<= 1;              // Сдвигаем левее
            buffer |= code.bits[i];    // Добавляем бит
            bits_filled++;
            if (bits_filled == 8) { //если байт заполнился
                fputc(buffer, out);    // Записали байт!
                bytes_written++;
                buffer = 0;
                bits_filled = 0;
            }
        }
    }

    //  После окончания файла могли остаться незаписанные биты (менее 8)
    if (bits_filled > 0) {
        buffer <<= (8 - bits_filled); // дополнить нулями
        fputc(buffer, out);
        bytes_written++;
    }

    fclose(in);
    fclose(out);
    return bytes_written; //Возвращаем число записанных байт данных (может пригодиться для статистики)
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
    if (fread(frequencies, 1, 256, in) != 256) { // fread возвращает 
                        //количество реально прочитанных элементов (здесь байтов)
        fprintf(stderr, "Failed to read frequency table.\n");
        fclose(in); fclose(out);
        return 0;
    }

    // 2. читаем длину исходного файла (кол-во символов)
    long original_len = 0;
    if (fread(&original_len, sizeof(long), 1, in) != 1) {
        fprintf(stderr, "Failed to read original length.\n");
        fclose(in); fclose(out);
        return 0;
    }

    // --- 2. Восстанавливаем дерево по таблице частот ---
    struct node_t *list = list_from_freq_array(frequencies); //создаём список частот
    struct node_t *tree = make_tree(list); // строим дерево из списка
    // --- 3. Декодируем файл ---
    struct node_t *node = tree; // node — текущий узел при спуске по дереву 
                                                        //(0 — влево, 1 — вправо)
    int byte;
    int bits_left = 0; // сколько бит осталось разобрать в текущем байте
    unsigned char current_byte = 0; // текущий байт, который мы разбираем по битам
    long decoded_count = 0; // сколько символов уже декодировано

    //Читаем каждый байт из сжатого файла до EOF
    while (decoded_count < original_len && (byte = fgetc(in)) != EOF) { 
        current_byte = (unsigned char)byte;
        bits_left = 8; // в каждом байте 8 бит

        //Разбираем байт по битам (от старшего к младшему)
        while (bits_left > 0 && decoded_count < original_len) { 
            unsigned char bit = (current_byte & 0x80) ? 1 : 0; // Самый старший бит 
                                                    //(бит с маской 1000 0000)
            current_byte <<= 1; //Сдвигаем байт влево, чтобы следующий бит стал старшим
            bits_left--;

            // Движение по дереву: 0 — влево, 1 — вправо
            if (bit == 0 && node->left) 
                node = node->left;
            else if (bit == 1 && node->right) 
                node = node->right;

            // Если дошли до листа
            if (node->left == NULL && node->right == NULL) {
                fputc(node->symbol, out);
                decoded_count++;
                node = tree; // возвращаемся на вершину
            }
        }
    }

    fclose(in);
    fclose(out);
    return 1; // успех
}