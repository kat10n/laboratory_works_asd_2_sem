#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "avl_tree.h"
#include "console_and_files.h"
#ifdef _WIN32
#include <windows.h>
#endif

void setup_utf8() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

static int abs_val(int x) { return x < 0 ? -x : x; }

static void assert_balanced(Node* node) {
    if (node == NULL) return;
    int bf = bfactor(node);
    assert(abs_val(bf) <= 1);
    assert_balanced(node->left);
    assert_balanced(node->right);
}

void test_insert_single() {
    Node* root = NULL;
    root = insert(root, "abc", 1.5);
    assert(root != NULL);
    assert(strcmp(root->key, "abc") == 0);
    assert(root->value == 1.5);
    free_tree(root);
}

void test_search_existing() {
    Node* root = NULL;
    root = insert(root, "abc", 1.0);
    root = insert(root, "xyz", 2.0);
    root = insert(root, "mno", 3.0);

    Node* found = search(root, "xyz");
    assert(found != NULL);
    assert(strcmp(found->key, "xyz") == 0);
    assert(found->value == 2.0);
    free_tree(root);
}

void test_search_missing() {
    Node* root = NULL;
    root = insert(root, "aaa", 1.0);
    root = insert(root, "bbb", 2.0);

    Node* found = search(root, "zzz");
    assert(found == NULL);
    free_tree(root);
}

void test_search_empty_tree() {
    Node* root = NULL;
    assert(search(root, "abc") == NULL);
}

void test_insert_duplicate_key() {
    Node* root = NULL;
    root = insert(root, "abc", 1.0);
    root = insert(root, "abc", 2.0);
    Node* found = search(root, "abc");
    assert(found != NULL);
    assert(found->value == 1.0);
    free_tree(root);
}

void test_balance_right_right() {
    Node* root = NULL;
    root = insert(root, "aaa", 1.0);
    root = insert(root, "bbb", 2.0);
    root = insert(root, "ccc", 3.0);
    assert_balanced(root);
    assert(strcmp(root->key, "bbb") == 0);
    free_tree(root);
}

void test_balance_left_left() {
    Node* root = NULL;
    root = insert(root, "ccc", 3.0);
    root = insert(root, "bbb", 2.0);
    root = insert(root, "aaa", 1.0);
    assert_balanced(root);
    assert(strcmp(root->key, "bbb") == 0);
    free_tree(root);
}

void test_balance_left_right() {
    Node* root = NULL;
    root = insert(root, "ccc", 3.0);
    root = insert(root, "aaa", 1.0);
    root = insert(root, "bbb", 2.0); 
    assert_balanced(root);
    assert(strcmp(root->key, "bbb") == 0);
    free_tree(root);
}

void test_balance_right_left() {
    Node* root = NULL;
    root = insert(root, "aaa", 1.0);
    root = insert(root, "ccc", 3.0);
    root = insert(root, "bbb", 2.0);
    assert_balanced(root);
    assert(strcmp(root->key, "bbb") == 0);
    free_tree(root);
}

void test_balance_many_inserts() {
    Node* root = NULL;
    const char* keys[] = {"g", "b", "m", "a", "d", "j", "p", "c", "f", "i", "l"};
    int n = 11;
    for (int i = 0; i < n; i++) {
        root = insert(root, (char*)keys[i], (double)i);
    }
    assert_balanced(root);
    for (int i = 0; i < n; i++) {
        assert(search(root, (char*)keys[i]) != NULL);
    }
    free_tree(root);
}

void test_remove_leaf() {
    Node* root = NULL;
    root = insert(root, "bbb", 2.0);
    root = insert(root, "aaa", 1.0);
    root = insert(root, "ccc", 3.0);

    root = remove_node(root, "aaa");
    assert(search(root, "aaa") == NULL);
    assert(search(root, "bbb") != NULL);
    assert(search(root, "ccc") != NULL);
    assert_balanced(root);
    free_tree(root);
}

void test_remove_root() {
    Node* root = NULL;
    root = insert(root, "bbb", 2.0);
    root = insert(root, "aaa", 1.0);
    root = insert(root, "ccc", 3.0);

    root = remove_node(root, "bbb");
    assert(search(root, "bbb") == NULL);
    assert(search(root, "aaa") != NULL);
    assert(search(root, "ccc") != NULL);
    assert_balanced(root);
    free_tree(root);
}

void test_remove_missing() {
    Node* root = NULL;
    root = insert(root, "aaa", 1.0);
    root = remove_node(root, "zzz");
    assert(search(root, "aaa") != NULL);
    free_tree(root);
}

void test_remove_all() {
    Node* root = NULL;
    root = insert(root, "aaa", 1.0);
    root = insert(root, "bbb", 2.0);
    root = insert(root, "ccc", 3.0);

    root = remove_node(root, "aaa");
    root = remove_node(root, "bbb");
    root = remove_node(root, "ccc");
    assert(root == NULL);
}

void test_remove_rebalance() {
    Node* root = NULL;
    const char* keys[] = {"d", "b", "f", "a", "c", "e", "g"};
    int n = 7;
    for (int i = 0; i < n; i++) {
        root = insert(root, (char*)keys[i], (double)i);
    }
    root = remove_node(root, "a");
    root = remove_node(root, "c");
    assert_balanced(root);
    free_tree(root);
}

int main() {
    setup_utf8();
    test_insert_single();
    test_search_existing();
    test_search_missing();
    test_search_empty_tree();
    test_insert_duplicate_key();

    test_balance_right_right();
    test_balance_left_left();
    test_balance_left_right();
    test_balance_right_left();
    test_balance_many_inserts();

    test_remove_leaf();
    test_remove_root();
    test_remove_missing();
    test_remove_all();
    test_remove_rebalance();

    printf("Все тесты пройдены!\n");
    return 0;
}