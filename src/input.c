#include "input.h"
#include <stdio.h>
#include <stdlib.h>

int in_read_line(char **out) {
    *out = NULL;
    size_t cap = 0, len = 0;
    int ch;
    while ((ch = getchar()) != EOF) {
        if (len + 1 >= cap) {
            size_t new_cap = cap ? cap * 2 : 64;
            char *p = realloc(*out, new_cap);
            if (!p) { free(*out); return 0; }
            *out = p;
            cap = new_cap;
        }
        if (ch == '\n') break;
        (*out)[len++] = (char)ch;
    }
    if (ch == EOF && len == 0) return 0;
    (*out)[len] = '\0';
    return 1;
}

int in_read_int(const char *prompt, int min, int max, int *out) {
    for (;;) {
        if (prompt) printf("%s", prompt);
        int value;
        if (scanf("%d", &value) == 1 && value >= min && value <= max) {
            while (getchar() != '\n');
            *out = value;
            return 1;
        }
        printf("Ошибка ввода. Попробуйте ещё раз.\n");
        while (getchar() != '\n');
    }
}

int in_ask_yes_no(const char *prompt) {
    char ch;
    for (;;) {
        printf("%s (y/n): ", prompt);
        if (scanf(" %c", &ch) != 1) continue;
        while (getchar() != '\n');
        if (ch == 'y' || ch == 'Y') return 1;
        if (ch == 'n' || ch == 'N') return 0;
        printf("Введите y или n.\n");
    }
}
