#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"


int get_height(Node* node) {
    if (node == NULL) return 0;
    return node->height;
}

int bfactor(Node* node) {
    if (node == NULL) return 0;
    int left = get_height(node->left);
    int right = get_height(node->right);
    return right - left;
}

int max(int a, int b) {
    return a > b ? a : b;
}

void fix_height(Node* node) {
    if (node == NULL) return;
    int left = get_height(node->left);
    int right = get_height(node->right);
    node->height = max(left, right) + 1;
}

Node* rotate_right(Node* node) {
    Node* left = node->left;
    node->left = left->right;
    left->right = node;
    fix_height(node);
    fix_height(left);
    return left;
}

Node* rotate_left(Node* node) {
    Node* right = node->right;
    node->right = right->left;
    right->left = node;
    fix_height(node);
    fix_height(right);
    return right;
}

Node* balance(Node* node) {
    if (node == NULL) return NULL;
    fix_height(node);
    if (bfactor(node) == 2) {
        if (bfactor(node->right) < 0) {
            node->right = rotate_right(node->right);
        }
        return rotate_left(node);
    }
    if (bfactor(node) == -2) {
        /* BUGFIX: было < 0, должно быть > 0 -- Left-Right случай */
        if (bfactor(node->left) > 0) {
            node->left = rotate_left(node->left);
        }
        return rotate_right(node);
    }
    return node;
}

Node* insert(Node* root, char* key, double value) {
    if (root == NULL) {
        Node* node = (Node*)malloc(sizeof(Node));
        strncpy(node->key, key, 6);
        node->key[6] = '\0';
        node->value = value;
        node->height = 1;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    if (strcmp(key, root->key) < 0) {
        root->left = insert(root->left, key, value);
    } else if (strcmp(key, root->key) > 0) {
        root->right = insert(root->right, key, value);
    }
    return balance(root);
}

Node* search(Node* root, char* key) {
    if (root == NULL) return NULL;
    int cmp = strcmp(key, root->key);
    if (cmp == 0) {
        return root;
    } else if (cmp < 0) {
        return search(root->left, key);
    } else {
        return search(root->right, key);
    }
}

Node* find_min(Node* node) {
    if (node == NULL) return NULL;
    return node->left ? find_min(node->left) : node;
}

Node* remove_min(Node* p) {
    if (p->left == NULL) {
        return p->right;
    }
    p->left = remove_min(p->left);
    return balance(p);
}

Node* remove_node(Node* root, char* key) {
    if (root == NULL) return NULL;
    int cmp = strcmp(key, root->key);
    if (cmp < 0) {
        root->left = remove_node(root->left, key);
    } else if (cmp > 0) {
        root->right = remove_node(root->right, key);
    } else {
        Node* left = root->left;
        Node* right = root->right;
        if (right == NULL) {
            free(root);
            return left;
        }
        Node* min = find_min(right);
        min->right = remove_min(right);
        min->left = left;
        free(root);
        return balance(min);
    }
    return balance(root);
}

void printTree(Node* root, int depth) {
    if (root == NULL) return;
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("%s %f\n", root->key, root->value);
    printTree(root->left, depth + 1);
    printTree(root->right, depth + 1);
}
