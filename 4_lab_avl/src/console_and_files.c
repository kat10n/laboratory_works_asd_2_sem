#include <stdio.h>
#include <stdlib.h>   /* BUGFIX: добавлен include для free() */
#include "struct.h"
#include "avl_tree.h"

void process_file(const char* filename, Node** root) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка открытия файла\n");
        return;
    }
    int op;
    char key[7];
    double val;

    while (fscanf(file, "%d", &op) != EOF) {
        if (op == 1) { // добавление
            fscanf(file, "%6s %lf", key, &val);
            *root = insert(*root, key, val);
        } else if (op == 2) { // удаление
            fscanf(file, "%6s", key);
            *root = remove_node(*root, key);
        } else if (op == 3) { // печать
            printTree(*root, 0);
        } else if (op == 4) { // поиск
            fscanf(file, "%6s", key);
            Node* found = search(*root, key);
            if (found != NULL) {
                printf("ключ: %s, значение: %f\n", found->key, found->value);
            } else {
                printf("ключ %s не найден\n", key);
            }
        } else {
            break;
        }
    }
    fclose(file);
}

void process_stdin(Node** root) {
    int op;
    char key[7];
    double val;

    printf("Введите команду: \n 1-добавить \n 2-удалить \n 3-печать \n 4-поиск \n 0-выход):\n");
    while (scanf("%d", &op) && op != 0) {
        switch(op) {
            case 1:
                printf("Ключ и значение: ");
                scanf("%6s %lf", key, &val);
                *root = insert(*root, key, val);
                printf("узел добавлен\n");
                break;
            case 2:
                printf("Ключ удаляемого элемента: ");
                scanf("%6s", key);
                if (search(*root, key) != NULL) {
                    *root = remove_node(*root, key);
                    printf("Узел удален\n");
                } else {
                    printf("ключ не найден\n");
                }
                break;
            case 3:
                printTree(*root, 0);
                break;
            case 4:
                printf("Ключ искомого элемента: ");
                scanf("%6s", key);
                {
                    Node* result = search(*root, key);
                    if (result != NULL) {
                        printf("Найдено: %f \n", result->value);
                    } else {
                        printf("ключ не найден\n");
                    }
                }
                break;
            case 0:
                break;
        }
        printf("Следующая команда: ");
    }
}

void log_result(const char* message, const char* key, double val, int found) {
    FILE* out = fopen("files/output.txt", "a");
    if (found) {
        fprintf(out, "%s: ключ=%s, значение=%.2f\n", message, key, val);
    } else {
        fprintf(out, "%s: ключ=%s не найден\n", message, key);
    }
    fclose(out);
}

void free_tree(Node* node) {
    if (node == NULL) {
        return;
    }
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}
