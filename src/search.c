#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"

void search_in_file(const char *filepath, const char *keyword, int ignore_case, int show_line_numbers) {
    FILE *file = fopen(filepath, "rb");
    if (!file) return;

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size <= 0 || file_size > 50 * 1024 * 1024) {
        fclose(file);
        return;
    }

    char *buffer = (char*)malloc(file_size + 1);
    if (!buffer) { fclose(file); return; }

    size_t read_bytes = fread(buffer, 1, file_size, file);
    buffer[read_bytes] = '\0';

    for (size_t i = 0; i < read_bytes && i < 1024; i++) {
        if (buffer[i] == '\0') {
            free(buffer);
            fclose(file);
            return;
        }
    }

    char *found = buffer;
    char *current_pos = buffer;
    int line_number = 1;

    while (1) {
        if (ignore_case) {
            found = strcasestr(current_pos, keyword); 
        } else {
            found = strstr(current_pos, keyword);
        }

        if (found == NULL) break;

        // Считаем строки от текущей позиции до найденного слова
        for (char *p = current_pos; p < found; p++) {
            if (*p == '\n') line_number++;
        }

        char *line_start = found;
        while (line_start > buffer && *(line_start - 1) != '\n') line_start--;
        
        char *line_end = found;
        while (*line_end != '\0' && *line_end != '\n') line_end++;
        
        char old_char = *line_end;
        *line_end = '\0';

        // ВЫВОД: если show_line_numbers == 1, печатаем номер строки
        if (show_line_numbers) {
            printf("\033[1;32m%s:%d:\033[0m %s\n", filepath, line_number, line_start);
        } else {
            printf("\033[1;32m%s:\033[0m %s\n", filepath, line_start);
        }

        *line_end = old_char;
        current_pos = found + 1;
    }

    free(buffer);
    fclose(file);
}