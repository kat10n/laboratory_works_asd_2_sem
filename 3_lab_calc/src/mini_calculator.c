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

Node *copy_tree(Node *n) {
    if (!n) return NULL;
    Node *copy = create_node(n->data);
    copy->left  = copy_tree(n->left);
    copy->right = copy_tree(n->right);
    return copy;
}

Node *expand_brackets(Node *root) {
    if (!root) return NULL;
    root->left  = expand_brackets(root->left);
    root->right = expand_brackets(root->right);

    if (root->data[0] != '*') return root;

    Node *L = root->left, *R = root->right;

    // a * (b + c)
    if (R->data[0] == '+' || R->data[0] == '-') {
        Node *mul1 = create_node("*");
        mul1->left  = copy_tree(L);
        mul1->right = R->left;

        Node *mul2 = create_node("*");
        mul2->left  = L;
        mul2->right = R->right;

        char op[2] = {R->data[0], '\0'};
        Node *result = create_node(op);
        result->left  = mul1;
        result->right = mul2;

        free(R->data); free(R);
        free(root->data); free(root);
        return expand_brackets(result);
    }

    // (a + b) * c
    if (L->data[0] == '+' || L->data[0] == '-') {
        Node *mul1 = create_node("*");
        mul1->left  = L->left;
        mul1->right = copy_tree(R);

        Node *mul2 = create_node("*");
        mul2->left  = L->right;
        mul2->right = R;

        char op[2] = {L->data[0], '\0'};
        Node *result = create_node(op);
        result->left  = mul1;
        result->right = mul2;

        free(L->data); free(L);
        free(root->data); free(root);
        return expand_brackets(result);
    }

    return root;
}

static void collect_factors(Node *n, int *prod, Node ***vars, int *count, int *cap) {
    if (!n) return;
    if (n->data[0] == '*') {
        Node *left = n->left, *right = n->right;
        free(n->data); free(n);
        collect_factors(left,  prod, vars, count, cap);
        collect_factors(right, prod, vars, count, cap);
    } else if (is_number_node(n)) {
        *prod *= atoi(n->data);
        free_tree(n);
    } else {
        if (*count >= *cap) {
            *cap *= 2;
            Node **tmp = realloc(*vars, sizeof(Node *)*(*cap));
            if (!tmp) {
                printf("Ошибка выделения памяти\n");
                return;
            }
            *vars = tmp;
        }
        (*vars)[(*count)++] = n;
    }
}

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

Node *simplify_multiplication(Node *root) {
    if (!root) return NULL;
    root->left  = simplify_multiplication(root->left);
    root->right = simplify_multiplication(root->right);

    if (!root->data || root->data[0] != '*') return root;

    int prod  = 1;
    int cap   = 8;
    int count = 0;
    Node **vars = malloc(sizeof(Node *) * cap);
    if (!vars) {
        printf("Ошибка выделения памяти\n");
        return root;
    }
    collect_factors(root, &prod, &vars, &count, &cap);
    Node *result = build_mul_tree(vars, count, prod);
    free(vars);
    return result;
}

static void to_infix(Node *n, char **buf, int *len, int *cap) {
    if (!n) return;
    if (n->left) {
        to_infix(n->left, buf, len, cap);
        if (*len + 2 > *cap) {
            *cap *= 2;
            char *tmp = realloc(*buf, *cap);
            if (!tmp) { printf("Ошибка выделения памяти\n"); return; }
            *buf = tmp;
        }
        (*buf)[(*len)++] = n->data[0];
        (*buf)[*len] = '\0';
        to_infix(n->right, buf, len, cap);
    } else {
        int slen = (int)strlen(n->data);
        while (*len + slen + 1 > *cap) {
            *cap *= 2;
            char *tmp = realloc(*buf, *cap);
            if (!tmp) { printf("Ошибка выделения памяти\n"); return; }
            *buf = tmp;
        }
        memcpy(*buf + *len, n->data, slen + 1);
        *len += slen;
    }
}

char *tree_to_infix(Node *n) {
    int cap = 64, len = 0;
    char *buf = malloc(cap);
    if (!buf) {
        printf("Ошибка выделения памяти\n");
        return NULL;
    }
    buf[0] = '\0';
    to_infix(n, &buf, &len, &cap);
    return buf;
}