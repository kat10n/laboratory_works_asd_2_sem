#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Вспомогательная функция — копия одного узла (листа)
Node* copy_tree(Node *node) {
    if (node == NULL) return NULL;
    Node *copy  = malloc(sizeof(Node));
    copy->data  = node->data;
    copy->left  = copy_tree(node->left);   // рекурсия
    copy->right = copy_tree(node->right);  // рекурсия
    return copy;
}

// Основная функция — рекурсивное раскрытие скобок
Node* distribute(Node *root) {
    if (root == NULL) return NULL;

    // Сначала рекурсивно обрабатываем поддеревья
    root->left  = distribute(root->left);
    root->right = distribute(root->right);

    // Нас интересуют только узлы умножения
    if (root->data != '*') return root;

    // Случай 1: a * (b +/- c)
    if (root->right && (root->right->data == '+' || root->right->data == '-')) {
        Node *plus = root->right;   // узел + или -
        Node *a    = root->left;    // левый операнд

        // левая ветка: a * b
        Node *mul_left = malloc(sizeof(Node));
        mul_left->data  = '*';
        mul_left->left  = a;
        mul_left->right = plus->left;

        // правая ветка: a * c  (нужна КОПИЯ a, т.к. a уже занята)
        Node *mul_right = malloc(sizeof(Node));
        mul_right->data  = '*';
        mul_right->left  = copy_tree(a);   // копия a
        mul_right->right = plus->right;

        // перевешиваем детей узла +
        plus->left  = mul_left;
        plus->right = mul_right;

        free(root); // старый узел * больше не нужен
        return distribute(plus); // рекурсия — вдруг внутри ещё есть что раскрывать
    }

    // Случай 2: (a +/- b) * c  — то же самое, но скобки слева
    if (root->left && (root->left->data == '+' || root->left->data == '-')) {
        Node *plus = root->left;
        Node *c    = root->right;

        Node *mul_left = malloc(sizeof(Node));
        mul_left->data  = '*';
        mul_left->left  = plus->left;
        mul_left->right = c;

        Node *mul_right = malloc(sizeof(Node));
        mul_right->data  = '*';
        mul_right->left  = plus->right;
        mul_right->right = copy_tree(c);

        plus->left  = mul_left;
        plus->right = mul_right;

        free(root);
        return distribute(plus);
    }

    // Случай 3: оба операнда — числа, можно посчитать
    // (работает только для однозначных чисел из-за char!)
    if (root->left  && isdigit(root->left->data) &&
        root->right && isdigit(root->right->data)) {

        int result = (root->left->data - '0') * (root->right->data - '0');

        Node *res = malloc(sizeof(Node));
        res->data  = '0' + result; // ⚠️ работает только если result < 10
        res->left  = NULL;
        res->right = NULL;

        free(root->left);
        free(root->right);
        free(root);
        return res;
    }

    return root; // ничего не раскрываем, возвращаем как есть
}