#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

int main() {
	read_file("text_files/direct_order.txt");
    read_file("text_files/reverse_order.txt");
    read_file("text_files/random_order.txt");
    return 0;

}