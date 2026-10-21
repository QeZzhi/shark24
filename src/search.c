#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"

void search_in_file(const char *filepath, const char *keyword, int ignore_case) {
    // Открываем файл в бинарном режиме для скорости
    FILE *file = fopen(filepath, "rb");
    if (!file) return;

    // Узнаем размер файла
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Защита от пустых файлов и гигантских бинарников (больше 50 МБ)
    if (file_size <= 0 || file_size > 50 * 1024 * 1024) {
        fclose(file);
        return;
    }

    // Выделяем память под весь файл + 1 байт для нулевого символа
    char *buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        return;
    }

    // Читаем весь файл за один раз
    size_t read_bytes = fread(buffer, 1, file_size, file);
    buffer[read_bytes] = '\0'; // Добавляем конец строки

    // Проверка на бинарный файл (ищем нулевые байты в начале)
    for (size_t i = 0; i < read_bytes && i < 1024; i++) {
        if (buffer[i] == '\0') {
            free(buffer);
            fclose(file);
            return; // Пропускаем бинарники
        }
    }

    char *found = buffer;
    
    // Главный цикл поиска
    while (1) {
        // Выбираем функцию поиска в зависимости от флага
        if (ignore_case) {
            found = strcasestr(found, keyword); // Поиск без учета регистра
        } else {
            found = strstr(found, keyword);     // Обычный поиск
        }
        
        if (found == NULL) break; // Если больше не нашли, выходим

        // Находим начало строки
        char *line_start = found;
        while (line_start > buffer && *(line_start - 1) != '\n') line_start--;
        
        // Находим конец строки
        char *line_end = found;
        while (*line_end != '\0' && *line_end != '\n') line_end++;
        
        // Временно обрезаем строку для вывода
        char old_char = *line_end;
        *line_end = '\0';
        
        // Выводим путь и найденную строку
        printf("\033[1;32m%s:\033[0m %s\n", filepath, line_start);
        
        // Возвращаем символ на место
        *line_end = old_char;

        found++; // Сдвигаем указатель, чтобы искать дальше
    }

    free(buffer);
    fclose(file);
}
