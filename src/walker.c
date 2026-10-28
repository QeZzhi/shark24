// ============================================================
// walker.c — реализация обхода директорий
// walker.c — directory walking implementation
// ============================================================

#include <stdio.h>      // Для snprintf, printf | For snprintf, printf
#include <stdlib.h>     // Для NULL | For NULL
#include <string.h>     // Для strcmp, strstr | For strcmp, strstr
#include <dirent.h>     // Для opendir, readdir | For opendir, readdir
#include <sys/stat.h>   // Для stat | For stat
#include "walker.h"     // Свой заголовок | Own header
#include "search.h"     // Для search_in_file | For search_in_file

void walk_directory(const char *dirpath, const char *keyword, int ignore_case, int show_line_numbers, int only_files, const char *exclude_dir) {
    DIR *dir = opendir(dirpath); // Открываем папку | Open directory
    if (!dir) return;            // Если не открылась — выходим | If failed — exit

    struct dirent *entry; // Структура для чтения папки | Structure for reading directory
    while ((entry = readdir(dir)) != NULL) { // Читаем по одной записи | Read one entry at a time
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue; // Пропускаем . и .. | Skip . and ..
        if (entry->d_name[0] == '.') continue; // Пропускаем скрытые | Skip hidden
        if (strstr(entry->d_name, ".o") != NULL) continue; // Пропускаем .o файлы | Skip .o files

        if (exclude_dir != NULL && strcmp(entry->d_name, exclude_dir) == 0) { // Если папка в исключениях | If dir is excluded
            continue; // Пропускаем | Skip
        }

        char path[2048]; // Буфер для полного пути | Buffer for full path
        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name); // Склеиваем путь | Build path

        struct stat statbuf; // Информация о файле | File info
        if (stat(path, &statbuf) == -1) continue; // Если не удалось — пропускаем | If failed — skip

        if (S_ISDIR(statbuf.st_mode)) { // Если это папка | If it's a directory
            walk_directory(path, keyword, ignore_case, show_line_numbers, only_files, exclude_dir); // Рекурсия | Recursion
        } else if (S_ISREG(statbuf.st_mode)) { // Если это файл | If it's a regular file
            search_in_file(path, keyword, ignore_case, show_line_numbers, only_files); // Поиск | Search
        }
    }
    closedir(dir); // Закрываем папку | Close directory
}