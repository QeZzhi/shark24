#ifndef SEARCH_H
#define SEARCH_H

// Защита от повторного включения (Header Guard).
// Если файл подключится дважды, компилятор не будет ругаться.

#ifdef __cplusplus
extern "C" {
#endif

// Объявление функции поиска в файле.
// Говорим компилятору: "Где-то в search.c есть функция search_in_file, 
// которая берет путь к файлу и ключевое слово, и ничего не возвращает (void)".
void search_in_file(const char *filepath, const char *keyword);

#ifdef __cplusplus
}
#endif

#endif // SEARCH_H