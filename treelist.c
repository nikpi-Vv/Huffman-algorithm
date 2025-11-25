#include "treelist.h"

// вставляет элемент в список
// Вставляет существующий узел в список (или создаёт новый, если node == NULL)
struct node_t * insert_node(struct node_t *root, struct node_t *node) {
    // Если узел не передан — это ошибка
    if (node == NULL) {
        fprintf(stderr, "Node is NULL!\n");
        return root;
    }
    
    // node->next всегда NULL, так как это новый узел
    node->next = NULL;
    
    // Если список пуст
    if (root == NULL) return node;
    
    // Если новый узел меньше первого
    if (node->frequency < root->frequency) {
        node->next = root;
        return node;
    }
    
    // Ищем правильное место
    struct node_t *current = root;
    while (current->next != NULL && current->next->frequency <= node->frequency) {
        current = current->next;
    }
    
    // Вставляем узел
    node->next = current->next;
    current->next = node;
    return root;
}

// создает упорядоченный по убыванию список из массива
struct node_t * list_from_freq_array(unsigned char *frequencies) {
    struct node_t *root = NULL;
    
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            // Создаём новый узел
            struct node_t *new_node = (struct node_t *) malloc(sizeof(struct node_t));
            
            if (new_node == NULL) {
                fprintf(stderr, "Memory allocation failed!\n");
                return NULL;
            }
            
            // Заполняем узел
            new_node->symbol = (unsigned char)i;
            new_node->frequency = frequencies[i];
            new_node->next = NULL;
            new_node->left = NULL;
            new_node->right = NULL;
            
            // Вставляем узел в список
            root = insert_node(root, new_node);
        }
    }
    
    return root;
}

//— объединяет два узла
struct node_t * make_new_node(struct node_t *left, struct node_t *right) {
    // TODO: Создаёшь новый узел
    struct node_t *new_node = (struct node_t *) malloc(sizeof(struct node_t));

    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return NULL;
    }

    new_node->symbol = 0; //внутренний узел(не лист)
    new_node->frequency = left->frequency + right->frequency;
    new_node->next = NULL;
    new_node->left = left;
    new_node->right = right;
    return new_node;
}

//строит полное дерево
struct node_t * make_tree(struct node_t *root) {
    while (root != NULL && root->next != NULL) {
        // Берём первые два узла
        struct node_t *left = root;
        struct node_t *right = root->next;
        
        // Удаляем их из списка
        root = root->next->next;
        
        // Объединяем в новый узел
        struct node_t *new_node = make_new_node(left, right);
        
        // Вставляем новый узел обратно
        root = insert_node(root, new_node);
    }
    
    return root;
}

// Заполняет таблицу кодов символов путём обхода дерева
void make_code_table(struct node_t *root, code_table_t *table) {
    // Инициализируем таблицу (все коды = пусто)
    for (int i = 0; i < 256; i++) {
        table->codes[i].length = 0;
    }
    
    // Вспомогательный буфер для текущего кода (рекурсия!)
    unsigned char current_code[256]; //чтобы хранить путь по дереву от корня до каждого листа 
    int code_length = 0;//длина этого маршрута (номер шага вниз)
    
    // Начинаем рекурсивный обход с корня
    generate_codes(root, table, current_code, code_length);
}

// Рекурсивно обходит дерево и заполняет коды
void generate_codes(struct node_t *node, code_table_t *table, 
                    unsigned char *current_code, int code_length) {
    
    // Базовый случай: если узел = лист (символ!)
    if (node->left == NULL && node->right == NULL) { //Лист — сохраняем результат и выходим
        // Это листик, сохраняем его код
        for (int i = 0; i < code_length; i++) {
            table->codes[node->symbol].bits[i] = current_code[i];
        }
        table->codes[node->symbol].length = code_length;
        return;
    }
    
    // Рекурсивный случай: идём влево и вправо
    
    // Идём ВЛЕВО (добавляем бит 0)
    if (node->left != NULL) {
        current_code[code_length] = 0;  // Добавляем 0
        generate_codes(node->left, table, current_code, code_length + 1);
    }
    
    // Идём ВПРАВО (добавляем бит 1)
    if (node->right != NULL) {
        current_code[code_length] = 1;  // Добавляем 1
        generate_codes(node->right, table, current_code, code_length + 1);
    }
}

