
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

void cli() {
    char command[8] = {0};
    int arg;
    Item *list = NULL;
    int list_size = 0;

    printf("Доступные команды:\n");
    printf("1       - отсортировать файл с упорядоченными элементами\n");
    printf("2       - отсортировать файл с элементами в обратном порядке\n");
    printf("3       - отсортировать файл с неупорядоченными элементами\n");
    printf("4 <key> - найти значение по ключу\n");
    printf("5       - выход\n");


    while (1) {
        printf("> ");
        if (scanf("%7s", command) != 1) break; // конец ввода

        if (strcmp(command, "5") == 0) {
            printf("Выход.\n");
            break;
        }
        else if (strcmp(command, "1") == 0 || strcmp(command, "2") == 0 || strcmp(command, "3") == 0) {
            // Освобождаем предыдущий массив, если был
            if (list != NULL) {
                for (int i = 0; i < list_size; i++) free(list[i].value);
                free(list);
                list = NULL;
                list_size = 0;
            }
            char *filename;
            if (strcmp(command, "1") == 0) {
                filename = "text_files/direct_order.txt";
            } else if (strcmp(command, "2") == 0) {
                filename = "text_files/reverse_order.txt";
            } else {
                filename = "text_files/random_order.txt";
            }
            list = read_file(filename, &list_size);
        }
        else if (strcmp(command, "4") == 0) {
            if (scanf("%d", &arg) != 1) {
                printf("Ошибка: команда требует одно число\n");
                while (getchar() != '\n');
            } else {
                if (list == NULL) {
                    printf("Сначала загрузите файл (команды 1-3)\n");
                } else {
                    Item *found = binary_search(list, 0, list_size - 1, arg);
                    if (found != NULL) {
                        printf("Найдено: %d | %s\n", found->key, found->value);
                    } else {
                        printf("Элемент с ключом %d не найден\n", arg);
                    }
                }
            }
        }
    }

    // Освобождаем память при выходе
    if (list != NULL) {
        for (int i = 0; i < list_size; i++) free(list[i].value);
        free(list);
    }
}
    // Освобождаем память при выходе
    if (list != NULL) {
        for (int i = 0; i < list_size; i++) free(list[i].value);
        free(list);
    }
}