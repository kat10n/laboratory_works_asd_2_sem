#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "tokenize.h"

static char *duplicate_string(const char *value) {
	char *copy = malloc(strlen(value) + 1);
	if (copy == NULL) {
		return NULL;
	}
	strcpy(copy, value);
	return copy;
}

Node* create_node(const char *value) {
	Node *node = malloc(sizeof(Node));
	if (node == NULL) {
		return NULL;
	}
	node->data = duplicate_string(value);
	node->left = NULL;
	node->right = NULL;
	return node;
}

static int is_operator_token(const char *token) {
	return token != NULL && strlen(token) == 1 && strchr("+-*/^%", token[0]) != NULL;
}

static void free_tokens(char **tokens) {
	int i;
	if (tokens == NULL) {
		return;
	}
	for (i = 0; tokens[i] != NULL; i++) {
		free(tokens[i]);
	}
	free(tokens);
}

static void append_token(char **buffer, int *length, const char *token) {
	int token_length = (int)strlen(token);
	int extra = *length == 0 ? token_length : token_length + 1;
	*buffer = realloc(*buffer, (*length + extra + 1) * sizeof(char));
	if (*length != 0) {
		(*buffer)[*length] = ' ';
		memcpy(*buffer + *length + 1, token, token_length + 1);
		*length += token_length + 1;
	} else {
		memcpy(*buffer, token, token_length + 1);
		*length = token_length;
	}
}

static void append_char_token(char **buffer, int *length, char c) {
	char op[2] = {c, '\0'};
	append_token(buffer, length, op);
}

int priority(char operator) {
	if (operator == '+' || operator == '-') return 0;
	if (operator == '*' || operator == '/' || operator == '%') return 1;
	if (operator == '^') return 2;
	return -1;
}


char* reverse_polish_notation(char *line) {
	char **tokens = tokenize(line);
	int i = 0;
	int size = 0;
	char *array = NULL;
	Stack *stack = stack_create();
	while (tokens != NULL && tokens[i] != NULL) {
		if (is_operator_token(tokens[i])) {
			while (!stack_is_empty(stack) && stack_peek(stack) != '(' &&
			       priority(stack_peek(stack)) >= priority(tokens[i][0])) {
				append_char_token(&array, &size, stack_peek(stack));
				stack = stack_pop(stack);
			}
			stack = stack_push(stack, tokens[i][0]);
		} else if (strcmp(tokens[i], "(") == 0) {
			stack = stack_push(stack, '(');
		} else if (strcmp(tokens[i], ")") == 0) {
			while (!stack_is_empty(stack) && stack_peek(stack) != '(') {
				append_char_token(&array, &size, stack_peek(stack));
				stack = stack_pop(stack);
			}
			if (!stack_is_empty(stack) && stack_peek(stack) == '(') {
				stack = stack_pop(stack);
			}
		} else {
			append_token(&array, &size, tokens[i]);
		}
		i++;
	}

	while (!stack_is_empty(stack)) {
		append_char_token(&array, &size, stack_peek(stack));
		stack = stack_pop(stack);
	}
	if (array == NULL) {
		array = malloc(1);
		array[0] = '\0';
	}
	free_tokens(tokens);
	return array;
}


Node* make_tree(char *line) {
	char *token = strtok(line, " ");
	Stack2 *stack = stack2_create();
	while (token != NULL) {
		if (is_operator_token(token)) {
			Node *element = create_node(token);
			element->right = stack2_peek(stack);
			stack = stack2_pop(stack);
			element->left = stack2_peek(stack);
			stack = stack2_pop(stack);
			stack = stack2_push(stack, element);
		} else {
			Node *element = create_node(token);
			stack = stack2_push(stack, element);
		}
		token = strtok(NULL, " ");
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
	printf("%s\n", root->data);
    print_tree(root->right, depth + 1);
    print_tree(root->left, depth +1);
}