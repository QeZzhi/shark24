#include <stdio.h>
#include <string.h>
#include "walker.h"

int main(int argc, char *argv[]) {
    const char *keyword;
    const char *directory;
    
    if (argc >= 4 && strcmp(argv[1], "search") == 0) {
        keyword = argv[2];
        directory = argv[3];
    } else {
        if (argc >= 3) {
            keyword = argv[1];
            directory = argv[2];
        } else {
            printf("Usage: %s search <keyword> <directory>\n", argv[0]);
            return 1;
        }
    }
    
    printf("\033[1;34m[shark24] Hunting for \"%s\" in %s...\033[0m\n\n", keyword, directory);
    
    walk_directory(directory, keyword);
    
    printf("\n\033[1;34m[shark24] Hunt complete. 🦈\033[0m\n");
    
    return 0;
}