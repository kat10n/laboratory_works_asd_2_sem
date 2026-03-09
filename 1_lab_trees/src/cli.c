#include <stdio.h>
#include <string.h>
#include "tree.h"


// Поиск узла по значению (возвращает указатель на первый найденный)
Node* findNode(Node* root, int value) {
    if (root == NULL) return NULL;
    if (root->data == value) return root;
    Node* found = findNode(root->firstChild, value);
    if (found) return found; // нашлось в потомках
    return findNode(root->nextSibling, value);
}

// Поиск родителя узла с заданным значением для удаления узла
Node* findParent(Node* root, int value, Node** child) {
    if (root == NULL) return NULL;
    Node* curr = root->firstChild;
    while (curr) {
        if (curr->data == value) {
            *child = curr;
            return root;
        }
        Node* res = findParent(curr, value, child);
        if (res) return res; // нашлось в потомках
        curr = curr->nextSibling;
    }
    return NULL;
}

// Добавление узла по значению родителя
void addNode(Node* root, int parentValue, int newValue) 
{
    Node* parent = findNode(root, parentValue);
    if (parent == NULL) {
        printf("Родитель со значением %d не найден.\n", parentValue);
        return;
    }
    Node* newNode = createNode(newValue);
    addChild(parent, newNode);
    printf("Узел %d добавлен как потомок %d\n", newValue, parentValue);
}

// Удаление узла по значению
void deleteNode(Node* root, int value) {
    if (root == NULL) {
        printf("Дерево пусто.\n");
        return;
    }
    // Случай удаления корня
    if ((root)->data == value) {
        removeSubtree(root);
        root = NULL;
        printf("Корень удалён. Дерево пусто.\n");
        return;
    }
    Node* parent = NULL;
    Node* toDelete = NULL;
    parent = findParent(root, value, &toDelete);
    if (parent == NULL) {
        printf("Узел со значением %d не найден.\n", value);
        return;
    }
    // Исключаем toDelete из списка детей parent
    if (parent->firstChild == toDelete) {
        parent->firstChild = toDelete->nextSibling;
    } else {
        Node* prev = parent->firstChild;
        while (prev->nextSibling != toDelete) {
            prev = prev->nextSibling;
        }
        prev->nextSibling = toDelete->nextSibling;
    }
    removeSubtree(toDelete);
    printf("Узел %d и его поддерево удалены.\n", value);
}

// Интерфейс командной строки
void cli(Node** root) {
    char command[128] = {0};
    int arg1, arg2;

    printf("Доступные команды:\n");
    printf("create <значение>         - создать корень дерева\n");
    printf("add <родитель> <значение> - добавить узел\n");
    printf("delete <значение>         - удалить узел и поддерево\n");
    printf("print                     - вывести дерево\n");
    printf("minleaf                   - лист минимальной глубины\n");
    printf("exit                      - выход\n");

    while (1) {
        printf("> ");
        if (scanf("%127s", command) != 1) break; // конец ввода

        if (strcmp(command, "exit") == 0) {
            printf("Выход.\n");
            break;
        }
        else if (strcmp(command, "create") == 0) {
            if (scanf("%d", &arg1) != 1) {
                printf("Ошибка: команда create требует одно число\n");
                while (getchar() != '\n');
            } else {
                if (*root != NULL) {
                    printf("Корень уже существует.\n");
                } else {
                    *root = createNode(arg1);
                    if (*root) printf("Создан корень %d\n", arg1);
                }
            }
        }
        else if (strcmp(command, "add") == 0) {
            if (scanf("%d %d", &arg1, &arg2) != 2) {
                printf("Ошибка: команда add требует два числа (родитель и потомок)\n");
                // очищаем буфер до конца строки на случай неправильного ввода
                while (getchar() != '\n');
            } else {
                addNode(root, arg1, arg2);
            }
        }
        else if (strcmp(command, "print") == 0) {
            if (*root == NULL) {
                printf("Дерево пусто.\n");
            } else {
                printf("Структура дерева:\n");
                printTree(*root, 0);
            }
        }
        else if (strcmp(command, "delete") == 0) {
            if (scanf("%d", &arg1) != 1) {
                printf("Ошибка: команда delete требует одно число\n");
                while (getchar() != '\n');
            } else {
                deleteNode(root, arg1);
            }
        }
        else if (strcmp(command, "minleaf") == 0) {
            if (*root == NULL) {
                printf("Дерево пусто.\n");
            } else {
                int val = findMinDepthLeaf(*root);
                if (val != -1) {
                    printf("Лист с минимальной глубиной: %d\n", val);
                }
            }
        }
        else {
            printf("Неизвестная команда. Доступные: add, print, delete, minleaf, exit\n");
        }
        // Очищаем остаток строки (на случай лишних символов после команды)
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}
