#pragma once
#include <stddef.h>

struct ClientList;

/* Статус сделки */
typedef enum {
    DS_TODO = 0,        // запланирована
    DS_IN_PROGRESS = 1, // в работе
    DS_DONE = 2,        // завершена (успешно)
    DS_CANCELED = 3     // отменена / неуспех
} DealStatus;

/* Одна сделка/таск, всегда привязана к клиенту */
typedef struct Deal {
    int   id;          // уникальный ID сделки
    int   client_id;   // ID клиента-владельца (ссылка на Client.id)
    char *title;       // краткое название
    char *description; // описание/заметки
    double amount;     // сумма (можно 0, если не нужно)
    DealStatus status; // статус сделки
} Deal;

typedef struct DealList {
    Deal   *data;
    size_t  count;
    size_t  capacity;
    int     next_id;
} DealList;

/* Инициализация/очистка */
void dl_init(DealList *dl);
void dl_free(DealList *dl);

int ensure_deal_list_capacity(DealList *list, const size_t needed);
void free_deal(const Deal *d);
const char* st_name(const DealStatus s);
int read_status(void);
