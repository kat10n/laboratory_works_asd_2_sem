#ifndef UNTITLED1_TREE_H
#define UNTITLED1_TREE_H
#include "struct.h"

Node* createNode(int value);
void addChild(Node* parent, Node* newChild);
void removeSubtree(Node* root);
void printTree(Node* root, int depth);
void findMinDepthLeafHelper(Node* root, int depth, int *minDepth, int *minValue);
int findMinDepthLeaf(Node* root);

#endif //UNTITLED1_TREE_H