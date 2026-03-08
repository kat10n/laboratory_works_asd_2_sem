#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "cli.h"
#include "tree.h"
#include "struct.h"


int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    Node* root = NULL;
    cli(&root);
    // Освобождаем память перед выходом
    if (root) {
        removeSubtree(root);
    }
    return 0;
}
