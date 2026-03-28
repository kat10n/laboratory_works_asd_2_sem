#ifndef LABORATORY_WORKS_ASD_2_SEM_STACK_H
#define LABORATORY_WORKS_ASD_2_SEM_STACK_H
#include "struct.h"
Stack* stack_create();
int stack_size(Stack *stack);
int stack_is_empty(Stack *stack);
Stack* stack_push(Stack *stack, char value);
Stack* stack_pop(Stack *stack);
char stack_peek(Stack *stack);
Stack2* stack2_create();
int stack2_size(Stack2 *stack);
int stack2_is_empty(Stack2 *stack);
Stack2* stack2_push(Stack2 *stack, Node *value);
Stack2* stack2_pop(Stack2 *stack);
Node* stack2_peek(Stack2 *stack);
#endif //LABORATORY_WORKS_ASD_2_SEM_STACK_H