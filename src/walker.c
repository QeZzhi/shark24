#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "walker.h"
#include "search.h"

void walk_directory(const char *dirpath, const char *keyword, int ignore_case) { // <-- Добавили ignore_case
    DIR *dir = opendir(dirpath);
    if (!dir) return;
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        if (entry->d_name[0] == '.') continue;
        if (strstr(entry->d_name, ".o") != NULL) continue;
        
        char path[2048];
        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);
        
        struct stat statbuf;
        if (stat(path, &statbuf) == -1) continue;
        
        if (S_ISDIR(statbuf.st_mode)) {
            // Передаем ignore_case при рекурсии
            walk_directory(path, keyword, ignore_case); 
        } else if (S_ISREG(statbuf.st_mode)) {
            // Передаем ignore_case в функцию поиска
            search_in_file(path, keyword, ignore_case); 
        }
    }
    closedir(dir);
}
