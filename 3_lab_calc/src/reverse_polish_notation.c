#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stack.h"

int priority(char operator) {
	if (operator == '+' || operator == '-') return 0;
	if (operator == '*' || operator == '/' || operator == '%') return 1;
	if (operator == '^') return 2;
	return -1;
}


char* reverse_polish_notation(char *line) {
    const char *POSSIBLE_OPERATORS = "+-*/^%";
    int i = 0;
    int size = 0;
    char *array = NULL;
    Stack *stack = stack_create();
    while (line[i] != '\0') {
        if (strchr(POSSIBLE_OPERATORS, line[i]) != NULL) {
			while (!stack_is_empty(stack) && stack_peek(stack) != '(' &&
					priority(stack_peek(stack)) >= priority(line[i])) {
            		size++;
                	array = realloc(array, (size + 1) * sizeof(char));
					array[size] = '\0';
                	array[size - 1] = stack_peek(stack);
				stack = stack_pop(stack);
				stack = stack_push(stack, line[i]);
			}
        	stack = stack_push(stack, line[i]);
        } else if (isalnum(line[i])) {
            size++;
            array = realloc(array, (size + 1) * sizeof(char));
			array[size] = '\0';
            array[size - 1] = line[i];
        } else if (isspace(line[i])) {
            i++;
            continue;
        } else if (line[i] == '(') {
            stack = stack_push(stack, line[i]);
        } else if (line[i] == ')') {
            while (!stack_is_empty(stack) && stack_peek(stack) != '(') {
                size++;
                array = realloc(array, (size + 1) * sizeof(char));
				array[size] = '\0';
                array[size - 1] = stack_peek(stack);
                stack = stack_pop(stack);
            }
			if (!stack_is_empty(stack) && stack_peek(stack) == '(')
				stack = stack_pop(stack);
        }
        i++;
	}
	while (!stack_is_empty(stack)) {
		size++;
    	array = realloc(array, (size + 1) * sizeof(char));
	    array[size] = '\0';
    	array[size - 1] = stack_peek(stack);
		stack = stack_pop(stack);
	}
    array[size] = '\0';
    return array;
}


Node* make_tree(char *line) {
	const char *POSSIBLE_OPERATORS = "+-*/^%";
    int i = 0;
	Stack2 *stack = stack2_create();
	while (i < (int)strlen(line)) {
        if (isalnum(line[i])) {
    		Node *element = malloc(sizeof(Node));
			element->data = line[i];
			element->left = NULL;
			element->right = NULL;
            stack = stack2_push(stack, element);
		} else if (strchr(POSSIBLE_OPERATORS, line[i]) != NULL) {
			Node *element = malloc(sizeof(Node));
			element->data = line[i];
			element->right = stack2_peek(stack);
			stack = stack2_pop(stack);
			element->left = stack2_peek(stack);
			stack = stack2_pop(stack);
            stack = stack2_push(stack, element);
		}
		i++;
	}
	return stack2_peek(stack);
}


void print_tree(Node* root, int depth) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("%c\n", root->data);
    print_tree(root->right, depth + 1);
    print_tree(root->left, depth +1);
}