#include <stdio.h>
#include <string.h>
#include "walker.h"

#define SHARK_VERSION "BETA 0.1"

int main(int argc, char *argv[]) {
    if (argc >= 2 && (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0)) {
        printf("shark24 version %s\n", SHARK_VERSION);
        printf("A fast and minimalist code search tool 🦈\n");
        return 0;
    }

    int ignore_case = 0;
    int show_line_numbers = 0;
    int only_files = 0; // <--- НОВЫЙ ФЛАГ
    const char *exclude_dir = NULL;
    const char *keyword = NULL;
    const char *directory = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            ignore_case = 1;
        } else if (strcmp(argv[i], "-n") == 0) {
            show_line_numbers = 1;
        } else if (strcmp(argv[i], "-l") == 0) { // <--- ОБРАБОТКА -l
            only_files = 1;
        } else if (strcmp(argv[i], "-e") == 0) {
            if (i + 1 < argc) {
                exclude_dir = argv[i + 1];
                i++; 
            } else {
                printf("Error: -e requires a directory name.\n");
                return 1;
            }
        } else {
            if (keyword == NULL) keyword = argv[i];
            else if (directory == NULL) directory = argv[i];
        }
    }

    if (keyword == NULL || directory == NULL) {
        printf("Error: Missing keyword or directory.\n");
        printf("Usage: %s [-i] [-n] [-l] [-e <exclude_dir>] <keyword> <directory>\n", argv[0]);
        return 1;
    }

    printf("\033[1;34m[shark24] Hunting for \"%s\" in %s...\033[0m\n\n", keyword, directory);
    
    // Передаем only_files
    walk_directory(directory, keyword, ignore_case, show_line_numbers, only_files, exclude_dir);
    
    printf("\n\033[1;34m[shark24] Hunt complete. 🦈\033[0m\n");
    return 0;
}