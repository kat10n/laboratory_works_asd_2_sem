#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "struct.h"
#include "bubble_sort.h"
#include "binary_search.h"

void setup_utf8() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

void test_bubble_sort_sorted() {
    printf("Тест 1: Сортировка уже отсортированного массива\n");
    Item list[3] = {{1, "a"}, {2, "b"}, {3, "c"}};
    bubble_sort(list, 3);
    assert(list[0].key == 1);
    assert(list[1].key == 2);
    assert(list[2].key == 3);
}

void test_bubble_sort_reverse() {
    printf("Тест 2: Сортировка массива в обратном порядке\n");
    Item list[4] = {{4, "d"}, {3, "c"}, {2, "b"}, {1, "a"}};
    bubble_sort(list, 4);
    assert(list[0].key == 1);
    assert(list[1].key == 2);
    assert(list[2].key == 3);
    assert(list[3].key == 4);
}

void test_bubble_sort_single() {
    printf("Тест 3: Сортировка массива из одного элемента\n");
    Item list[1] = {{42, "x"}};
    bubble_sort(list, 1);
    assert(list[0].key == 42);
}

void test_binary_search_found() {
    printf("Тест 4: Бинарный поиск — элемент найден\n");
    Item list[5] = {{1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"}};
    Item *result = binary_search(list, 0, 4, 3);
    assert(result != NULL);
    assert(result->key == 3);
}

void test_binary_search_not_found() {
    printf("Тест 5: Бинарный поиск — элемент не найден\n");
    Item list[3] = {{1, "a"}, {2, "b"}, {3, "c"}};
    Item *result = binary_search(list, 0, 2, 10);
    assert(result == NULL);
}

int main() {
    setup_utf8();
    test_bubble_sort_sorted();
    test_bubble_sort_reverse();
    test_bubble_sort_single();
    test_binary_search_found();
    test_binary_search_not_found();
    printf("Все тесты пройдены корректно\n");
    return 0;
}
