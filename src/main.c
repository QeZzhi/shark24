#include <stdio.h>
#include <string.h>
#include "walker.h"

#define SHARK_VERSION "BETA 0.1"

int main(int argc, char *argv[]) {
    // 1. Проверка на флаг --version (или -v)
    if (argc >= 2 && (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0)) {
        printf("shark24 version %s\n", SHARK_VERSION);
        printf("A fast and minimalist code search tool 🦈\n");
        return 0;
    }

    // 2. Проверка на правильное количество аргументов
    if (argc < 3) {
        printf("Usage: %s <keyword> <directory>\n", argv[0]);
        printf("Example: %s main src/\n", argv[0]);
        printf("Run '%s --version' to check the version.\n", argv[0]);
        return 1;
    }
    
    // 3. Сохраняем аргументы
    const char *keyword = argv[1];
    const char *directory = argv[2];
    
    // 4. Выводим сообщение о начале поиска
    printf("\033[1;34m[shark24] Hunting for \"%s\" in %s...\033[0m\n\n", keyword, directory);
    
    // 5. Запускаем поиск
    walk_directory(directory, keyword);
    
    // 6. Выводим сообщение об окончании
    printf("\n\033[1;34m[shark24] Hunt complete. 🦈\033[0m\n");
    
    return 0;
}