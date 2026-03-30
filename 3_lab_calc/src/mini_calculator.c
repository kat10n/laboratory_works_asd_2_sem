#include "struct.h"
#include "reverse_polish_notation.h"
#include "mini_calculator.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int is_number_node(Node *n) {
    if (!n || !n->data) return 0;
    for (int i = 0; n->data[i]; i++) {
        if (!isdigit((unsigned char)n->data[i])) return 0;
    }
    return 1;
}

Node *make_number_node(int value) {
    char buf[32];
    sprintf(buf, "%d", value);
    return create_node(buf);
}

void collect_factors(Node *n, int *prod, Node ***factors, int *count, int *cap) {
    if (!n) return;
    if (n->data && n->data[0] == '*') {
        collect_factors(n->left, prod, factors, count, cap);
        collect_factors(n->right, prod, factors, count, cap);
        return;
    }
    if (is_number_node(n)) {
        *prod *= atoi(n->data);
        return;
    }
    if (*count >= *cap) {
        *cap = (*cap == 0) ? 4 : (*cap * 2);
        *factors = realloc(*factors, sizeof(Node*) * (*cap));
    }
    (*factors)[*count] = n;
    (*count)++;
}

Node *build_mul_tree(Node **factors, int count, int prod) {
    if (count == 0) return make_number_node(prod);
    if (prod == 1) {
        Node *result = factors[0];
        for (int i = 1; i < count; i++) {
            Node *new_node = create_node("*");
            new_node->left = result;
            new_node->right = factors[i];
            result = new_node;
        }
        return result;
    }
    Node *result = make_number_node(prod);
    for (int i = 0; i < count; i++) {
        Node *new_node = create_node("*");
        new_node->left = result;
        new_node->right = factors[i];
        result = new_node;
    }
    return result;
}

void free_tree(Node *n) {
    if (!n) return;
    free_tree(n->left);
    free_tree(n->right);
    free(n->data);
    free(n);
}

static int is_op_node(Node *n) {
    return n && n->data && strlen(n->data) == 1 &&
           strchr("+-*/^%", n->data[0]) != NULL;
}

static void append_piece(char **buf, size_t *len, const char *s) {
    size_t add = strlen(s ? s : "");
    char *nbuf = realloc(*buf, (*len + add + 1) * sizeof(char));
    if (!nbuf) return;
    memcpy(nbuf + *len, s ? s : "", add);
    nbuf[*len + add] = '\0';
    *buf = nbuf;
    *len += add;
}

static void to_infix(Node *n, char **buf, size_t *len) {
    if (!n) return;
    if (is_op_node(n)) {
        to_infix(n->left, buf, len);
        append_piece(buf, len, n->data);
        to_infix(n->right, buf, len);
    } else {
        append_piece(buf, len, n->data);
    }
}

char *tree_to_infix(Node *n) {
    char *buf = malloc(1);
    if (!buf) return NULL;
    buf[0] = '\0';
    size_t len = 0;
    to_infix(n, &buf, &len);
    return buf;
}

Node *simplify_multiplication(Node *root) {
    if (!root) return NULL;
    root->left = simplify_multiplication(root->left);
    root->right = simplify_multiplication(root->right);
    if (!root->data || root->data[0] != '*') return root;
    int prod = 1;
    Node **factors = NULL;
    int count = 0, cap = 0;
    collect_factors(root, &prod, &factors, &count, &cap);
    Node *new_tree = build_mul_tree(factors, count, prod);
    free(root->data);
    free(root);
    free(factors);
    return new_tree;
}