#pragma once

#include <stddef.h>

struct DealList;

typedef struct Client {
    int id;
    char *name;
    char *company;
    char *email;
    char *phone;
    char *status;      // Статус: потенциальный / в работе / закрыт
} Client;

typedef struct ClientList {
    Client *data;      // Динамический массив клиентов
    size_t count;       // Текущее количество клиентов
    size_t capacity;   // Ёмкость массива
    int next_id;       // Счётчик ID
} ClientList;

// Инициализация и очистка памяти
void cl_init(ClientList *list);
void cl_free(ClientList *list);

int ensure_client_list_capacity(ClientList *list, size_t needed);
void free_client(const Client *c);