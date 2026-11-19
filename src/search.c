/*
 * Copyright (c) 2026 Emir (QeZzhi) - Qez Project
 * File: search.c
 * Created: 2026-11-19 15:32
 */

// ============================================================
// search.c — реализация поиска строк в файле
// search.c — implementation of string search in file
// ============================================================

#define _GNU_SOURCE

#include <stdio.h>      // Для fopen, fread, printf | For fopen, fread, printf
#include <stdlib.h>     // Для malloc, free | For malloc, free
#include <string.h>     // Для strstr, strcasestr | For strstr, strcasestr
#include "../inc/search.h"     // Свой заголовок | Own header

void search_in_file(const char *filepath, const char *keyword, int ignore_case, int show_line_numbers, int only_files) {
    FILE *file = NULL;
    char *buffer = NULL;

    // Открываем файл в бинарном режиме для скорости | Open file in binary mode for speed
    file = fopen(filepath, "rb");
    if (!file) return; // Если не открылся — выходим | If failed — exit
    
    fseek(file, 0, SEEK_END);       // Идем в конец файла | Go to end of file
    long file_size = ftell(file);   // Узнаем размер файла | Get file size
    fseek(file, 0, SEEK_SET);       // Возвращаемся в начало | Go back to start
    
    // Защита от пустых и слишком больших файлов (больше 50 МБ) | Protection against empty and huge files
    if (file_size <= 0 || file_size > 50 * 1024 * 1024) {
        goto cleanup;               // Переходим к очистке | Go to cleanup
    }
    
    // Выделяем память под весь файл + 1 байт для '\0' | Allocate memory for whole file + 1 byte for '\0'
    buffer = (char*)malloc(file_size + 1);
    if (!buffer) { goto cleanup; } // Если памяти нет — на очистку | If out of memory — go to cleanup
    
    // Читаем весь файл за один раз | Read the whole file at once
    size_t read_bytes = fread(buffer, 1, file_size, file);
    buffer[read_bytes] = '\0';             // Ставим конец строки | Set end of string
    
    // Проверяем первые 1024 байта на наличие '\0' (бинарный файл) | Check first 1024 bytes for '\0' (binary)
    for (size_t i = 0; i < read_bytes && i < 1024; i++) {
        if (buffer[i] == '\0') {           // Если нашли нулевой байт | If null byte found
            goto cleanup;                  // Это бинарник — на очистку | This is binary — go to cleanup
        }
    }
    
    char *found = buffer;       // Указатель на найденное слово | Pointer to found word
    int file_printed = 0;       // Флаг для -l (напечатали ли файл) | Flag for -l (file printed)
    
    // Главный цикл поиска | Main search loop
    while (1) {
        // Поиск с учетом или без учета регистра | Search with or without case sensitivity
        if (ignore_case) {
            found = strcasestr(found, keyword); // Без учета регистра | Case-insensitive
        } else {
            found = strstr(found, keyword);     // С учетом регистра | Case-sensitive
        }
        
        if (found == NULL) break; // Если не нашли — выходим из цикла | If not found — break
        
        // === ИСПРАВЛЕНИЕ: Считаем строки ОТ НАЧАЛА ФАЙЛА до found ===
        // === FIX: Count lines FROM START OF FILE to found ===
        int line_number = 1;
        for (char *p = buffer; p < found; p++) {
            if (*p == '\n') line_number++; // Каждый перенос строки +1 | Each newline +1
        }
        
        // Ищем начало строки (идем назад до '\n') | Find start of the line (go back to '\n')
        char *line_start = found;
        while (line_start > buffer && *(line_start - 1) != '\n') line_start--;
        
        // Ищем конец строки (идем вперед до '\n') | Find end of the line (go forward to '\n')
        char *line_end = found;
        while (*line_end != '\0' && *line_end != '\n') line_end++;
        
        char old_char = *line_end; // Запоминаем символ в конце | Save char at the end
        *line_end = '\0';          // Временно обрезаем строку | Temporarily truncate line
        
        // ВЫВОД В ЗАВИСИМОСТИ ОТ ФЛАГОВ | OUTPUT DEPENDING ON FLAGS
        if (only_files) {
            if (!file_printed) { // Если файл еще не печатали | If file not printed yet
                printf("\033[1;32m%s\033[0m\n", filepath); // Печатаем только имя файла | Print only filename
                file_printed = 1;  // Запоминаем, что напечатали | Remember we printed it
            }
        } else if (show_line_numbers) {
            printf("\033[1;32m%s:%d:\033[0m %s\n", filepath, line_number, line_start); // С номером | With line number
        } else {
            printf("\033[1;32m%s:\033[0m %s\n", filepath, line_start); // Без номера | Without line number
        }
        
        *line_end = old_char;   // Возвращаем символ на место | Restore character
        found++;                // Сдвигаем позицию для следующего поиска | Move position for next search
    }

cleanup:
    if (buffer) free(buffer); // Освобождаем память, если она была выделена | Free memory if allocated
    if (file) fclose(file);   // Закрываем файл, если он был открыт | Close file if opened
}