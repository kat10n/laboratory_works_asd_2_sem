#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char **tokenize(char *line) {
    char **tokens = malloc(sizeof(char*) * 100);
    int i = 0, j = 0;
    while (line[i] != '\0') {
        if (line[i] == ' ') {
            i++;
            continue;
        }
        if (strchr("+-*/^()", line[i]) != NULL) {
            tokens[j] = malloc(sizeof(char) * 2);
            tokens[j][0] = line[i];
            tokens[j][1] = '\0';
            j++;
            i++;
        } else if (isalnum(line[i])) {
            int k = 0;
            tokens[j] = malloc(sizeof(char) * 100);
            while (isalnum(line[i])) {
                tokens[j][k] = line[i];
                k++;
                i++;
            }
            tokens[j][k] = '\0';
            j++;
        } else {
            free(tokens);
            return NULL;
        }
    }
    tokens[j] = NULL;
    return tokens;
}