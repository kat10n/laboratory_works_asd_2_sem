#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kruskal.h"

#ifdef _WIN32
#include <windows.h>
static void setup_utf8(void) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}
#else
static void setup_utf8(void) { (void)0; }
#endif

static void free_matrix(int** m, int n) {
    if (!m) return;
    for (int i = 0; i < n; i++)
        free(m[i]);
    free(m);
}

static int** alloc_matrix(int n) {
    int** m = malloc((size_t)n * sizeof(int*));
    for (int i = 0; i < n; i++)
        m[i] = malloc((size_t)n * sizeof(int));
    return m;
}

/* 0 — матрица корректна, иначе -1 и текст в err */
static int validate_matrix(int n, int** m, char* err, size_t err_len) {
    if (n < 1) {
        snprintf(err, err_len, "число вершин должно быть >= 1");
        return -1;
    }
    if (n == 1) {
        if (m[0][0] != 0) {
            snprintf(err, err_len, "на диагонали должны быть нули");
            return -1;
        }
        return 0;
    }

    for (int i = 0; i < n; i++) {
        if (m[i][i] != 0) {
            snprintf(err, err_len, "на диагонали должны быть нули");
            return -1;
        }
        for (int j = 0; j < n; j++) {
            if (m[i][j] < 0) {
                snprintf(err, err_len, "веса не могут быть отрицательными");
                return -1;
            }
        }
        for (int j = i + 1; j < n; j++) {
            if (m[i][j] != m[j][i]) {
                snprintf(err, err_len, "матрица должна быть симметричной");
                return -1;
            }
        }
    }

    int* vis = calloc((size_t)n, sizeof(int));
    int* q = malloc((size_t)n * sizeof(int));
    int head = 0, tail = 0;
    q[tail++] = 0;
    vis[0] = 1;
    int seen = 1;

    while (head < tail) {
        int v = q[head++];
        for (int u = 0; u < n; u++) {
            if (!vis[u] && m[v][u] > 0) {
                vis[u] = 1;
                q[tail++] = u;
                seen++;
            }
        }
    }
    free(q);

    if (seen != n) {
        snprintf(err, err_len, "граф несвязный");
        free(vis);
        return -1;
    }

    int edges = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (m[i][j] > 0)
                edges++;

    if (edges < n - 1) {
        snprintf(err, err_len, "слишком мало рёбер для связного графа");
        free(vis);
        return -1;
    }

    free(vis);
    return 0;
}

static void print_menu(void) {
    printf("\n    Меню\n");
    printf("0 - выход\n");
    printf("1 - ввести матрицу в терминале  ->  вывести МОД в терминал\n");
    printf("2 - ввести матрицу в терминале  ->  записать МОД в файл\n");
    printf("3 - прочитать матрицу из файла  ->  вывести МОД в терминал\n");
    printf("4 - прочитать матрицу из файла  ->  записать МОД в файл\n");
    printf("Выбор: ");
    fflush(stdout);
}

static void print_result(FILE* out, Edge* mst, int n, int total_weight) {
    fprintf(out, "Рёбра минимального остовного дерева:\n");
    for (int i = 0; i < n - 1; i++)
        fprintf(out, "  Вершина %d - Вершина %d  (вес %d)\n",
                mst[i].u, mst[i].v, mst[i].weight);
    fprintf(out, "Общий вес остова: %d\n", total_weight);
}

static int read_choice(void) {
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin))
        return 0;
    int choice = -1;
    sscanf(buf, "%d", &choice);
    return choice;
}

static void trim_line(char* s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r'))
        s[--len] = '\0';
}

static void read_line(char* buf, size_t size, const char* prompt) {
    printf("%s", prompt);
    fflush(stdout);
    if (!fgets(buf, (int)size, stdin))
        buf[0] = '\0';
    trim_line(buf);
}

static int read_matrix_terminal(int* n, int*** out) {
    printf("Число вершин N: ");
    fflush(stdout);
    if (scanf("%d", n) != 1 || *n < 1) {
        printf("Ошибка: N должно быть >= 1.\n");
        while (getchar() != '\n') {}
        return 0;
    }

    int** m = alloc_matrix(*n);
    printf("Введите матрицу %d x %d (по строкам, 0 - нет ребра):\n", *n, *n);
    for (int i = 0; i < *n; i++) {
        printf("Строка %d: ", i);
        fflush(stdout);
        for (int j = 0; j < *n; j++)
            if (scanf("%d", &m[i][j]) != 1) {
                printf("Ошибка ввода чисел.\n");
                while (getchar() != '\n') {}
                free_matrix(m, *n);
                return 0;
            }
    }
    while (getchar() != '\n') {}

    *out = m;
    return 1;
}

static int read_matrix_file(const char* path, int* n, int*** out) {
    FILE* f = fopen(path, "r");
    if (!f) {
        printf("Ошибка: не удалось открыть \"%s\"\n", path);
        return 0;
    }

    if (fscanf(f, "%d", n) != 1 || *n < 1) {
        printf("Ошибка: в файле неверное число вершин.\n");
        fclose(f);
        return 0;
    }

    int** m = alloc_matrix(*n);
    for (int i = 0; i < *n; i++)
        for (int j = 0; j < *n; j++)
            if (fscanf(f, "%d", &m[i][j]) != 1) {
                printf("Ошибка: не хватает чисел в матрице (ожидалось %d x %d).\n", *n, *n);
                fclose(f);
                free_matrix(m, *n);
                return 0;
            }

    fclose(f);
    *out = m;
    return 1;
}

static int run_kruskal_and_print(int n, int** matrix, FILE* out) {
    char err[256];
    if (validate_matrix(n, matrix, err, sizeof(err)) != 0) {
        printf("Некорректная матрица смежности: %s\n", err);
        return 0;
    }

    if (n == 1) {
        fprintf(out, "Одна вершина - остов пуст, вес 0.\n");
        return 1;
    }

    Edge* mst = malloc((size_t)(n - 1) * sizeof(Edge));
    int total = kruskal(n, matrix, mst);
    print_result(out, mst, n, total);
    free(mst);
    return 1;
}

static void option_terminal_terminal(void) {
    int n = 0;
    int** m = NULL;
    if (!read_matrix_terminal(&n, &m))
        return;
    run_kruskal_and_print(n, m, stdout);
    free_matrix(m, n);
}

static void option_terminal_file(void) {
    char path[512];
    char err[256];
    read_line(path, sizeof(path), "Имя файла для записи МОД: ");
    if (path[0] == '\0') {
        printf("Файл не указан.\n");
        return;
    }

    int n = 0;
    int** m = NULL;
    if (!read_matrix_terminal(&n, &m))
        return;

    if (validate_matrix(n, m, err, sizeof(err)) != 0) {
        printf("Некорректная матрица смежности: %s\n", err);
        free_matrix(m, n);
        return;
    }

    FILE* f = fopen(path, "w");
    if (!f) {
        printf("Ошибка: не удалось создать \"%s\"\n", path);
        free_matrix(m, n);
        return;
    }

    if (run_kruskal_and_print(n, m, f))
        printf("МОД записан в \"%s\".\n", path);

    fclose(f);
    free_matrix(m, n);
}

static void option_file_terminal(void) {
    char path[512];
    read_line(path, sizeof(path), "Имя файла с матрицей: ");
    if (path[0] == '\0') {
        printf("Файл не указан.\n");
        return;
    }

    int n = 0;
    int** m = NULL;
    if (!read_matrix_file(path, &n, &m))
        return;

    run_kruskal_and_print(n, m, stdout);
    free_matrix(m, n);
}

static void option_file_file(void) {
    char in_path[512], out_path[512];
    read_line(in_path, sizeof(in_path), "Имя файла с матрицей: ");
    if (in_path[0] == '\0') {
        printf("Файл не указан.\n");
        return;
    }
    read_line(out_path, sizeof(out_path), "Имя файла для записи МОД: ");
    if (out_path[0] == '\0') {
        printf("Файл для записи не указан.\n");
        return;
    }

    int n = 0;
    int** m = NULL;
    char err[256];
    if (!read_matrix_file(in_path, &n, &m))
        return;

    if (validate_matrix(n, m, err, sizeof(err)) != 0) {
        printf("Некорректная матрица смежности: %s\n", err);
        free_matrix(m, n);
        return;
    }

    FILE* f = fopen(out_path, "w");
    if (!f) {
        printf("Ошибка: не удалось создать \"%s\"\n", out_path);
        free_matrix(m, n);
        return;
    }

    if (run_kruskal_and_print(n, m, f))
        printf("МОД записан в \"%s\".\n", out_path);

    fclose(f);
    free_matrix(m, n);
}

int main(void) {
    setup_utf8();

    for (;;) {
        print_menu();
        switch (read_choice()) {
            case 0:
                printf("Выход.\n");
                return 0;
            case 1:
                option_terminal_terminal();
                break;
            case 2:
                option_terminal_file();
                break;
            case 3:
                option_file_terminal();
                break;
            case 4:
                option_file_file();
                break;
            default:
                printf("Нет такого пункта. Введите число от 0 до 4.\n");
        }
    }
}