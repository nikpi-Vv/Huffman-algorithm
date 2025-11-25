#include "treelist.h"
#include <stdlib.h>
#include <stdio.h>

int main (int argn, char *argc[]) {
    (void)argn;
    (void)argc;

    
    // Пример массива частот (например, "AABBBCCCC")
    unsigned char frequencies[256] = {0};
    frequencies['A'] = 2;
    frequencies['B'] = 3;
    frequencies['C'] = 4;

    // 1. Создаём отсортированный список узлов
    struct node_t *root = list_from_freq_array(frequencies);

    // 2. Строим дерево Хаффмана
    struct node_t *tree = make_tree(root);

    // 3. Создаём таблицу кодов (пока структуры: code_t, code_table_t)
    code_table_t table;
    make_code_table(tree, &table);

    // 4. Показываем коды для символов (пример, как всё работает)
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            printf("Symbol: %c, Code: ", i);
            for (int j = 0; j < table.codes[i].length; j++)
                printf("%d", table.codes[i].bits[j]);
            printf("\n");
        }
    }

    // 5. Освобождаем память (реализацию free оставим позже)

    return 0;
}