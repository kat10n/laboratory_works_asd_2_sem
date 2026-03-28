#include <gtest/gtest.h>
#include <cstdlib>
#include <cstring>

extern "C" {
    #include "../src/struct.h"
    #include "../src/enter.h"
    #include "../src/reverse_polish_notation.h"
    #include "../src/mini_calculator.h"
}

/* ── Вспомогательные функции ── */
static void free_tree(Node *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

static void tree_to_infix(Node *root, char *buf, int *pos) {
    if (!root) return;
    int is_op = (root->left || root->right);
    if (is_op) buf[(*pos)++] = '(';
    tree_to_infix(root->left,  buf, pos);
    buf[(*pos)++] = root->data;
    tree_to_infix(root->right, buf, pos);
    if (is_op) buf[(*pos)++] = ')';
    buf[*pos] = '\0';
}

/* ══════════════ RPN ══════════════ */

TEST(RPN, SimpleAddition) {
    char input[] = "1+2";
    char *rpn = reverse_polish_notation(input);
    EXPECT_STREQ(rpn, "12+");
    free(rpn);
}

TEST(RPN, PriorityMultiply) {
    char input[] = "1+2*3";
    char *rpn = reverse_polish_notation(input);
    EXPECT_STREQ(rpn, "123*+");
    free(rpn);
}

TEST(RPN, Brackets) {
    char input[] = "7*(3+4)-1";
    char *rpn = reverse_polish_notation(input);
    EXPECT_STREQ(rpn, "734+*1-");
    free(rpn);
}

TEST(RPN, ChainSamePriority) {
    char input[] = "1+2+3";
    char *rpn = reverse_polish_notation(input);
    EXPECT_STREQ(rpn, "12+3+");
    free(rpn);
}

/* ══════════════ Validation ══════════════ */

TEST(Validation, Valid) {
    EXPECT_EQ(validation((char*)"3+a"),    1);
    EXPECT_EQ(validation((char*)"7*(x+1)"),1);
}

TEST(Validation, MixedToken) {
    EXPECT_EQ(validation((char*)"3a"),  0);
}

TEST(Validation, UnbalancedBrackets) {
    EXPECT_EQ(validation((char*)"(3+1"), 0);
    EXPECT_EQ(validation((char*)"3+1)"), 0);
}

/* ══════════════ Distribute ══════════════ */

TEST(Distribute, SimpleRight) {
    // 2*(x+1)  →  ((2*x)+(2*1))
    char *rpn = reverse_polish_notation((char*)"2*(x+1)");
    Node *tree   = make_tree(rpn);
    Node *result = distribute(tree);

    char buf[256] = {};
    int  pos = 0;
    tree_to_infix(result, buf, &pos);
    EXPECT_STREQ(buf, "((2*x)+(2*1))");

    free_tree(result);
    free(rpn);
}

TEST(Distribute, SimpleLeft) {
    // (a+b)*3  →  ((a*3)+(b*3))
    char *rpn = reverse_polish_notation((char*)"(a+b)*3");
    Node *tree   = make_tree(rpn);
    Node *result = distribute(tree);

    char buf[256] = {};
    int  pos = 0;
    tree_to_infix(result, buf, &pos);
    EXPECT_STREQ(buf, "((a*3)+(b*3))");

    free_tree(result);
    free(rpn);
}

TEST(Distribute, NoDistribution) {
    // a*b — раскрывать нечего
    char *rpn = reverse_polish_notation((char*)"a*b");
    Node *tree   = make_tree(rpn);
    Node *result = distribute(tree);

    char buf[256] = {};
    int  pos = 0;
    tree_to_infix(result, buf, &pos);
    EXPECT_STREQ(buf, "(a*b)");

    free_tree(result);
    free(rpn);
}
