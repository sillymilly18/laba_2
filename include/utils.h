#pragma once

// Копирует строку, вызывающий сам отвечает за очистку памяти после использования
char *copy_string(const char *s);

// Стирает текст из консоли
void flush_stdin_line(void);