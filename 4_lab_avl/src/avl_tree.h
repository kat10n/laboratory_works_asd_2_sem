#ifndef LABORATORY_WORKS_ASD_2_SEM_AVL_TREE_H
#define LABORATORY_WORKS_ASD_2_SEM_AVL_TREE_H
#include <stdio.h>
#include "struct.h"
int get_height(Node* node);
int bfactor(Node* node);
int max(int a, int b);
void fix_height(Node* node);
Node* rotate_right(Node* node);
Node* rotate_left(Node* node);
Node* balance(Node* node);
Node* insert(Node* root, char* key, double value);
Node* search(Node* root, char* key);
Node* find_min(Node* node);
Node* remove_min(Node* p);
Node* remove_node(Node* root, char* key);
void printTree(Node* root, int depth);
void printTreeToFile(Node* root, int depth, FILE* out);
#endif //LABORATORY_WORKS_ASD_2_SEM_AVL_TREE_H