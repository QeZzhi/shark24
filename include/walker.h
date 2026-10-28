// ============================================================
// walker.h — заголовок обхода директорий
// walker.h — directory walking header
// ============================================================

#ifndef WALKER_H
#define WALKER_H

// Функция рекурсивного обхода | Recursive walking function
void walk_directory(const char *dirpath, const char *keyword, int ignore_case, int show_line_numbers, int only_files, const char *exclude_dir);

#endif