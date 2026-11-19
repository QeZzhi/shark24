/*
 * Copyright (c) 2026 Emir (QeZzhi) - Qez Project
 * File: walker.h
* Created: 2026-11-19 22:20
 */

// ============================================================
// walker.h — заголовок обхода директорий
// walker.h — directory walking header
// ============================================================

#ifndef WALKER_H
#define WALKER_H

// Функция рекурсивного обхода | Recursive walking function
void walk_directory(const char *dirpath, const char *keyword, int ignore_case, int show_line_numbers, int only_files, int count_only, const char *exclude_dir);

#endif
