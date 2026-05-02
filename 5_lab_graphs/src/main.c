#include <stdio.h>
#include <stdlib.h>
#include "kruskal.h"

#ifdef _WIN32
#include <windows.h>
void setup_utf8() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}
#endif

int main() {
#ifdef _WIN32
    setup_utf8();
#endif

    FILE* file = fopen("filestxt/input.txt", "r");
    if (!file) {
        printf("Файл не найден\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);

    int** matrix = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
            fscanf(file, "%d", &matrix[i][j]);
    }
    fclose(file);

    Edge* mst = malloc((n - 1) * sizeof(Edge));
    int total_weight = kruskal(n, matrix, mst);

    printf("Ребра минимального остовного дерева:\n");
    for (int i = 0; i < n - 1; i++)
        printf("Вершина %d - Вершина %d (вес: %d)\n", mst[i].u, mst[i].v, mst[i].weight);
    printf("Общий вес остова: %d\n", total_weight);

    for (int i = 0; i < n; i++)
        free(matrix[i]);
    free(matrix);
    free(mst);
    return 0;
}