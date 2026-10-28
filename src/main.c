// ============================================================
// main.c — точка входа в программу shark24
// main.c — entry point of shark24
// ============================================================

#include <stdio.h>      // Для printf | For printf
#include <string.h>     // Для strcmp, strstr | For strcmp, strstr
#include "walker.h"     // Для walk_directory | For walk_directory

#define SHARK_VERSION "BETA 0.1" // Версия программы | Program version

int main(int argc, char *argv[]) {
    // Если запросили версию | If version requested
    if (argc >= 2 && (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0)) {
        printf("\033[1;34m"); // Синий цвет | Blue color
        printf("shark24 version %s\n", SHARK_VERSION); // Вывод версии | Print version
        printf("A fast and minimalist code search tool 🦈\n"); // Описание | Description
        printf("\033[0m"); // Сброс цвета | Reset color
        return 0; // Успешный выход | Successful exit
    }

    int ignore_case = 0;            // Флаг -i (0 = выкл) | -i flag (0 = off)
    int show_line_numbers = 0;      // Флаг -n (0 = выкл) | -n flag (0 = off)
    int only_files = 0;             // Флаг -l (0 = выкл) | -l flag (0 = off)
    const char *exclude_dir = NULL; // Флаг -e (NULL = нет) | -e flag (NULL = none)
    const char *keyword = NULL;     // Искомое слово | Search keyword
    const char *directory = NULL;   // Где искать | Where to search

    // Проходим по всем аргументам | Loop through all arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) { // Если аргумент "-i" | If argument is "-i"
            ignore_case = 1;              // Включаем игнор регистра | Enable ignore case
        } else if (strcmp(argv[i], "-n") == 0) { // Если аргумент "-n" | If argument is "-n"
            show_line_numbers = 1;               // Включаем номера строк | Enable line numbers
        } else if (strcmp(argv[i], "-l") == 0) { // Если аргумент "-l" | If argument is "-l"
            only_files = 1;                      // Включаем режим только файлов | Enable only files mode
        } else if (strcmp(argv[i], "-e") == 0) { // Если аргумент "-e" | If argument is "-e"
            if (i + 1 < argc) {                  // Если есть следующий аргумент | If next argument exists
                exclude_dir = argv[i + 1];       // Берем его как папку для исключения | Take it as excluded dir
                i++;                             // Пропускаем этот аргумент | Skip this argument
            } else {
                printf("Error: -e requires a directory name.\n"); // Ошибка | Error
                return 1;                        // Выход с ошибкой | Exit with error
            }
        } else {
            if (keyword == NULL) keyword = argv[i];          // Первое слово — ключ | First word is keyword
            else if (directory == NULL) directory = argv[i]; // Второе слово — папка | Second word is directory
        }
    }

    // Проверяем, что ввели слово и папку | Check if keyword and directory are provided
    if (keyword == NULL || directory == NULL) {
        printf("Error: Missing keyword or directory.\n");
        printf("Usage: %s [-i] [-n] [-l] [-e <exclude_dir>] <keyword> <directory>\n", argv[0]);
        return 1;
    }

    // Выводим сообщение о начале поиска | Print start message
    printf("\033[1;34m[shark24] Hunting for \"%s\" in %s...\033[0m\n\n", keyword, directory);
    
    // Запускаем поиск | Start search
    walk_directory(directory, keyword, ignore_case, show_line_numbers, only_files, exclude_dir);
    
    // Выводим сообщение об окончании | Print completion message
    printf("\n\033[1;34m[shark24] Hunt complete. 🦈\033[0m\n");
    return 0; // Успешный выход | Successful exit
}