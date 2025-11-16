#include "treelist.h"

struct node_t {
    unsigned char symbol;
    unsigned long long frequency;
    struct node_t *next;
    struct node_t *left;
    struct node_t *right;
};

// вставляет элемент в список
void insert_node(struct node_t *root, int pos) {
    
}