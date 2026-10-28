// ============================================================
// search.c — реализация поиска в файле
// search.c — file search implementation
// ============================================================

#include <stdio.h>      // Для fopen, fread, printf | For fopen, fread, printf
#include <stdlib.h>     // Для malloc, free | For malloc, free
#include <string.h>     // Для strstr, strcasestr | For strstr, strcasestr
#include "search.h"     // Свой заголовок | Own header

void search_in_file(const char *filepath, const char *keyword, int ignore_case, int show_line_numbers, int only_files) {
    FILE *file = fopen(filepath, "rb"); // Открываем файл | Open file
    if (!file) return;                  // Если не открылся — выходим | If failed — exit
    
    fseek(file, 0, SEEK_END);           // Идем в конец файла | Go to end of file
    long file_size = ftell(file);       // Узнаем размер | Get size
    fseek(file, 0, SEEK_SET);           // Возвращаемся в начало | Go back to start
    
    if (file_size <= 0 || file_size > 50 * 1024 * 1024) { // Если файл пустой или слишком большой | If empty or too large
        fclose(file);                   // Закрываем | Close
        return;                         // Выходим | Exit
    }
    
    char *buffer = (char*)malloc(file_size + 1); // Выделяем память | Allocate memory
    if (!buffer) { fclose(file); return; }       // Если не выделилось — выходим | If failed — exit
    
    size_t read_bytes = fread(buffer, 1, file_size, file); // Читаем весь файл | Read whole file
    buffer[read_bytes] = '\0';                             // Ставим конец строки | Set end of string
    
    for (size_t i = 0; i < read_bytes && i < 1024; i++) { // Проверяем первые байты | Check first bytes
        if (buffer[i] == '\0') {                          // Если есть нулевой байт | If null byte found
            free(buffer);                                 // Освобождаем память | Free memory
            fclose(file);                                 // Закрываем файл | Close file
            return;                                       // Выходим (это бинарник) | Exit (binary file)
        }
    }
    
    char *found = buffer;       // Указатель для поиска | Search pointer
    char *current_pos = buffer; // Текущая позиция | Current position
    int line_number = 1;        // Счетчик строк | Line counter
    int file_printed = 0;       // Флаг печати файла для -l | File print flag for -l
    
    while (1) { // Бесконечный цикл поиска | Infinite search loop
        if (ignore_case) { // Если -i | If -i
            found = strcasestr(current_pos, keyword); // Ищем без учета регистра | Search case-insensitive
        } else {
            found = strstr(current_pos, keyword);     // Ищем с учетом регистра | Search case-sensitive
        }
        
        if (found == NULL) break; // Если не нашли — выходим из цикла | If not found — break
        
        for (char *p = current_pos; p < found; p++) { // Идем от текущей позиции до найденного | Go from current pos to found
            if (*p == '\n') line_number++;            // Считаем переносы строк | Count newlines
        }
        
        char *line_start = found; // Ищем начало строки | Find line start
        while (line_start > buffer && *(line_start - 1) != '\n') line_start--; // Идем назад до \n | Go back to \n
        
        char *line_end = found; // Ищем конец строки | Find line end
        while (*line_end != '\0' && *line_end != '\n') line_end++; // Идем вперед до \n | Go forward to \n
        
        char old_char = *line_end; // Запоминаем символ | Save character
        *line_end = '\0';          // Временно обрезаем строку | Temporarily truncate line
        
        if (only_files) { // Если -l | If -l
            if (!file_printed) { // Если файл еще не печатали | If file not printed yet
                printf("\033[1;32m%s\033[0m\n", filepath); // Печатаем имя файла | Print file name
                file_printed = 1; // Запоминаем, что напечатали | Remember we printed it
            }
        } else if (show_line_numbers) { // Если -n | If -n
            printf("\033[1;32m%s:%d:\033[0m %s\n", filepath, line_number, line_start); // Печатаем с номером | Print with line number
        } else {
            printf("\033[1;32m%s:\033[0m %s\n", filepath, line_start); // Печатаем без номера | Print without line number
        }
        
        line_end = old_char;   // Возвращаем символ на место | Restore character
        current_pos = found + 1; // Сдвигаем позицию для следующего поиска | Move position for next search
    }

    free(buffer); // Освобождаем память | Free memory
    fclose(file); // Закрываем файл | Close file
