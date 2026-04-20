#include <stdio.h>
#include "console_and_files.h"
#include "struct.h"
#ifdef _WIN32
#include <windows.h>
#endif

void setup_utf8() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

int main() {
    setup_utf8();
    Node* root = NULL;
    process_stdin(&root);
    free_tree(root);
    return 0;
}