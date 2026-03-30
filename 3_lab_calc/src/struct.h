#ifndef LABORATORY_WORKS_ASD_2_SEM_STRUCT_H
#define LABORATORY_WORKS_ASD_2_SEM_STRUCT_H
typedef struct Stack{
    char top;
    struct Stack *next;
    int size;
}Stack;

typedef struct Node{
	char *data;
    struct Node *right;
    struct Node *left;
}Node;

typedef struct Stack2{
    Node *top;
    struct Stack2 *next;
    int size;
}Stack2;
#endif //LABORATORY_WORKS_ASD_2_SEM_STRUCT_H