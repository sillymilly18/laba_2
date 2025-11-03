#pragma once


// Считывает строку из stdin (динамически выделяет память)
int in_read_line(char **out);

// Считывает целое число с проверкой диапазона
int in_read_int(const char *prompt, int min, int max, int *out);

// Считывает подтверждение (y/n)
int in_ask_yes_no(const char *prompt);
