#ifndef LABORATORY_WORKS_ASD_2_SEM_MINI_CALCULATOR_H
#define LABORATORY_WORKS_ASD_2_SEM_MINI_CALCULATOR_H
#include "struct.h"
int   is_number_node(Node *n);
Node *make_number_node(int value);
Node *copy_tree(Node *n);
Node *expand_brackets(Node *root);
Node *simplify_multiplication(Node *root);
char *tree_to_infix(Node *n);
void  free_tree(Node *n);
#endif