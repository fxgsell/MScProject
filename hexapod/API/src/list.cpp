#include <stdio.h>

#include "list.hpp"
 
list::list(): start(0), end(0) {
}

list::list(void *data): start(0), end(0) {
  push_back(data);
}

bool list::empty() {
  if (start == 0)
    return true;
  return false;
}


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

void list::print() {
  list_elem *cur;
   
  for (cur = this->start; cur != 0;) {
     printf("Elem: %p\n", cur);
     cur = cur->next;
  }
  printf("Elem: %p\n", cur);
}

void list::insert(list *l) {
  if (l->end)
    l->end->next = start;
  else
    l->end = start;
  start = l->start;
};

void *list::push_front(void *data) {
  list_elem *e;

  e = new list_elem;
  if (e) {
    e->data = data;
    if (start)
      e->next = start;
    else
      e->next = 0;
    start = e;
  }
  return e;
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



