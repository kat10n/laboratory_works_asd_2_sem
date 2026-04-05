#include "struct.h"
#include "mini_calculator.h"
#include "reverse_polish_notation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_number_node(Node *n) {
    if (!n || !n->data) return 0;
    for (int i = 0; n->data[i]; i++)
        if (!isdigit((unsigned char)n->data[i])) return 0;
    return 1;
}

Node *make_number_node(int value) {
    char buf[32];
    sprintf(buf, "%d", value);
    return create_node(buf);
}

void free_tree(Node *n) {
    if (!n) return;
    free_tree(n->left);
    free_tree(n->right);
    free(n->data);
    free(n);
}

static void collect_factors(Node *n, int *prod, Node **vars, int *count) {
    if (!n) return;
    if (n->data[0] == '*') {
        Node *left = n->left, *right = n->right;
        free(n->data); free(n);
        collect_factors(left,  prod, vars, count);
        collect_factors(right, prod, vars, count);
    } else if (is_number_node(n)) {
        *prod *= atoi(n->data);
        free_tree(n);
    } else {
        vars[(*count)++] = n;
    }
}

// Собирает дерево умножений из переменных факторов и числового коэффициента.
static Node *build_mul_tree(Node **vars, int count, int prod) {
    if (count == 0) return make_number_node(prod);

    Node *result = vars[0];
    for (int i = 1; i < count; i++) {
        Node *mul = create_node("*");
        mul->left  = result;
        mul->right = vars[i];
        result = mul;
    }
    if (prod != 1) {
        Node *mul = create_node("*");
        mul->left  = make_number_node(prod);
        mul->right = result;
        result = mul;
    }
    return result;
}

// Упрощает умножение в дереве: схлопывает числовые множители в одно число.
// Пример: 7 * 6 * j * k * 4  →  168 * j * k
Node *simplify_multiplication(Node *root) {
    if (!root) return NULL;
    root->left  = simplify_multiplication(root->left);
    root->right = simplify_multiplication(root->right);

    if (!root->data || root->data[0] != '*') return root;

    int prod = 1;
    Node *vars[64];
    int count = 0;
    collect_factors(root, &prod, vars, &count);
    return build_mul_tree(vars, count, prod);
}

// Записывает дерево как инфиксное выражение без пробелов и скобок.
static void to_infix(Node *n, char *buf, int *len) {
    if (!n) return;
    if (n->left) { // узел-оператор
        to_infix(n->left, buf, len);
        buf[(*len)++] = n->data[0];
        buf[*len] = '\0';
        to_infix(n->right, buf, len);
    } else { // лист
        int slen = strlen(n->data);
        memcpy(buf + *len, n->data, slen + 1);
        *len += slen;
    }
}

char *tree_to_infix(Node *n) {
    char *buf = malloc(1024);
    int len = 0;
    buf[0] = '\0';
    to_infix(n, buf, &len);
    return buf;
}