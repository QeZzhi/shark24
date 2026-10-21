#include <stdio.h>
#include <string.h>
#include "walker.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s [-i] <keyword> <directory>\n", argv[0]);
        return 1;
    }
    
    int ignore_case = 0;
    const char *keyword = NULL;   // Объявляем здесь, чтобы было видно везде
    const char *directory = NULL; // Объявляем здесь

    // Проверяем, есть ли флаг -i
    if (strcmp(argv[1], "-i") == 0) {
        if (argc < 4) { // Если есть -i, но нет слова или папки
            printf("Usage: %s -i <keyword> <directory>\n", argv[0]);
            return 1;
        }
        ignore_case = 1;
        keyword = argv[2];   // Присваиваем значение
        directory = argv[3];
    } else {
        keyword = argv[1];   // Присваиваем значение
        directory = argv[2];
    }
    
    printf("\033[1;34m[shark24] Hunting for \"%s\" in %s%s...\033[0m\n\n", keyword, directory, ignore_case ? " (ignore case)" : "");
    
    walk_directory(directory, keyword, ignore_case);
    
    printf("\n\033[1;34m[shark24] Hunt complete. 🦈\033[0m\n");
    return 0;
}
