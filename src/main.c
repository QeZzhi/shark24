#include <stdio.h>
#include "walker.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <keyword> <directory>\n", argv[0]);
        printf("Example: %s main src/\n", argv[0]);
        return 1;
    }
    
    const char *keyword = argv[1];
    const char *directory = argv[2];
    
    printf("\033[1;34m[shark24] Hunting for \"%s\" in %s...\033[0m\n\n", keyword, directory);
    
    walk_directory(directory, keyword);
    
    printf("\n\033[1;34m[shark24] Hunt complete. 🦈\033[0m\n");
    return 0;
}