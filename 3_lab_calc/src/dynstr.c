#include "dynstr.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DYNSTR_INIT_CAP 64

static void dynstr_grow(DynStr *s, int needed) {
    while (s->len + needed + 1 > s->cap) {
        s->cap *= 2;
        char *tmp = realloc(s->data, s->cap);
        if (!tmp) { printf("Ошибка выделения памяти\n"); return; }
        s->data = tmp;
    }
}

DynStr dynstr_create(void) {
    DynStr s;
    s.cap  = DYNSTR_INIT_CAP;
    s.len  = 0;
    s.data = malloc(s.cap);
    if (s.data) s.data[0] = '\0';
    return s;
}

void dynstr_append(DynStr *s, const char *text) {
    int slen = (int)strlen(text);
    dynstr_grow(s, slen);
    memcpy(s->data + s->len, text, slen + 1);
    s->len += slen;
}

void dynstr_append_char(DynStr *s, char c) {
    dynstr_grow(s, 1);
    s->data[s->len++] = c;
    s->data[s->len]   = '\0';
}

char *dynstr_take(DynStr *s) {
    char *result = s->data;
    s->data = NULL;
    s->len  = 0;
    s->cap  = 0;
    return result;
}