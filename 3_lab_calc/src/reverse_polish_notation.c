#include "struct.h"
#include "reverse_polish_notation.h"
#include "dynstr.h"
#include "stack.h"
#include "tokenize.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *create_node(const char *value) {
	Node *node = malloc(sizeof(Node));
	node->data = strdup(value);
	node->left = node->right = NULL;
	return node;
}

int is_operator(const char *token) {
	return token && strlen(token) == 1 && strchr("+-*/^%", token[0]) != NULL;
}

int priority(char op) {
	if (op == '+' || op == '-') return 0;
	if (op == '*' || op == '/' || op == '%') return 1;
	if (op == '^') return 2;
	return -1;
}

static void flush_op(Stack **stack, DynStr *buf) {
	if (buf->len > 0) dynstr_append(buf, " ");
	char op[2] = {stack_peek(*stack), '\0'};
	dynstr_append(buf, op);
	*stack = stack_pop(*stack);
}

char *reverse_polish_notation(char *line) {
	char **tokens = tokenize(line);
	Stack  *stack = stack_create();
	DynStr  buf   = dynstr_create();

	for (int i = 0; tokens[i] != NULL; i++) {
		char *tok = tokens[i];

		if (is_operator(tok)) {
			while (!stack_is_empty(stack) && stack_peek(stack) != '(' &&
			       priority(stack_peek(stack)) >= priority(tok[0]))
				flush_op(&stack, &buf);
			stack = stack_push(stack, tok[0]);

		} else if (tok[0] == '(') {
			stack = stack_push(stack, '(');

		} else if (tok[0] == ')') {
			while (!stack_is_empty(stack) && stack_peek(stack) != '(')
				flush_op(&stack, &buf);
			if (!stack_is_empty(stack))
				stack = stack_pop(stack); 

		} else {
			if (buf.len > 0) dynstr_append(&buf, " ");
			dynstr_append(&buf, tok);
		}
		free(tok);
	}
	free(tokens);

	while (!stack_is_empty(stack))
		flush_op(&stack, &buf);

	return dynstr_take(&buf);
}

Node *make_tree(char *rpn) {
	Stack2 *stack = stack2_create();
	char *token = strtok(rpn, " ");
	while (token != NULL) {
		Node *node = create_node(token);
		if (is_operator(token)) {
			node->right = stack2_peek(stack); stack = stack2_pop(stack);
			node->left  = stack2_peek(stack); stack = stack2_pop(stack);
		}
		stack = stack2_push(stack, node);
		token = strtok(NULL, " ");
	}
	return stack2_peek(stack);
}

void print_tree(Node *root, int depth) {
	if (!root) return;
	for (int i = 0; i < depth; i++) printf("  ");
	printf("%s\n", root->data);
	print_tree(root->right, depth + 1);
	print_tree(root->left,  depth + 1);
}