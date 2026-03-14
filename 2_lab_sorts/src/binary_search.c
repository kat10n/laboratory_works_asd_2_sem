#include <stdio.h>
#include <stdlib.h>
#include "struct.h"


Item* binary_search(Item list[], int left, int right, int key) {
    if (left > right) {
        return NULL;
    }

    int mid = left + (right - left) / 2;

    if (list[mid].key == key) {
        return &list[mid].value;
    }

    if (list[mid].key < key) {
        return binary_search(list, mid + 1, right, key);
    } else {
        return binary_search(list, left, mid - 1, key);
    }
}