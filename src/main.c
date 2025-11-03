#include <stdio.h>
#include <stdbool.h>
#include "client.h"
#include "console.h"
#include "input.h"
#include "deal.h"

//набор именованных констант
typedef enum {
    MENU_CLIENTS = 1,
    MENU_DEALS,
    MENU_FILTERS,
    MENU_EXIT
} RootMenu;

static void print_root_menu(void) {
    printf("\n===== CRM Light =====\n");
    printf("1) Клиенты\n");
    printf("2) Сделки\n");
    printf("3) Фильтры\n");
    printf("4) Выход\n");
}

//фильтры
static void filters_menu(const ClientList *clients, const DealList *deals) {
    while (true) {
        printf("\n== Фильтры ==\n");
        printf("1) Сделки клиента\n");
        printf("2) Сделки по статусу\n");
        printf("3) Назад\n");
        int ch; in_read_int("Выбор: ", 1, 3, &ch);      //хранение выбора

        if (ch == 1) {
            int cid; in_read_int("ID клиента: ", 1, 1000000000, &cid);
            if (cl_index_by_id(clients, cid) < 0) printf("Клиент не найден.\n");
            else dl_print_by_client(deals, cid);
        } else if (ch == 2) {
            DealStatus st = (DealStatus)read_status();
            dl_print_by_status(deals, st);
        } else return;
    }
}

int main(void) {
    ClientList clients;
    cl_init(&clients);

    DealList   deals;
    dl_init(&deals);

    while (true) {
        print_root_menu();
        int choice;
        in_read_int("Выбор: ", 1, 4, &choice);

        if (choice == MENU_CLIENTS) {
            clients_menu(&clients, &deals);
        } else if (choice == MENU_DEALS) {
            deals_menu(&clients, &deals);
        } else if (choice == MENU_FILTERS) {
            filters_menu(&clients, &deals);
        } else if (choice == MENU_EXIT) {
            if (in_ask_yes_no("Выйти и очистить ресурсы?")) break;
        }
    }

    dl_free(&deals);
    cl_free(&clients);
    printf("📁 Память очищена. Завершение программы.\n");
}
