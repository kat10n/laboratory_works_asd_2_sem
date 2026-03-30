#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "struct.h"
#include "enter.h"
#include "reverse_polish_notation.h"
#include "mini_calculator.h"

void setup_utf8() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

void test_rpn_simple() {
    printf("Test 1: simple RPN\n");
    char input[] = "1+2";
    char *rpn = reverse_polish_notation(input);
    assert(strcmp(rpn, "1 2 +") == 0);
    free(rpn);
}

void test_rpn_priority() {
    printf("Test 2: multiplication priority\n");
    char input[] = "1+2*3";
    char *rpn = reverse_polish_notation(input);
    assert(strcmp(rpn, "1 2 3 * +") == 0);
    free(rpn);
}

void test_rpn_brackets() {
    printf("Test 3: brackets\n");
    char input[] = "7*(3+4)-1";
    char *rpn = reverse_polish_notation(input);
    assert(strcmp(rpn, "7 3 4 + * 1 -") == 0);
    free(rpn);
}

void test_validation_mixed() {
    printf("Test 4: validation\n");
    assert(validation("3a") == 0);
    assert(validation("3+a") == 1);
    assert(validation("(3+1") == 0);
    assert(validation("5+г") == 1);
}

void test_multiply_numbers() {
    char *result;

    printf("Test 5: multiply numeric chain\n");
    char input[] = "7*6*j*k*4";
    char *rpn = reverse_polish_notation(input);
    Node *tree = make_tree(rpn);
    tree = simplify_multiplication(tree);
    result = tree_to_infix(tree);

    assert(strcmp(result, "168*j*k") == 0);

    free(result);
    free_tree(tree);
    free(rpn);
}

void test_multiply_coeff_rule() {
    char *result;

    printf("Test 6: multiply coeff rule\n");
    char input[] = "10*k*2";
    char *rpn = reverse_polish_notation(input);
    Node *tree = make_tree(rpn);
    tree = simplify_multiplication(tree);
    result = tree_to_infix(tree);

    assert(strcmp(result, "20*k") == 0);

    free(result);
    free_tree(tree);
    free(rpn);
}

int main() {
    setup_utf8();
    test_rpn_simple();
    test_rpn_priority();
    test_rpn_brackets();
    test_validation_mixed();
    test_multiply_numbers();
    test_multiply_coeff_rule();

    printf("All tests passed!\n");
    return 0;
}
