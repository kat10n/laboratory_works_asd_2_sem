#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

void bubble_sort(Item *list[], int n) {
    for (int i = 0; i < n - 1; i++) {
		flag = 0;
		for (int j = 0; j < n - i - 1; j++) {
			if (list[j]->key < list[j + 1]->key) {
				Item a = list[j];
				list[j] = list[j + 1];
				list[j + 1] = a;
				flag = 1;
			}
		if (flag == 0){
			break;
		}
	}
}