#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* enter() {
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, stdin);
    line[strcspn(line, "\n")] = '\0';
    return line;
}

int validation(char *line) {
    const char *POSSIBLE_SYMBOLS = "+-*/^0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM()";
    const char *NUMBERS = "1234567890";
    const char *LETTERS = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";

    int i = 0, brackets = 0, numbers = 0, letters = 0;
    
    while (i < (int)strlen(line) && line[i] != '\n') {
        if (line[i] == '(') brackets++;
        if (line[i] == ')') brackets--;
        if (brackets < 0) return 0;

        if (strchr(POSSIBLE_SYMBOLS, line[i]) == NULL) return 0;
        if (strchr(NUMBERS, line[i]) == NULL) numbers = 0;
        if (strchr(NUMBERS, line[i]) != NULL){
            numbers = 1;
            if (letters == 1) return 0;
        }
        if (strchr(LETTERS, line[i]) == NULL) letters = 0;
        if (strchr(LETTERS, line[i]) != NULL){
            letters = 1;
            if (numbers == 1) return 0;
        }
        i++;
    }
    if (brackets != 0) return 0;
    return 1;
}