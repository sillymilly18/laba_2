#include "deal.h"
#include "input.h"
#include <stdlib.h>

void init_deals_list(DealList *dl) {
  dl->data = NULL;
  dl->count = 0;
  dl->capacity = 0;
  dl->next_id = 1;
}

int ensure_deal_list_capacity(DealList *list, const size_t needed){
  if (list->capacity >= needed) return 1;

  size_t new_cap = list->capacity ? list->capacity * 2 : 4;
  if (new_cap < needed) new_cap = needed;

  Deal *p = realloc(list->data, new_cap * sizeof(Deal));
  if (!p) return 0;

  list->data = p;
  list->capacity = new_cap;

  return 1;
}

void free_deal(const Deal *d) {
  if(!d) return;

  free(d->title); free(d->description);
}

void free_deals_list(DealList *dl) {
  if (!dl) {
    return;
  }

  for (size_t i = 0; i < dl->count; ++i) {
    free_deal(&dl->data[i]);
  }

  free(dl->data);

  dl->data = NULL;
  dl->count = 0;
  dl->capacity = 0;
  dl->next_id = 1;
}

const char* st_name(const DealStatus s){
  switch(s){
    case DS_TODO: return "запланирована";
    case DS_IN_PROGRESS: return "в работе";
    case DS_DONE: return "завершена";
    case DS_CANCELED: return "отменена";
    default: return "?";
  }
}

int read_status(void) {
  int s;
  in_read_int("Статус (0=TODO,1=IN_PROGRESS,2=DONE,3=CANCELED): ", 0, 3, &s);

  return s;
}