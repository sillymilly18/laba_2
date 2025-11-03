#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *copy_string(const char *s) {
    if (!s) return NULL;

    char *copy = malloc(strlen(s) + 1);
    if (copy) strcpy(copy, s);

    return copy;
}

void flush_stdin_line(void) {
    int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
}