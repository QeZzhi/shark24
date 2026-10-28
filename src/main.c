#include <stdio.h>
#include <string.h>
#include "walker.h"

#define SHARK_VERSION "BETA 0.1"

int main(int argc, char *argv[]) {
    // 1. Проверка на флаг --version
    if (argc >= 2 && (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0)) {
        printf("shark24 version %s\n", SHARK_VERSION);
        return 0;
    }

    int ignore_case = 0;
    const char *exclude_dir = NULL;
    const char *keyword = NULL;
    const char *directory = NULL;

    // 2. Парсинг аргументов (позволяет комбинировать -i и -e)
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            ignore_case = 1;
        } else if (strcmp(argv[i], "-e") == 0) {
            if (i + 1 < argc) {
                exclude_dir = argv[i + 1];
                i++; // Пропускаем имя папки
            } else {
                printf("Error: -e requires a directory name.\n");
                return 1;
            }
        } else {
            if (keyword == NULL) {
                keyword = argv[i];
            } else if (directory == NULL) {
                directory = argv[i];
            }
        }
    }

    // 3. Проверка на наличие слова и папки
    if (keyword == NULL || directory == NULL) {
        printf("Error: Missing keyword or directory.\n");
        printf("Usage: %s [-i] [-e <exclude_dir>] <keyword> <directory>\n", argv[0]);
        return 1;
    }

    printf("\033[1;34m[shark24] Hunting for \"%s\" in %s...\033[0m\n\n", keyword, directory);
    
    // 4. Запуск
    walk_directory(directory, keyword, ignore_case, exclude_dir);
    
    printf("\n\033[1;34m[shark24] Hunt complete. 🦈\033[0m\n");
    return 0;
}