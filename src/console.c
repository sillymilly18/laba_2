#include "console.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "utils.h"

int client_add(ClientList *list, const char *name, const char *company, const char *email, const char *phone, const char *status, int *out_id) {
    if (!ensure_client_list_capacity(list, list->count + 1)) return 0;

    const Client c = {
        .id = list->next_id++,
        .name = copy_string(name),
        .company = copy_string(company),
        .email = copy_string(email),
        .phone = copy_string(phone),
        .status = copy_string(status)
    };

    if (!c.name || !c.company || !c.email || !c.phone || !c.status) {
        free_client(&c);
        return 0;
    }

    list->data[list->count++] = c;
    if (out_id) *out_id = c.id;

    return 1;
}

int client_index_by_id(const ClientList *list, int id) {
    for (size_t i = 0; i < list->count; i++) {
        if (list->data[i].id == id) return (int)i;
    }

    return -1;
}

int client_remove_at(ClientList *list, const size_t index) {
    if (index >= list->count) return 0;

    const Client *client = &list->data[index];
    free_client(client);

    for (size_t i = index + 1; i < list->count; i++) {
        list->data[i - 1] = list->data[i];
    }

    list->count--;

    return 1;
}

int client_update_at(ClientList *list, size_t index, const char *new_name, const char *new_company, const char *new_email, const char *new_phone, const char *new_status) {
    if (index >= list->count) return 0;      // когда пользователь выбрал несуществующего клиента.

    Client *c = &list->data[index];

    if (new_name) {
        free(c->name);

        c->name = copy_string(new_name);
    }

    if (new_company) {
        free(c->company);

        c->company = copy_string(new_company);
    }

    if (new_email) {
        free(c->email);

        c->email = copy_string(new_email);
    }

    if (new_phone) {
        free(c->phone);

        c->phone = copy_string(new_phone);
    }

    if (new_status) {
        free(c->status);

        c->status = copy_string(new_status);
    }

    return 1;
}

void print_client(const ClientList *list, size_t index) {
    if (index >= list->count) {
     return;
    }
    const Client client = list->data[index];

    printf("\n📇 Клиент #%d\n", client.id);
    printf("Имя: %s\nКомпания: %s\nEmail: %s\nТелефон: %s\nСтатус: %s\n", client.name, client.company, client.email, client.phone, client.status);
}

void print_all_clients(const ClientList *list) {
    if (list->count == 0) {
        printf("\n[База клиентов пуста]\n");

        return;
    }

    printf("\n📁 Все клиенты:\n");
    for (size_t i = 0; i < list->count; i++) {
        print_client(list, i);
    }
}

void print_clients_menu(ClientList *clients, DealList *deals) {
    while (true) {
        printf("\n== Клиенты ==\n");
        printf("1) Добавить\n");
        printf("2) Удалить\n");
        printf("3) Изменить\n");
        printf("4) Показать одного\n");
        printf("5) Показать всех\n");
        printf("6) Назад\n");
        int ch;
        in_read_int("Выбор: ", 1, 6, &ch);

        if (ch == 1) {
            char *name=NULL, *company=NULL, *email=NULL, *phone=NULL, *status=NULL;

            printf("Имя: ");
            in_read_line(&name);

            printf("Компания: ");
            in_read_line(&company);

            printf("Email: ");
            in_read_line(&email);

            printf("Телефон: ");
            in_read_line(&phone);

            printf("Статус: ");
            in_read_line(&status);

            int id;
            if (client_add(clients, name, company, email, phone, status, &id)) {
                printf("Клиент добавлен (ID: %d)\n", id);
            } else {
                printf("Ошибка добавления\n");
            }

            free(name); free(company); free(email); free(phone); free(status);
        } else if (ch == 2) {
            if (clients->count == 0) {
                printf("Список пуст.\n");

                continue;
            }

            int id;
            in_read_int("ID клиента: ", 1, 1000000000, &id);

            int idx = client_index_by_id(clients, id);
            if (idx < 0) {
                printf("Клиент не найден.\n");

                continue;
            }

            printf("К удалению:\n");
            print_client(clients, (size_t)idx);

            if (!in_ask_yes_no("Удалить клиента и все его сделки?")) {
                printf("Отменено.\n");

                continue;
            }

            size_t removed = dl_remove_by_client(deals, id);

            client_remove_at(clients, (size_t)idx);
            printf("Клиент удалён. Каскадом удалено сделок: %zu\n", removed);
        } else if (ch == 3) {
            if (clients->count == 0) {
                printf("Список пуст.\n");

                continue;
            }

            int id;
            in_read_int("ID клиента: ", 1, 1000000000, &id);

            int idx = client_index_by_id(clients, id);
            if (idx < 0) {
                printf("Клиент не найден.\n");

                continue;
            }

            char *name=NULL, *company=NULL, *email=NULL, *phone=NULL, *status=NULL;
            if (in_ask_yes_no("Изменить имя?")) {
                printf("Новое имя: ");

                in_read_line(&name);
            }
            if (in_ask_yes_no("Изменить компанию?")) {
                printf("Новая компания: ");

                in_read_line(&company);
            }
            if (in_ask_yes_no("Изменить email?")) {
                printf("Новый email: ");

                in_read_line(&email);
            }
            if (in_ask_yes_no("Изменить телефон?")) {
                printf("Новый телефон: ");

                in_read_line(&phone);
            }
            if (in_ask_yes_no("Изменить статус?")) {
                printf("Новый статус: ");

                in_read_line(&status);
            }

            client_update_at(clients, (size_t)idx, name, company, email, phone, status);
            printf("Обновлено.\n");

            free(name); free(company); free(email); free(phone); free(status);
        } else if (ch == 4) {
            int id;
            in_read_int("ID клиента: ", 1, 1000000000, &id);

            int idx = client_index_by_id(clients, id);

            idx < 0 ? printf("Клиент не найден.\n") : print_client(clients, (size_t)idx);
        } else if (ch == 5) {
            print_all_clients(clients);
        } else { // 6
            return;
        }
    }
}


int client_deal_add(DealList *dl, int client_id, const char *title,
           const char *description, double amount, DealStatus st, int *out_id)
{
  if(!ensure_deal_list_capacity(dl, dl->count+1)) return 0;

  const Deal d = {
    .id = dl->next_id++,
    .client_id = client_id,
    .title = copy_string(title),
    .description = copy_string(description),
    .amount = amount,
    .status = st
  };

  if(!d.title || !d.description) {
    free_deal(&d);

    return 0;
  }

  dl->data[dl->count++]=d;
  if(out_id) *out_id=d.id;

  return 1;
}

int deal_index_by_id(const DealList *dl, int id){
  for(size_t i=0;i<dl->count;++i) {
    if(dl->data[i].id==id) {
      return (int)i;
    };
  };

  return -1;
}

int deal_remove_at(DealList *dl, const size_t index) {
  if(index>=dl->count) {
    return 0;
  }

  const Deal deal = dl->data[index];
  free_deal(&deal);

  for(size_t i=index+1;i<dl->count;++i) {
    dl->data[i-1]=dl->data[i];
  };

  dl->count--;

  return 1;
}

int deal_update_at(const DealList *dl, const size_t index,
                 const int *new_client_id, const char *new_title,
                 const char *new_description, const double *new_amount,
                 const DealStatus *new_status) {
  if(index>=dl->count) return 0;

  Deal *d=&dl->data[index];

  if(new_client_id) {
    d->client_id = *new_client_id;
  }

  if(new_title) {
    char *t=copy_string(new_title);
    if(!t) return 0;

    free(d->title);

    d->title=t;
  }

  if(new_description) {
    char *t=copy_string(new_description);
    if(!t) return 0;

    free(d->description);

    d->description=t;
  }

  if(new_amount) {
    d->amount = *new_amount;
  }

  if(new_status) {
    d->status = *new_status;
  }

  return 1;
}

void print_deal(const DealList *dl, size_t index){
  if(index>=dl->count) {
    return;
  }

  const Deal *d = &dl->data[index];

  printf("ID: %d | КлиентID: %d | \"%s\" | %.2f | Статус: %s\nОписание: %s\n",
         d->id, d->client_id, d->title, d->amount, st_name(d->status), d->description);
}

void print_all_deals(const DealList *dl){
  if(dl->count == 0) {
    printf("[сделок нет]\n");
    return;
  }

  for(size_t i = 0; i < dl->count; ++i){
    print_deal(dl, i);
  }
}

void print_client_deals(const DealList *dl, int client_id) {
  bool found = false;

  for(size_t i = 0; i < dl->count; ++i) {
    if(dl->data[i].client_id != client_id) {
      continue;
    }

    print_deal(dl,i);

    found=true;
  }

  if(!found) printf("[для клиента %d сделок нет]\n", client_id);
}

void print_deals_by_status(const DealList *dl, DealStatus st){
  bool found = false;

  for(size_t i=0;i<dl->count;++i) {
    if(dl->data[i].status != st) {
      continue;
    }

    print_deal(dl,i);

    found = true;
  }

  if(!found) {
    printf("[сделок со статусом нет]\n");
  }
}

size_t dl_remove_by_client(DealList *dl, int client_id){
  size_t removed=0;
  for(size_t i = 0; i<dl->count;){
    if(dl->data[i].client_id == client_id) {
      deal_remove_at(dl,i);
      ++removed;
    } else {
      ++i;
    };
  }

  return removed;
}

void deals_add(const ClientList *clients, DealList *deals) {
  if (clients->count == 0) {
    printf("Сначала добавьте клиента.\n");

    return;
  }

  int cid;
  in_read_int("ID клиента: ", 1, 1000000000, &cid);

  if (client_index_by_id(clients, cid) < 0) {
    printf("Клиент не найден.\n");

    return;
  }

  char *title=NULL, *desc=NULL;
  printf("Название сделки: ");
  in_read_line(&title);

  printf("Описание: ");
  in_read_line(&desc);

  printf("Сумма (можно 0): ");
  double amount = 0.0;
  if (scanf("%lf", &amount) != 1) amount = 0.0;

  flush_stdin_line();

  DealStatus st = (DealStatus)read_status();
  int id;
  client_deal_add(deals, cid, title ? title : "", desc ? desc : "", amount, st, &id) ? printf("Сделка добавлена (ID=%d)\n", id) :  printf("Ошибка добавления сделки\n");

  free(title);
  free(desc);
}

void deals_change_status(const DealList *deals) {
  if (deals->count == 0) {
    printf("Сделок нет.\n");

    return;
  }

  int did;
  in_read_int("ID сделки: ", 1, 1000000000, &did);

  int idx = deal_index_by_id(deals, did);
  if (idx < 0) {
    printf("Не найдена.\n");

    return;
  }

  const DealStatus st = (DealStatus)read_status();
  printf(deal_update_at(deals, (size_t)idx, NULL, NULL, NULL, NULL, &st) ?  "Статус обновлён\n" : "Ошибка\n");
}

void deals_edit(const DealList *deals, const ClientList *clients) {
  if (deals->count == 0) {
    printf("Сделок нет.\n");

    return;
  }

  int did;
  in_read_int("ID сделки: ", 1, 1000000000, &did);

  int idx = deal_index_by_id(deals, did);
  if (idx < 0) {
    printf("Не найдена.\n");

    return;
  }

  int new_cid;
  int *cid_ptr = NULL;
  char *title=NULL;
  char *desc=NULL;
  double amount;
  double *amount_ptr = NULL;
  DealStatus st;
  DealStatus *st_ptr = NULL;

    if (in_ask_yes_no("Сменить клиента?")) {
      in_read_int("Новый client_id: ", 1, 1000000000, &new_cid);
      if (client_index_by_id(clients, new_cid) < 0) {
        printf("Такого клиента нет.\n");
        return;
      }

      cid_ptr = &new_cid;
    }

  if (in_ask_yes_no("Изменить название?")) {
    printf("Новое название: ");
    in_read_line(&title);
  }

  if (in_ask_yes_no("Изменить описание?")) {
    printf("Новое описание: ");
    in_read_line(&desc);
  }

  if (in_ask_yes_no("Изменить сумму?")) {
    printf("Новая сумма: ");
    if (scanf("%lf",&amount)!=1) amount=0.0;

    flush_stdin_line();

    amount_ptr=&amount;
  }

  if (in_ask_yes_no("Изменить статус?")) {
    st=(DealStatus)read_status();

    st_ptr=&st;
  }

  printf(deal_update_at(deals, (size_t)idx, cid_ptr, title, desc, amount_ptr, st_ptr) ? "Сделка обновлена\n" : "Ошибка\n");

  free(title);
  free(desc);
}

void deals_delete(DealList *deals) {
  if (deals->count == 0) {
    printf("Сделок нет.\n");

    return;
  }

  int did; in_read_int("ID сделки: ", 1, 1000000000, &did);

  int idx = deal_index_by_id(deals, did);

  if (idx < 0) {
    printf("Не найдена.\n");

    return;
  }

  if (in_ask_yes_no("Удалить сделку?")) {
    printf(deal_remove_at(deals, (size_t)idx) ? "Удалена\n" : "Ошибка\n");
  }
}

void print_deals_menu(ClientList *clients, DealList *deals) {
    while (true) {
      printf("\n== Сделки ==\n");
      printf("1) Добавить\n");
      printf("2) Изменить статус\n");
      printf("3) Редактировать сделку\n");
      printf("4) Удалить\n");
      printf("5) Показать все\n");
      printf("6) Назад\n");
      int ch;
      in_read_int("Выбор: ", 1, 6, &ch);

      switch (ch) {
        case 1:
          deals_add(clients, deals);
          continue;
        case 2:
          deals_change_status(deals);
          continue;
        case 3:
          deals_edit(deals, clients);
          continue;
        case 4:
          deals_delete(deals);
          continue;
        case 5:
          print_all_deals(deals);
          continue;
        default:
          return;
      }
    }
}