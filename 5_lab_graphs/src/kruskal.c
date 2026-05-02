#include <stdlib.h>
#include "kruskal.h"

int find(int* parent, int i) {
    if (parent[i] == i)
        return i;
    return parent[i] = find(parent, parent[i]);
}

int unite(int* parent, int* rank, int x, int y) {
    int rootX = find(parent, x);
    int rootY = find(parent, y);
    if (rootX == rootY) return 0;
    if (rank[rootX] < rank[rootY])      parent[rootX] = rootY;
    else if (rank[rootX] > rank[rootY]) parent[rootY] = rootX;
    else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
    return 1;
}

int compare(const void* a, const void* b) {
    return ((Edge*)a)->weight - ((Edge*)b)->weight;
}

int kruskal(int n, int** matrix, Edge* mst) {
    int max_edges = n * (n - 1) / 2;
    Edge* edges = malloc(max_edges * sizeof(Edge));
    int edge_count = 0;

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (matrix[i][j] > 0) {
                edges[edge_count].u = i;
                edges[edge_count].v = j;
                edges[edge_count].weight = matrix[i][j];
                edge_count++;
            }

    qsort(edges, edge_count, sizeof(Edge), compare);

    int* parent = malloc(n * sizeof(int));
    int* rank   = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i]   = 0;
    }

    int mst_size = 0, total_weight = 0;
    for (int i = 0; i < edge_count && mst_size < n - 1; i++) {
        if (unite(parent, rank, edges[i].u, edges[i].v)) {
            mst[mst_size++] = edges[i];
            total_weight += edges[i].weight;
        }
    }

    free(edges);
    free(parent);
    free(rank);
    return total_weight;
}