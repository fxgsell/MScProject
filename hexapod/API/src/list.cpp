#include "list.hpp"

void  *list::pop() {
  if (start) {
    void *ret = start->data;
    list_elem *t = start->next;
    delete start;
    start = t;
    return ret;
  }
  return 0;
};


void list::insert(list *l){
  if (l->end)
    l->end->next = start;
  else
    l->end = start;
  start = l->start;
};


void *list::push_back(void *data) {
  list_elem *e;

  e = new list_elem;
  if (e) {
    e->data = data;
    e->next = 0;
    if (end)
      end->next = e;
    else
      start = e;
    end = e;
  }
  return e;
};



