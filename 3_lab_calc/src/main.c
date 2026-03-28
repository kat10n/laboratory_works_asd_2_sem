#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include "enter.h"
#include "tokenize.h"
#include "reverse_polish_notation.h"
#include "mini_calculator.h"
#include "struct.h"

int main() {
    char *line = enter();
    
    if (!validation(line)) {
        printf("Ошибка в выражении\n");
        return 1;
    }

    char **tokens = tokenize(line);

    char *rpn = reverse_polish_notation(line);
    printf("RPN: %s\n", rpn);

    Node *tree = make_tree(rpn);//make_tree надо переделать чтобы возвращала Node*

    printf("Дерево до:\n");
    print_tree(tree, 0);

    Node *result = distribute(tree);

    printf("Дерево после:\n");
    print_tree(result, 0);

    free(line);
    free(rpn);
    return 0;
}