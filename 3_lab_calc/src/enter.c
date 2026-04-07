#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char* enter() {
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, stdin) == -1) {
        free(line);
        return NULL;
    }
    line[strcspn(line, "\n")] = '\0';
    return line;
}

int validation(char *line) {
    int i = 0, brackets = 0, numbers = 0, letters = 0;
    
    while (i < (int)strlen(line) && line[i] != '\n') {
        unsigned char symbol = (unsigned char)line[i]; //для isdigit и isalpha

        if (line[i] == ' ') {
            numbers = 0;
            letters = 0;
            i++;
            continue;
        }
        if (line[i] == '(') {
            brackets++;
            numbers = 0;
            letters = 0;
            i++;
            continue;
        }
        if (line[i] == ')') {
            brackets--;
            numbers = 0;
            letters = 0;
            i++;
            continue;
        }
        if (brackets < 0) return 0;

        if (strchr("+-*/^", line[i]) != NULL) {
            numbers = 0;
            letters = 0;
            i++;
            continue;
        }

        if (!(isdigit(symbol) || isalpha(symbol) || symbol >= 128)) return 0;

        int is_number = isdigit(symbol);
        int is_letter = isalpha(symbol) || symbol >= 128;
        if (is_number && letters) return 0;
        if (is_letter && numbers) return 0;
        if (is_number) numbers = 1; else numbers = 0;
        if (is_letter) letters = 1; else letters = 0;
        i++;
    }
    if (brackets != 0) return 0;
    return 1;
}