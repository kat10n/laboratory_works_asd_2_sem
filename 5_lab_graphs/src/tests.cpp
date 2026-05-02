#include <gtest/gtest.h>
#include <cstdlib>

extern "C" {
#include "kruskal.h"
#include "struct.h"
}

// вспомогательная функция создаёт динамическую матрицу n×n из плоского массива
int** make_matrix(int n, std::initializer_list<int> values) {
    int** m = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        m[i] = (int*)calloc(n, sizeof(int));
    int i = 0;
    for (int v : values)
        m[i / n][i % n] = v, i++;
    return m;
}

void free_matrix(int** m, int n) {
    for (int i = 0; i < n; i++) free(m[i]);
    free(m);
}

TEST(FindTest, RootReturnsItself) {
    int parent[] = {0, 1, 2, 3};
    EXPECT_EQ(find(parent, 0), 0);
    EXPECT_EQ(find(parent, 3), 3);
}

TEST(FindTest, PathCompression) {
    int parent[] = {0, 0, 1}; // 2 -> 1 -> 0
    EXPECT_EQ(find(parent, 2), 0);
    EXPECT_EQ(parent[2], 0);
}

TEST(UniteTest, MergesTwoSets) {
    int parent[] = {0, 1, 2};
    int rank[]   = {0, 0, 0};
    EXPECT_EQ(unite(parent, rank, 0, 1), 1);
    EXPECT_EQ(find(parent, 0), find(parent, 1));
}

TEST(UniteTest, DetectsCycle) {
    int parent[] = {0, 1, 2};
    int rank[]   = {0, 0, 0};
    unite(parent, rank, 0, 1);
    EXPECT_EQ(unite(parent, rank, 0, 1), 0);
}

TEST(KruskalTest, SingleVertex) {
    int** m = make_matrix(1, {0});
    Edge* mst = (Edge*)malloc(0);
    EXPECT_EQ(kruskal(1, m, mst), 0);
    free_matrix(m, 1);
    free(mst);
}

TEST(KruskalTest, TwoVertices) {
    int** m = make_matrix(2, {0,5, 5,0});
    Edge* mst = (Edge*)malloc(sizeof(Edge));
    EXPECT_EQ(kruskal(2, m, mst), 5);
    EXPECT_EQ(mst[0].weight, 5);
    free_matrix(m, 2);
    free(mst);
}

TEST(KruskalTest, SimpleGraph4Nodes) {
    int** m = make_matrix(4, {
        0,1,0,4,
        1,0,2,0,
        0,2,0,3,
        4,0,3,0
    });
    Edge* mst = (Edge*)malloc(3 * sizeof(Edge));
    EXPECT_EQ(kruskal(4, m, mst), 6);
    free_matrix(m, 4);
    free(mst);
}

TEST(KruskalTest, ChoosesMinimumEdges) {
    int** m = make_matrix(3, {
        0,10,1,
        10,0,2,
        1,2,0
    });
    Edge* mst = (Edge*)malloc(2 * sizeof(Edge));
    EXPECT_EQ(kruskal(3, m, mst), 3);
    EXPECT_EQ(mst[0].weight, 1);
    EXPECT_EQ(mst[1].weight, 2);
    free_matrix(m, 3);
    free(mst);
}

TEST(KruskalTest, EdgeCountIsNMinus1) {
    int** m = make_matrix(5, {
        0,3,1,4,2,
        3,0,5,7,6,
        1,5,0,2,3,
        4,7,2,0,1,
        2,6,3,1,0
    });
    Edge* mst = (Edge*)malloc(4 * sizeof(Edge));
    kruskal(5, m, mst);
    int count = 0;
    for (int i = 0; i < 4; i++)
        if (mst[i].weight > 0) count++;
    EXPECT_EQ(count, 4);
    free_matrix(m, 5);
    free(mst);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}