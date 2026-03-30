#ifndef LABORATORY_WORKS_ASD_2_SEM_MINI_CALCULATOR_H
#define LABORATORY_WORKS_ASD_2_SEM_MINI_CALCULATOR_H
#include "struct.h"
int is_number_node(Node *n);
Node *make_number_node(int value);
void collect_factors(Node *n, int *prod, Node ***factors, int *count, int *cap);
Node *build_mul_tree(Node **factors, int count, int prod);
Node *simplify_multiplication(Node *root);
char *tree_to_infix(Node *n);
void free_tree(Node *n);
#endif //LABORATORY_WORKS_ASD_2_SEM_MINI_CALCULATOR_H