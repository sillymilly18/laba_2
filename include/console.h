#pragma once

#include "client.h"
#include "deal.h"
#include <stddef.h>

// CRUD операции
static int  cl_add(ClientList *list, const char *name, const char *company, const char *email, const char *phone, const char *status, int *out_id);
int  cl_index_by_id(const ClientList *list, int id);
static int  cl_remove_at(ClientList *list, size_t index);
static int  cl_update_at(ClientList *list, size_t index, const char *new_name, const char *new_company, const char *new_email, const char *new_phone, const char *new_status);

static int  dl_add(DealList *dl, int client_id, const char *title,
            const char *description, double amount, DealStatus st, int *out_id);
static int  dl_index_by_id(const DealList *dl, int id);
static int  dl_remove_at(DealList *dl, size_t index);
static int  dl_update_at(const DealList *dl, size_t index,
                  const int *new_client_id, const char *new_title,
                  const char *new_description, const double *new_amount,
                  const DealStatus *new_status);

size_t dl_remove_by_client(DealList *dl, int client_id); // возвращает, сколько удалено


// Вывод информации
static void client_print(const ClientList *list, size_t index);
static void cl_print_all(const ClientList *list);

void clients_menu(ClientList *clients, struct DealList *deals);

static void dl_print_one(const DealList *dl, size_t index);
void dl_print_all(const DealList *dl);
void dl_print_by_client(const DealList *dl, int client_id);
void dl_print_by_status(const DealList *dl, DealStatus st);

void deals_menu(struct ClientList *clients, DealList *deals);

