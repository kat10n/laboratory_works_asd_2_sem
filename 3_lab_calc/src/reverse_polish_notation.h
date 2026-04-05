#ifndef LABORATORY_WORKS_ASD_2_SEM_REVERSE_POLISH_NOTATION_H
#define LABORATORY_WORKS_ASD_2_SEM_REVERSE_POLISH_NOTATION_H
#include "struct.h"
Node *create_node(const char *value);
int   is_operator(const char *token);
char *reverse_polish_notation(char *line);
Node *make_tree(char *rpn);
void  print_tree(Node *root, int depth);
#endif //LABORATORY_WORKS_ASD_2_SEM_REVERSE_POLISH_NOTATION_H