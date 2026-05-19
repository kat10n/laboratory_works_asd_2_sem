#include <benchmark/benchmark.h>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>

extern "C" {
#include "kruskal.h"
#include "struct.h"
}
struct CSEdge { int u, v, w; };
// ─── общий генератор графа ────────────────────────────────────────────────

static int** make_random_graph(int n) {
    int** m = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        m[i] = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < n - 1; i++) {
        int w = rand() % 100 + 1;
        m[i][i+1] = m[i+1][i] = w;
    }
    for (int i = 0; i < n; i++)
        for (int j = i+1; j < n; j++)
            if (!m[i][j] && rand() % 10 < 3) {
                int w = rand() % 100 + 1;
                m[i][j] = m[j][i] = w;
            }
    return m;
}

static void free_matrix(int** m, int n) {
    for (int i = 0; i < n; i++) free(m[i]);
    free(m);
}

// ─── BM_Kruskal_C : твой kruskal.c ───────────────────────────────────────

static void BM_Kruskal_C(benchmark::State& state) {
    int n = state.range(0);
    srand(42);
    int** matrix = make_random_graph(n);
    Edge* mst = (Edge*)malloc((size_t)(n-1) * sizeof(Edge));

    for (auto _ : state)
        benchmark::DoNotOptimize(kruskal(n, matrix, mst));

    free(mst);
    free_matrix(matrix, n);
}

// ─── C# Kruskal (GraphKraskal) — реализация зеркально с GitHub ───────────
//     Union-Find + сортировка рёбер, как в типичном C# Kruskal
// edges строятся один раз при создании графа (как в Graph.cs конструкторе)
static std::vector<CSEdge> build_edges(int n, int** matrix) {
    std::vector<CSEdge> edges;
    for (int i = 0; i < n; i++)
        for (int j = i+1; j < n; j++)
            if (matrix[i][j])
                edges.push_back({i, j, matrix[i][j]});
    return edges;
}

// без rank, как в Graph.cs
static int cs_find(std::vector<int>& parent, int x) {
    if (parent[x] != x) parent[x] = cs_find(parent, parent[x]);
    return parent[x];
}

static void cs_union(std::vector<int>& parent, int a, int b) {
    parent[cs_find(parent, a)] = cs_find(parent, b); // точно как в C#
}

static int cs_kruskal(int n, const std::vector<CSEdge>& edges) {
    // копируем и сортируем на каждый вызов — как new List<Edge>(edges) + Sort()
    std::vector<CSEdge> sorted = edges;
    std::sort(sorted.begin(), sorted.end(),
              [](const CSEdge& a, const CSEdge& b){ return a.w < b.w; });

    std::vector<int> parent(n);
    for (int i = 0; i < n; i++) parent[i] = i;

    int total = 0, count = 0;
    for (auto& e : sorted) {
        if (count == n - 1) break;
        int x = cs_find(parent, e.u);
        int y = cs_find(parent, e.v);
        if (x != y) {
            cs_union(parent, e.u, e.v);
            total += e.w;
            count++;
        }
    }
    return total;
}

static void BM_Kruskal_CS(benchmark::State& state) {
    int n = state.range(0);
    srand(42);
    int** matrix = make_random_graph(n);
    // edges строятся один раз (как Graph g = new Graph(n) + AddEdge)
    std::vector<CSEdge> edges = build_edges(n, matrix);

    for (auto _ : state)
        benchmark::DoNotOptimize(cs_kruskal(n, edges));

    free_matrix(matrix, n);
}

// ─── регистрация ──────────────────────────────────────────────────────────

BENCHMARK(BM_Kruskal_C) ->Arg(10)->Arg(100)->Arg(500)->Arg(1000);
BENCHMARK(BM_Kruskal_CS)->Arg(10)->Arg(100)->Arg(500)->Arg(1000);

BENCHMARK_MAIN();