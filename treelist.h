#ifndef TREELIST_H
#define TREELIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Структура узла дерева
struct node_t {
    unsigned char symbol;           // символ (для листьев)
    unsigned long long frequency;   // частота/вес
    struct node_t *next;            // связь в списке (для построения)
    struct node_t *left;            // левое поддерево (0)
    struct node_t *right;           // правое поддерево (1)
};

// Функции
// вставляет элемент в список
// Вставляет узел в отсортированный по частоте список
struct node_t * insert_node(struct node_t *root, struct node_t *node);

// создает упорядоченный по убыванию список из массива
struct node_t * list_from_freq_array(unsigned char *frequencies);

// создает новый элемент дерева из двух элементов дерева
struct node_t * make_new_node(struct node_t *left, struct node_t *right);

// создает дерево из упорядоченного списка
struct node_t make_tree(struct node_t *root);

// заполняет таблицу кодов символов
void make_code_table(struct node_t *root, unsigned char *codes);

// возвращает код символа по дереву
unsigned char get_char_code(unsigned char symbol, struct node_t *root);

#endif