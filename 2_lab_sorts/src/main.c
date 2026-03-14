#include <stdio.h>
#include <stdlib.h>
#include "cli.h"
#ifdef _WIN32
#include <windows.h>
#endif


int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif  
	cli();
    return 0;

}