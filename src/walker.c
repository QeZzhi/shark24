#include <stdio.h>      // Для snprintf (безопасное формирование строк)
#include <stdlib.h>     // Для стандартных функций
#include <string.h>     // Для strcmp (сравнение строк) и strstr (поиск подстроки)
#include <dirent.h>     // Для работы с директориями (opendir, readdir, closedir)
#include <sys/stat.h>   // Для stat (получение информации о файле)
#include "walker.h"     // Наш заголовочный файл
#include "search.h"     // Для вызова search_in_file

void walk_directory(const char *dirpath, const char *keyword) {
    // Открываем директорию. opendir возвращает указатель на поток директории.
    // Если не удалось открыть (например, нет прав), возвращает NULL.
    DIR *dir = opendir(dirpath);
    if (!dir) return; // Если папку открыть нельзя, просто выходим из функции.
    
    // Структура dirent хранит информацию о текущей записи в папке (имя файла/папки).
    struct dirent *entry;
    
    // readdir читает папку по одной записи за раз. 
    // Когда записи заканчиваются, возвращает NULL, и цикл прерывается.
    while ((entry = readdir(dir)) != NULL) {
        // Пропускаем "." (текущая папка) и ".." (родительская папка).
        // Если их не пропустить, программа уйдет в бесконечную рекурсию.
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        
        // Пропускаем скрытые файлы и папки (начинаются с точки, например, .git).
        if (entry->d_name[0] == '.') continue;
        
        // Пропускаем объектные файлы (.o), чтобы не искать в бинарниках и не тратить время.
        if (strstr(entry->d_name, ".o") != NULL) continue;
        
        // Формируем полный путь к файлу или папке.
        // snprintf безопасно записывает строку, не переполняя буфер path (размер 2048 байт).
        char path[2048];
        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);
        
        // Получаем информацию о файле/папке по его пути.
        struct stat statbuf;
        if (stat(path, &statbuf) == -1) continue; // Если не удалось получить инфу, пропускаем.
        
        // Проверяем, является ли объект директорией.
        if (S_ISDIR(statbuf.st_mode)) {
            // Если это папка — вызываем эту же функцию (рекурсия), 
            // чтобы зайти внутрь и продолжить поиск там.
            walk_directory(path, keyword);
        } 
        // Проверяем, является ли объект обычным файлом.
        else if (S_ISREG(statbuf.st_mode)) {
            // Если это файл — передаем его в функцию поиска.
            search_in_file(path, keyword);
        }
    }
    
    // Закрываем директорию, чтобы освободить ресурсы системы.
    closedir(dir);
}