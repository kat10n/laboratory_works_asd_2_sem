#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char **tokenize(char *line) {
    int tok_arr_cap = 16;
    char **tokens = malloc(sizeof(char *) * tok_arr_cap);
    if (!tokens) {
        printf("Ошибка выделения памяти\n");
        return NULL;
    }

    int i = 0, j = 0;
    while (line[i] != '\0') {
        if (line[i] == ' ') {
            i++;
            continue;
        }
        if (j + 2 > tok_arr_cap) {
            tok_arr_cap *= 2;
            char **tmp = realloc(tokens, sizeof(char *) * tok_arr_cap);
            if (!tmp) {
                printf("Ошибка выделения памяти\n");
                tokens[j] = NULL;
                return tokens;
            }
            tokens = tmp;
        }

        if (strchr("+-*/^()", line[i]) != NULL) {
            tokens[j] = malloc(sizeof(char) * 2);
            if (!tokens[j]) {
                printf("Ошибка выделения памяти\n");
                tokens[j] = NULL;
                return tokens;
            }
            tokens[j][0] = line[i];
            tokens[j][1] = '\0';
            j++;
            i++;
        } else {
            int tok_cap = 16, k = 0;
            tokens[j] = malloc(sizeof(char) * tok_cap);
            if (!tokens[j]) {
                printf("Ошибка выделения памяти\n");
                tokens[j] = NULL;
                return tokens;
            }
            while (line[i] != '\0' && line[i] != ' ' &&
                   strchr("+-*/^()", line[i]) == NULL) {
                /* +2: место под текущий символ и под '\0' */
                if (k + 2 > tok_cap) {
                    tok_cap *= 2;
                    char *tmp = realloc(tokens[j], sizeof(char) * tok_cap);
                    if (!tmp) {
                        printf("Ошибка выделения памяти\n");
                        break;
                    }
                    tokens[j] = tmp;
                }
                tokens[j][k++] = line[i++];
            }
            tokens[j][k] = '\0';
            j++;
        }
    }

    tokens[j] = NULL; /* sentinel */
    return tokens;
}