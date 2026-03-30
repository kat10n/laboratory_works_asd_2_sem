#ifndef LABORATORY_WORKS_ASD_2_SEM_REVERSE_POLISH_NOTATION_H
#define LABORATORY_WORKS_ASD_2_SEM_REVERSE_POLISH_NOTATION_H
#include "struct.h"
char  *reverse_polish_notation(char *line);
Node  *make_tree(char *rpn);
void   print_tree(Node *root, int depth);
Node  *create_node(const char *value);
#endif //LABORATORY_WORKS_ASD_2_SEM_REVERSE_POLISH_NOTATION_H