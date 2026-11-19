// ============================================================
// walker.c — реализация обхода директорий
// walker.c — directory walking implementation
// ============================================================

#include <stdio.h>      // Для snprintf, printf | For snprintf, printf
#include <stdlib.h>     // Для NULL | For NULL
#include <string.h>     // Для strcmp, strstr, strlen | For strcmp, strstr, strlen
#include <dirent.h>     // Для opendir, readdir | For opendir, readdir
#include <sys/stat.h>   // Для stat | For stat
#include "../inc/walker.h"     // Свой заголовок | Own header
#include "../inc/search.h"     // Для search_in_file | For search_in_file

void walk_directory(const char *dirpath, const char *keyword, int ignore_case, int show_line_numbers, int only_files, const char *exclude_dir) {
    DIR *dir = opendir(dirpath); // Открываем папку | Open directory
    if (!dir) return;            // Если не открылась — выходим | If failed — exit

    struct dirent *entry; // Структура для чтения папки | Structure for reading directory
    while ((entry = readdir(dir)) != NULL) { // Читаем по одной записи | Read one entry at a time
        
        // Пропускаем . и .. | Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue; 
        
        // Пропускаем скрытые файлы (начинаются с точки) | Skip hidden files (start with dot)
        if (entry->d_name[0] == '.') continue; 
        
        // === ИСПРАВЛЕНИЕ: Проверка на .o в КОНЦЕ имени файла ===
        // === FIX: Check for .o at the END of the filename ===
        size_t name_len = strlen(entry->d_name);
        if (name_len > 2 && strcmp(entry->d_name + name_len - 2, ".o") == 0) {
            continue; // Пропускаем только если файл заканчивается на .o | Skip only if file ends with .o
        }

        // Проверка на исключенную папку | Check for excluded directory
        if (exclude_dir != NULL && strcmp(entry->d_name, exclude_dir) == 0) {
            continue; // Пропускаем | Skip
        }

        char path[2048]; // Буфер для полного пути | Buffer for full path
        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name); // Склеиваем путь | Build path

        struct stat statbuf; // Информация о файле | File info
        if (lstat(path, &statbuf) == -1) continue; // Если не удалось — пропускаем | If failed — skip
        if (S_ISLNK(statbuf.st_mode)) continue; // Пропускаем символические ссылки | Skip symlinks
        if (S_ISDIR(statbuf.st_mode)) { // Если это папка | If it's a directory
            walk_directory(path, keyword, ignore_case, show_line_numbers, only_files, exclude_dir); // Рекурсия | Recursion
        } else if (S_ISREG(statbuf.st_mode)) { // Если это файл | If it's a regular file
            search_in_file(path, keyword, ignore_case, show_line_numbers, only_files); // Поиск | Search
        }
    }
    closedir(dir); // Закрываем папку | Close directory
}