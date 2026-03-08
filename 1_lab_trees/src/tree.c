#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "struct.h"

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode) {
        newNode->data = value;
        newNode->firstChild = NULL;
        newNode->nextSibling = NULL;
    } else {
        printf("Память не выделена\n");
    }
    return newNode;
}

void addChild(Node* parent, Node* newChild) {
    if (parent == NULL) {
        printf("такого родителя нет\n");
        return;
    }
    if (newChild == NULL) {
        printf("такого узла нет\n");
        return;
    }
    if (parent->firstChild == NULL) {
        parent->firstChild = newChild;
    } else {
        Node* sibling = parent->firstChild;
        while (sibling->nextSibling != NULL) {
            sibling = sibling->nextSibling;
        }
        sibling->nextSibling = newChild;
    }
}

void removeSubtree(Node* root) {
    if (root == NULL){
        printf("такого узла нет\n");
        return;
    }
    Node* child = root->firstChild;
    while (child != NULL) {
        Node* next = child->nextSibling;
        removeSubtree(child);
        child = next;
    }
    free(root);
}

void printTree(Node* root, int depth) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("%d\n", root->data);
    printTree(root->firstChild, depth + 1);
    printTree(root->nextSibling, depth);
}


void findMinDepthLeafHelper(Node* root, int depth, int *minDepth, int *minValue) {
    if (root == NULL) return;

    // Если узел является листом
    if (root->firstChild == NULL) {
        if (depth < *minDepth) {
            *minDepth = depth;
            *minValue = root->data;
        }
    }

    // Обходим потомков
    findMinDepthLeafHelper(root->firstChild, depth + 1, minDepth, minValue);
    // Обходим сиблингов
    findMinDepthLeafHelper(root->nextSibling, depth, minDepth, minValue);
}

// Осн функция
int findMinDepthLeaf(Node* root) {
    if (root == NULL) {
        printf("Дерево пусто.\n");
        return -1;
    }

    int minDepth = INT_MAX;
    int minValue = 0;
    findMinDepthLeafHelper(root, 0, &minDepth, &minValue);

    if (minDepth == INT_MAX) {
        printf("В дереве нет листьев.\n");
        return -1;
    }

    return minValue;
}
