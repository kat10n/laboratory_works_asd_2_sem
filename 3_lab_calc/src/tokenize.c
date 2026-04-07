#include "struct.h"
#include <stdlib.h>
#include <string.h>

char **tokenize(char *line) {
    int cap = 16, j = 0;
    char **tokens = malloc(sizeof(char *) * cap);

    for (int i = 0; line[i]; ) {
        if (line[i] == ' ') { i++; continue; }

        if (j + 2 > cap)
            tokens = realloc(tokens, sizeof(char *) * (cap *= 2));

        if (strchr("+-*/^()", line[i])) {
            tokens[j] = malloc(2);
            tokens[j][0] = line[i++];
            tokens[j][1] = '\0';
        } else {
            int k = 0, tcap = 16;
            tokens[j] = malloc(tcap);
            while (line[i] && line[i] != ' ' && !strchr("+-*/^()", line[i])) {
                if (k + 2 > tcap)
                    tokens[j] = realloc(tokens[j], tcap *= 2);
                tokens[j][k++] = line[i++];
            }
            tokens[j][k] = '\0';
        }
        j++;
    }
    tokens[j] = NULL;
    return tokens;
}