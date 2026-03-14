#include <stdio.h>
#include <stdlib.h>
#include "struct.h"


Item* binary_search(Item list[], int n, Item item, int key){
    if (list == NULL || item == NULL || key < list[0].key || key > list[0].key){
        int a = (n - 1) / 2;
        if (list[a].key == item.key){
            return list[a].value;
        }
        if (list[a].key < item.key){
            Item slice[a];

            for (int i = 0; i < a; i++) {
                slice[i] = list[i];
            }
            list = slice;
            binary_search(list, a, item, key);
        }
        if (list[a].key > item.key){
            Item slice[n - a];

            for (int i = a++; i < n; i++) {
                slice[i] = list[i];
            }
            list = slice;
            binary_search(list, n, item, key);
        }
    }
}