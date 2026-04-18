#ifndef LABORATORY_WORKS_ASD_2_SEM_WORKING_WITH_FILES_H
#define LABORATORY_WORKS_ASD_2_SEM_WORKING_WITH_FILES_H
#include "struct.h"
void process_file(const char* filename, Node** root);
void process_stdin(Node** root);
void log_result(const char* message, const char* key, double val, int found);
void free_tree(Node* node);
#endif //LABORATORY_WORKS_ASD_2_SEM_WORKING_WITH_FILES_H