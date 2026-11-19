/*
 * Copyright (c) 2026 Emir (QeZzhi) - Qez Project
 * File: search.h
* Created: 2026-11-19 22:20
 */

// ============================================================
// search.h — заголовок поиска в файле
// search.h — file search header
// ============================================================

#ifndef SEARCH_H
#define SEARCH_H

// Функция поиска | Search function
void search_in_file(const char *filepath, const char *keyword, int ignore_case, int show_line_numbers, int only_files, int count_only);

#endif
