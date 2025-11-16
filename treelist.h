struct node_t;

// вставляет элемент в список
void insert_node(struct node_t *root, int pos);

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