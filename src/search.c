#include <stdio.h>
#include <string.h>
#include "search.h"

void search_in_file(const char *filepath, const char *keyword) {
    FILE *file = fopen(filepath, "r");
    if (!file) return;

    char line[1024];
    int line_number = 1;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, keyword) != NULL) {
            printf("\033[1;32m%s:%d:\033[0m %s", filepath, line_number, line);
        }
        line_number++;
    }
    fclose(file);
}