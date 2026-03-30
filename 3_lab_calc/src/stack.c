#include "struct.h"
#include <stdlib.h>
#include <stdio.h>


Stack* stack_create() {
    return NULL;
}

int stack_size(Stack *stack) {
    if (stack == NULL) return 0;
    return stack->size;
}

int stack_is_empty(Stack *stack) {
    return stack == NULL;
}

Stack* stack_push(Stack *stack, char value) {
    Stack *newNode = (Stack*)malloc(sizeof(Stack));
    if (newNode == NULL) {
        printf("Ошибка выделения памяти\n");
        return stack;
    }
    newNode->top = value;
    newNode->next = stack;
    newNode->size = stack_size(stack) + 1;
    return newNode;
}

Stack* stack_pop(Stack *stack) {
    if (stack_is_empty(stack)) {
       printf("Стек пуст\n");
       return NULL;
    }
    Stack *newTop = stack->next;
    free(stack);
    return newTop;
}

char stack_peek(Stack *stack) {
    if (stack_is_empty(stack)) {
       printf("Стек пуст\n");
       return '\0';
    }
    return stack->top;
}


Stack* stack2_create() {
    return NULL;
}

int stack2_size(Stack2 *stack) {
    if (stack == NULL) return 0;
    return stack->size;
}

int stack2_is_empty(Stack2 *stack) {
    return stack == NULL;
}

Stack2* stack2_push(Stack2 *stack, Node *value) {
    Stack2 *newNode = (Stack2*)malloc(sizeof(Stack2));
    if (newNode == NULL) {
        printf("Ошибка выделения памяти\n");
        return stack;
    }
    newNode->top = value;
    newNode->next = stack;
    newNode->size = stack2_size(stack) + 1;
    return newNode;
}

Stack2* stack2_pop(Stack2 *stack) {
    if (stack2_is_empty(stack)) {
       printf("Стек пуст\n");
       return NULL;
    }
    Stack2 *newTop = stack->next;
    free(stack);
    return newTop;
}

Node* stack2_peek(Stack2 *stack) {
    if (stack2_is_empty(stack)) {
       printf("Стек пуст\n");
       return NULL;
    }
    return stack->top;
}