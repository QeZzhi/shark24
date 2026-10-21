#ifndef WALKER_H
#define WALKER_H

// #ifndef / #define / #endif — это "Header Guard" (защита заголовка).
// Если этот файл случайно подключится дважды в одном проекте, 
// #ifndef проверит: "Уже определен WALKER_H?". Если да — он просто пропустит весь код до #endif.
// Это защищает от ошибки "redefinition of function".

#ifdef __cplusplus
extern "C" {
#endif

// Объявление функции.
// Мы не пишем тело функции (что именно она делает). 
// Мы просто говорим компилятору: "Где-то в проекте есть функция walk_directory, 
// которая принимает два указателя на строки (dirpath и keyword) и ничего не возвращает (void)".
void walk_directory(const char *dirpath, const char *keyword);

#ifdef __cplusplus
}
#endif

#endif // WALKER_H