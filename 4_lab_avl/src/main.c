#include <stdio.h>
#include "console_and_files.h"
#include "struct.h"


int main() {
    Node* root = NULL;
    process_file("files/input.txt", &root);
    process_stdin(&root);
    free_tree(root);
    return 0;
}
