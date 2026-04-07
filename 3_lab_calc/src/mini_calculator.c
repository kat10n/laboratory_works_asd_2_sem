#include "struct.h"
#include "mini_calculator.h"
#include "dynstr.h"
#include "reverse_polish_notation.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

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
    Node *copy  = create_node(n->data);
    copy->left  = copy_tree(n->left);
    copy->right = copy_tree(n->right);
    return copy;
}

static int is_sum(Node *n) {
    return n->data[0] == '+' || n->data[0] == '-';
}

static int mul_product(Node *n) {
    if (n->data[0] == '*')
        return mul_product(n->left) * mul_product(n->right);
    return is_number_node(n) ? atoi(n->data) : 1;
}

static Node *mul_strip_nums(Node *n) {
    if (n->data[0] != '*') {
        if (is_number_node(n)) { free_tree(n); return NULL; }
        return n;
    }
    n->left  = mul_strip_nums(n->left);
    n->right = mul_strip_nums(n->right);
    if (!n->left)  { 
        Node *r = n->right; 
        free(n->data); 
        free(n); 
        return r; 
    }
    if (!n->right) { 
        Node *l = n->left;  
        free(n->data); 
        free(n);
        return l; 
    }
    return n;
}

static Node *collapse_mul(Node *root) {
    int   prod = mul_product(root);
    Node *vars = mul_strip_nums(root);
    if (!vars) return make_number_node(prod);
    if (prod == 1) return vars;
    Node *mul  = create_node("*");
    mul->left  = make_number_node(prod);
    mul->right = vars;
    return mul;
}

Node *simplify(Node *root) {
    if (!root) return NULL;
    root->left  = simplify(root->left);
    root->right = simplify(root->right);

    if (root->data[0] != '*') return root;

    Node *L = root->left, *R = root->right;

    // a*(b +- c)  -> a*b +- a*c
    if (is_sum(R)) {
        Node *mul1  = create_node("*");
        mul1->left  = copy_tree(L);
        mul1->right = R->left;

        Node *mul2  = create_node("*");
        mul2->left  = L;
        mul2->right = R->right;

        char op[2]    = {R->data[0], '\0'};
        Node *result  = create_node(op);
        result->left  = mul1;
        result->right = mul2;

        free(R->data); free(R);
        free(root->data); free(root);
        return simplify(result);
    }
    // (a +- b) * c  ->  a*c +- b*c
    if (is_sum(L)) {
        Node *mul1  = create_node("*");
        mul1->left  = L->left;
        mul1->right = copy_tree(R);

        Node *mul2  = create_node("*");
        mul2->left  = L->right;
        mul2->right = R;

        char op[2]    = {L->data[0], '\0'};
        Node *result  = create_node(op);
        result->left  = mul1;
        result->right = mul2;

        free(L->data); free(L);
        free(root->data); free(root);
        return simplify(result);
    }

    return collapse_mul(root);
}

static void to_infix(Node *n, DynStr *buf) {
    if (!n) return;
    if (n->left) {
        to_infix(n->left, buf);
        dynstr_append_char(buf, n->data[0]);
        to_infix(n->right, buf);
    } else {
        dynstr_append(buf, n->data);
    }
}

char *tree_to_infix(Node *n) {
    DynStr buf = dynstr_create();
    to_infix(n, &buf);
    return dynstr_take(&buf);
}