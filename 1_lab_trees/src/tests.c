#include <assert.h>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "tree.h"

void setup_utf8() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

void test_empty_tree() {
    printf("Тест 1: Пустое дерево\n");
    int result = findMinDepthLeaf(NULL);
    assert(result == -1);
}

void test_single_node() {
    printf("Тест 2: Дерево с одним узлом\n");
    Node* root = createNode(42);
    int result = findMinDepthLeaf(root);
    assert(result == 42);
    removeSubtree(root);
}

void test_branching_tree() {
    printf("Тест 3: Разветвлённое дерево\n");
    Node* root = createNode(1);
    addChild(root, createNode(2));
    addChild(root, createNode(3));
    addChild(root->firstChild, createNode(4)); // глубина 2
    int result = findMinDepthLeaf(root);
    assert(result == 2 || result == 3); // листы на глубине 1
    removeSubtree(root);
}

void test_after_deletion() {
    printf("Тест 4: После удаления поддерева\n");
    Node* root = createNode(1);
    addChild(root, createNode(2));
    addChild(root, createNode(3));
    addChild(root->firstChild, createNode(4));
    // Удалим узел 2, теперь минимальный лист - 3
    Node* toDelete = root->firstChild;
    root->firstChild = toDelete->nextSibling;
    removeSubtree(toDelete);
    int result = findMinDepthLeaf(root);
    assert(result == 3);
    removeSubtree(root);
}

void test_delete_root() {
    printf("Тест 5: Удаление корня\n");
    Node* root = createNode(1);
    addChild(root, createNode(2));
    removeSubtree(root);
    root = NULL;
    int result = findMinDepthLeaf(root);
    assert(result == -1);
}

int main() {
    setup_utf8();
    test_empty_tree();
    test_single_node();
    test_branching_tree();
    test_after_deletion();
    test_delete_root();
    printf("Все тесты пройдены корректно");
    return 0;
}