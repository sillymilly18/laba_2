#include "client.h"
#include <stdlib.h>

void init_clients_list(ClientList *list) {
    list->data = NULL;
    list->count = 0;
    list->capacity = 0;
    list->next_id = 1;
}

int ensure_client_list_capacity(ClientList *list, size_t needed) {
    if (list->capacity >= needed) return 1;

    size_t new_cap = list->capacity ? list->capacity * 2 : 4;
    if (new_cap < needed) new_cap = needed;

    Client *p = realloc(list->data, new_cap * sizeof(Client));
    if (!p) return 0;

    list->data = p;
    list->capacity = new_cap;

    return 1;
}

void free_client(const Client *c) {
    free(c->name); free(c->company); free(c->email); free(c->phone); free(c->status);
}

void free_clients_list(ClientList *list) {
    for (size_t i = 0; i < list->count; i++) {
        const Client *client = &list->data[i];
        free_client(client);
    }

    free(list->data);
    list->data = NULL;
    list->count = list->capacity = 0;
}

