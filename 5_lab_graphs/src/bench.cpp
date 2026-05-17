#include <benchmark/benchmark.h>
#include <cstdlib>
#include <ctime>

extern "C" {
#include "kruskal.h"
#include "struct.h"
}

/* -------------------------------------------------------
 * kruskal_simple — аналог алгоритма из GraphKraskal (C#):
 * union без union-by-rank, только path compression в find.
 * ------------------------------------------------------- */
static int find_simple(int* parent, int i) {
    if (parent[i] != i)
        parent[i] = find_simple(parent, parent[i]);
    return parent[i];
}

static int kruskal_simple(int n, int** matrix, Edge* mst) {
    int max_edges = n * (n - 1) / 2;
    Edge* edges = (Edge*)malloc(max_edges * sizeof(Edge));
    int edge_count = 0;

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (matrix[i][j] > 0) {
                edges[edge_count].u = i;
                edges[edge_count].v = j;
                edges[edge_count].weight = matrix[i][j];
                edge_count++;
            }

    /* сортировка — как в C# через edges.Sort() */
    qsort(edges, edge_count, sizeof(Edge), [](const void* a, const void* b) {
        return ((Edge*)a)->weight - ((Edge*)b)->weight;
    });

    int* parent = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) parent[i] = i;

    int mst_size = 0, total_weight = 0;
    for (int i = 0; i < edge_count && mst_size < n - 1; i++) {
        int x = find_simple(parent, edges[i].u);
        int y = find_simple(parent, edges[i].v);
        if (x != y) {
            /* simple union: parent[x] = y, без rank */
            parent[x] = y;
            mst[mst_size++] = edges[i];
            total_weight += edges[i].weight;
        }
    }

    free(edges);
    free(parent);
    return total_weight;
}

/* -------------------------------------------------------
 * Генерация случайного связного графа (матрица смежности)
 * ------------------------------------------------------- */
static int** make_random_graph(int n) {
    int** m = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        m[i] = (int*)calloc(n, sizeof(int));
    }
    /* гарантируем связность: цепочка */
    for (int i = 0; i < n - 1; i++) {
        int w = rand() % 100 + 1;
        m[i][i + 1] = w;
        m[i + 1][i] = w;
    }
    /* добавляем случайные рёбра (~30% от максимума) */
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (m[i][j] == 0 && rand() % 10 < 3) {
                int w = rand() % 100 + 1;
                m[i][j] = w;
                m[j][i] = w;
            }
    return m;
}

static void free_matrix(int** m, int n) {
    for (int i = 0; i < n; i++) free(m[i]);
    free(m);
}

/* -------------------------------------------------------
 * Бенчмарки
 * ------------------------------------------------------- */

/* Наш Kruskal (union-by-rank + path compression) */
static void BM_KruskalMine(benchmark::State& state) {
    int n = state.range(0);
    srand(42);
    int** matrix = make_random_graph(n);
    Edge* mst = (Edge*)malloc((n - 1) * sizeof(Edge));

    for (auto _ : state) {
        benchmark::DoNotOptimize(kruskal(n, matrix, mst));
    }

    free(mst);
    free_matrix(matrix, n);
}

/* GraphKraskal-аналог (simple union без rank) */
static void BM_KruskalSimple(benchmark::State& state) {
    int n = state.range(0);
    srand(42);
    int** matrix = make_random_graph(n);
    Edge* mst = (Edge*)malloc((n - 1) * sizeof(Edge));

    for (auto _ : state) {
        benchmark::DoNotOptimize(kruskal_simple(n, matrix, mst));
    }

    free(mst);
    free_matrix(matrix, n);
}

/* Размеры графов для сравнения */
BENCHMARK(BM_KruskalMine)  ->Arg(10)->Arg(100)->Arg(500)->Arg(1000);
BENCHMARK(BM_KruskalSimple)->Arg(10)->Arg(100)->Arg(500)->Arg(1000);

BENCHMARK_MAIN();
