#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

void read_file(char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл");
    }

    int lines = 0;
    int ch;
    int last_char = 0; // для отслеживания последнего символа

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
        last_char = ch;
    }

    // Если файл не пуст и последний символ не '\n', добавляем строку
    if (last_char != 0 && last_char != '\n') {
        lines++;
    }
    int n = 15;
    Item *list = malloc(sizeof(Item)*n);
    for (int i = 0; i <= n; i++) {
        fscanf(file, "%d %s", &list[i].key, list[i].value);
    }
    buble_sort(list, n);
    fclose(file);
    free(list);
}