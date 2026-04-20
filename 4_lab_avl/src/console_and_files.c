#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "avl_tree.h"

void process_file(const char* input_filename, const char* output_filename, Node** root) {
    FILE* in = fopen(input_filename, "r");
    if (!in) { printf("Ошибка открытия входного файла: %s\n", input_filename); return; }

    FILE* out = fopen(output_filename, "w");
    if (!out) { printf("Ошибка открытия выходного файла: %s\n", output_filename); fclose(in); return; }

    int op;
    char key[7];
    double val;

    while (fscanf(in, "%d", &op) == 1) {
        if (op == 1) {
            if (fscanf(in, "%6s %lf", key, &val) != 2) break;
            fprintf(out, "1 %s %.4f\n", key, val);
            if (search(*root, key) != NULL) {
                fprintf(out, "Ошибка: ключ '%s' уже существует\n\n", key);
            } else {
                *root = insert(*root, key, val);
                fprintf(out, "Узел добавлен: ключ=%s, значение=%.4f\n\n", key, val);
            }
        } else if (op == 2) {
            if (fscanf(in, "%6s", key) != 1) break;
            fprintf(out, "2 %s\n", key);
            if (search(*root, key) != NULL) {
                *root = remove_node(*root, key);
                fprintf(out, "Узел удалён: ключ=%s\n\n", key);
            } else {
                fprintf(out, "Ошибка: ключ '%s' не найден\n\n", key);
            }
        } else if (op == 3) {
            fprintf(out, "3\n");
            if (*root == NULL) {
                fprintf(out, "дерево пустое\n\n");
            } else {
                printTreeToFile(*root, 0, out);
                fprintf(out, "\n");
            }
        } else if (op == 4) {
            if (fscanf(in, "%6s", key) != 1) break;
            fprintf(out, "4 %s\n", key);
            Node* found = search(*root, key);
            if (found != NULL) {
                fprintf(out, "Найдено: ключ=%s, значение=%.4f\n\n", found->key, found->value);
            } else {
                fprintf(out, "Ключ '%s' не найден\n\n", key);
            }
        } else {
            fprintf(out, "Неизвестная команда: %d\n\n", op);
        }
    }

    fclose(in);
    fclose(out);
    printf("\nРезультаты записаны в файл: %s\n", output_filename);
}

void process_stdin(Node** root) {
    int op;
    char key[7];
    double val;
    char in_path[256];
    char out_path[256];

    printf(" 1 <ключ> <значение> — добавить\n");
    printf(" 2 <ключ>            — удалить\n");
    printf(" 3                   — печать дерева\n");
    printf(" 4 <ключ>            — поиск\n");
    printf(" 5                   — выполнить из файла\n");
    printf(" 0                   — выход\n");

    while (1) {
        printf("\nВведит команду: ");
        if (scanf("%d", &op) != 1) break;
        if (op == 0) break;

        switch (op) {
            case 1:
                if (scanf("%6s %lf", key, &val) == 2) {
                    if (search(*root, key) != NULL) {
                        printf("Ошибка: ключ '%s' уже существует\n", key);
                    } else {
                        *root = insert(*root, key, val);
                        printf("Узел добавлен: ключ=%s, значение=%.4f\n", key, val);
                    }
                }
                break;
            case 2:
                if (scanf("%6s", key) == 1) {
                    if (search(*root, key) != NULL) {
                        *root = remove_node(*root, key);
                        printf("Узел удалён: ключ=%s\n", key);
                    } else {
                        printf("Ошибка: ключ '%s' не найден\n", key);
                    }
                }
                break;
            case 3:
                if (*root == NULL) printf("дерево пустое\n");
                else printTree(*root, 0);
                break;
            case 4:
                if (scanf("%6s", key) == 1) {
                    Node* result = search(*root, key);
                    if (result != NULL)
                        printf("Найдено: ключ=%s, значение=%.4f\n", result->key, result->value);
                    else
                        printf("Ключ '%s' не найден\n", key);
                }
                break;
            case 5:
                printf("Путь к входному файлу: ");
                scanf("%255s", in_path);
                printf("Путь к выходному файлу: ");
                scanf("%255s", out_path);
                process_file(in_path, out_path, root);
                break;
            default:
                printf("Неизвестная команда. Введите число от 0 до 5.\n");
        }
    }
    printf("Выход.\n");
}

void free_tree(Node* node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}