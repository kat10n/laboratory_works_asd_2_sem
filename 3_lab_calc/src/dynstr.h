#ifndef LABORATORY_WORKS_ASD_2_SEM_DYSTR_H
#define LABORATORY_WORKS_ASD_2_SEM_DYSTR_H

typedef struct {
    char *data;
    int   len;
    int   cap;
} DynStr;

DynStr  dynstr_create(void);
void    dynstr_append(DynStr *s, const char *text);
void    dynstr_append_char(DynStr *s, char c);
char   *dynstr_take(DynStr *s);

#endif //LABORATORY_WORKS_ASD_2_SEM_DYSTR_H