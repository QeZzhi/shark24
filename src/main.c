#include <stdio.h>
#include <string.h>
#include "walker.h"

int main(int argc, char *argv[]) {
    // Если аргументов меньше 3, значит не хватает параметров
    if (argc < 3) {
        printf("Usage: %s [-i] <keyword> <directory>\n", argv[0]);
        return 1;
    }
    
    int ignore_case = 0; // Флаг игнорирования регистра
    const char *keyword;
    const char *directory;

    // Проверяем, есть ли флаг -i
    if (strcmp(argv[1], "-i") == 0) {
        ignore_case = 1;
        keyword = argv[2];
        directory = argv[3];
        
        // Если после -i не хватает аргументов
        if (argc < 4) {
            printf("Usage: %s [-i] <keyword> <directory>\n", argv[0]);
            return 1;
        }
    } else {
        keyword = argv[1];
        directory = argv[2];
    }
    
    printf("\033[1;34m[shark24] Hunting for \"%s\" in %s...\033[0m\n\n", keyword, directory);
    
    // Передаем флаг ignore_case дальше в функцию поиска
    walk_directory(directory, keyword, ignore_case);
    
    printf("\n\033[1;34m[shark24] Hunt complete. 🦈\033[0m\n");
    return 0;
}
