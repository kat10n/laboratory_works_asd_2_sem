#ifndef LABORATORY_WORKS_ASD_2_SEM_KRUSKAL_H
#define LABORATORY_WORKS_ASD_2_SEM_KRUSKAL_H

#include "struct.h"

int find(int* parent, int i);
int unite(int* parent, int* rank, int x, int y);
int compare(const void* a, const void* b);
int kruskal(int n, int** matrix, Edge* mst);

#endif //LABORATORY_WORKS_ASD_2_SEM_KRUSKAL_H