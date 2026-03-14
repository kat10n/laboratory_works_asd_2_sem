#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

void read_file(char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл");
        return;
    }
    int n;
    if (fscanf(file, "%d", &n) != 1) {
        printf("Невозможно прочитать количество строк в файле");
        fclose(file);
        return;
    }

    Item *list = malloc(sizeof(Item) * n);
    if (list == NULL) {
        printf("Ошибка выделения памяти");
        fclose(file);
        return;
    }

    for (int i = 0; i < n; i++) {
        if (fscanf(file, "%d", &list[i].key) != 1) {
            printf("Ошибка чтения ключа для элемента %d", i);
            for (int j = 0; j < i; j++) {
                free(list[j].value);
            }
            free(list);
            fclose(file);
            return;
        }

        char *skip = NULL;
        size_t skip_len = 0;
        getline(&skip, &skip_len, file); // Пропускаем остаток строки после чтения ключа
        free(skip);

        // Читаем строку со значением
        char *line = NULL;
        size_t len = 0;
        int read = getline(&line, &len, file);
        if (read == -1) {
            printf("Ошибка чтения значения для элемента %d", i);
            free(line);
            for (int j = 0; j < i; j++) {
                free(list[j].value);
            }
            free(list);
            fclose(file);
            return;
        }

        if (read > 0 && line[read - 1] == '\n') { // Удаляем символ новой строки, если он есть
            line[read - 1] = '\0'; 
        }

        list[i].value = line;
    }

    bubble_sort(list, n);

    for (int i = 0; i < n; i++) {
        free(list[i].value);
    }
    free(list);
    fclose(file);
}