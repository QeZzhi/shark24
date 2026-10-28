#ifndef WALKER_H
#define WALKER_H

void walk_directory(const char *dirpath, const char *keyword, int ignore_case, int show_line_numbers, const char *exclude_dir);

#endif