#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "enter.h"
#include "reverse_polish_notation.h"
#include "mini_calculator.h"

void setup_utf8() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

void print_tree_infix(Node *n) {
    if (!n) return;
    if (n->data && (n->data[0] == '+' || n->data[0] == '-' ||
                    n->data[0] == '*' || n->data[0] == '/' ||
                    n->data[0] == '%' || n->data[0] == '^')) {
        print_tree_infix(n->left);
        printf(" %s ", n->data);
        print_tree_infix(n->right);
                    } else {
                        printf("%s", n->data);
                    }
}

int main() {
    setup_utf8();
    while (1) {
        char *line, *rpn;
        Node *tree;

        printf("Введите выражение или exit для выхода: ");
        line = enter();
        if (line == NULL) break;
        if (strcmp(line, "exit") == 0) { free(line); break; }
        if (line[0] == '\0') { free(line); continue; }
        if (!validation(line)) {
            printf("выражение не прошло валидацию, введите корректное выражение\n");
            free(line);
            continue;
        }

        rpn = reverse_polish_notation(line);
        printf("ОПЗ: %s\n", rpn);

        tree = make_tree(rpn);
        printf("Дерево:\n");
        print_tree(tree, 0);

        tree = simplify(tree);
        printf("Результат: ");
        print_tree_infix(tree);
        printf("\n");

        free(line);
        free(rpn);
        free_tree(tree);
    }
    return 0;
}