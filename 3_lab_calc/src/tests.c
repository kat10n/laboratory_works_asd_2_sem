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

static char *process(char *expr) {
    char *rpn  = reverse_polish_notation(expr);
    Node *tree = make_tree(rpn);
    tree = expand_brackets(tree);
    tree = simplify_multiplication(tree);
    char *result = tree_to_infix(tree);
    free_tree(tree);
    free(rpn);
    return result;
}

void test_rpn_simple() {
    char input[] = "1+2";
    char *rpn = reverse_polish_notation(input);
    assert(strcmp(rpn, "1 2 +") == 0);
    free(rpn);
}

void test_rpn_priority() {
    char input[] = "1+2*3";
    char *rpn = reverse_polish_notation(input);
    assert(strcmp(rpn, "1 2 3 * +") == 0);
    free(rpn);
}

void test_rpn_brackets() {
    char input[] = "7*(3+4)-1";
    char *rpn = reverse_polish_notation(input);
    assert(strcmp(rpn, "7 3 4 + * 1 -") == 0);
    free(rpn);
}

void test_rpn_nested_brackets() {
    char input[] = "(a+b)*(c+d)";
    char *rpn = reverse_polish_notation(input);
    assert(strcmp(rpn, "a b + c d + *") == 0);
    free(rpn);
}

void test_rpn_left_assoc() {
    char input[] = "a+b+c";
    char *rpn = reverse_polish_notation(input);
    assert(strcmp(rpn, "a b + c +") == 0);
    free(rpn);
}

void test_validation() {
    assert(validation("3a")    == 0);
    assert(validation("3+a")   == 1);
    assert(validation("(3+1")  == 0);
    assert(validation("3+1)")  == 0);
    assert(validation("a*b+c") == 1);
    assert(validation("")      == 1);
}

void test_simplify_chain() {
    char *r = process("7*6*j*k*4");
    assert(strcmp(r, "168*j*k") == 0);
    free(r);
}

void test_simplify_pure_nums() {
    char *r = process("3*4*5");
    assert(strcmp(r, "60") == 0);
    free(r);
}

void test_simplify_single_var() {
    char *r = process("1*x*1");
    assert(strcmp(r, "x") == 0);
    free(r);
}

void test_expand_right() {
    char *r = process("a*(b+c)");
    assert(strcmp(r, "a*b+a*c") == 0);
    free(r);
}

void test_expand_left() {
    char *r = process("(a+b)*c");
    assert(strcmp(r, "a*c+b*c") == 0);
    free(r);
}

void test_expand_subtract() {
    char *r = process("(a-b)*c");
    assert(strcmp(r, "a*c-b*c") == 0);
    free(r);
}

void test_expand_with_nums() {
    char *r = process("2*(a+3)");
    assert(strcmp(r, "2*a+6") == 0);
    free(r);
}

void test_expand_coeff() {
    char *r = process("3*(2*a+b)");
    assert(strcmp(r, "6*a+3*b") == 0);
    free(r);
}

int main() {
    setup_utf8();

    test_rpn_simple();
    test_rpn_priority();
    test_rpn_brackets();
    test_rpn_nested_brackets();
    test_rpn_left_assoc();
    test_validation();
    test_simplify_chain();
    test_simplify_pure_nums();
    test_simplify_single_var();
    test_expand_right();
    test_expand_left();
    test_expand_subtract();
    test_expand_with_nums();
    test_expand_coeff();
    printf("Все тесты работают!\n");
    return 0;
}