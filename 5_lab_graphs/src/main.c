#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kruskal.h"

#ifdef _WIN32
#include <windows.h>
void setup_utf8() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}
#endif

void print_usage(const char* prog) {
    printf("Использование: %s -f <файл> [-o <файл>]\n", prog);
    printf("Опции:\n");
    printf("  -f <файл>   входной файл с матрицей смежности\n");
    printf("  -o <файл>   сохранить результат в файл (по умолчанию вывод в терминал)\n");
    printf("  -h          показать эту справку\n");
    printf("\nФормат входного файла:\n");
    printf("  Первая строка: N (число вершин)\n");
    printf("  Далее N строк: матрица смежности N x N\n");
    printf("  0 означает отсутствие ребра\n");
}

int** read_matrix(FILE* file, int* n) {
    fscanf(file, "%d", n);
    int** matrix = malloc(*n * sizeof(int*));
    for (int i = 0; i < *n; i++) {
        matrix[i] = malloc(*n * sizeof(int));
        for (int j = 0; j < *n; j++)
            fscanf(file, "%d", &matrix[i][j]);
    }
    return matrix;
}

void print_result(FILE* out, Edge* mst, int n, int total_weight) {
    fprintf(out, "Рёбра минимального остовного дерева:\n");
    for (int i = 0; i < n - 1; i++)
        fprintf(out, "  Вершина %d -- Вершина %d  (вес: %d)\n",
                mst[i].u, mst[i].v, mst[i].weight);
    fprintf(out, "Общий вес остова: %d\n", total_weight);
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    setup_utf8();
#endif

    const char* input_file  = NULL;
    const char* output_file = NULL;

    /* --- разбор аргументов --- */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            input_file = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else {
            fprintf(stderr, "Неизвестный аргумент: %s\n", argv[i]);
            fprintf(stderr, "Запустите с -h для справки.\n");
            return 1;
        }
    }

    if (!input_file) {
        fprintf(stderr, "Ошибка: не указан входной файл (-f).\n");
        fprintf(stderr, "Запустите с -h для справки.\n");
        return 1;
    }

    /* --- чтение графа --- */
    FILE* fin = fopen(input_file, "r");
    if (!fin) {
        fprintf(stderr, "Ошибка: не удалось открыть файл \"%s\"\n", input_file);
        return 1;
    }

    int n;
    int** matrix = read_matrix(fin, &n);
    fclose(fin);

    /* --- алгоритм Краскала --- */
    Edge* mst = malloc((n - 1) * sizeof(Edge));
    int total_weight = kruskal(n, matrix, mst);

    /* --- вывод результата --- */
    if (output_file) {
        FILE* fout = fopen(output_file, "w");
        if (!fout) {
            fprintf(stderr, "Ошибка: не удалось создать файл \"%s\"\n", output_file);
        } else {
            print_result(fout, mst, n, total_weight);
            fclose(fout);
            printf("Результат сохранён в \"%s\"\n", output_file);
        }
    } else {
        print_result(stdout, mst, n, total_weight);
    }

    for (int i = 0; i < n; i++)
        free(matrix[i]);
    free(matrix);
    free(mst);
    return 0;
}