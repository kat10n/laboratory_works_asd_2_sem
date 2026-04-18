#ifndef LABORATORY_WORKS_ASD_2_SEM_STRUCT_H
#define LABORATORY_WORKS_ASD_2_SEM_STRUCT_H

typedef struct Node {
    char key[7];
    double value;
    int height;
    struct Node* left;
    struct Node* right;
} Node;

#endif //LABORATORY_WORKS_ASD_2_SEM_STRUCT_H