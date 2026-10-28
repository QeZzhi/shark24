// ============================================================
// search.h — заголовок поиска в файле
// search.h — file search header
// ============================================================

#ifndef SEARCH_H
#define SEARCH_H

// Функция поиска | Search function
void search_in_file(const char *filepath, const char *keyword, int ignore_case, int show_line_numbers, int only_files);

#endif