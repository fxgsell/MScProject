#include <stdio.h>

#include "list.hpp"
 
list::list(): start(0), end(0) {
}

list::list(void *data): start(0), end(0) {
  push_back(data);
}

bool list::clear() {
  list_elem* t;
  
  while (start != 0) {
    if (start->data)
      delete start->data;
    t = start->next;
    delete start;
    start = t;
  }
  end = 0;
  return true;
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
    if (!t)
      end = 0; 
    return ret;
  }
  return 0;
};

void list::print() {
  list_elem *cur;
   
  printf("List:\n");
  for (cur = this->start; cur != 0;) {
     printf("  Elem: %p\n", cur);
     cur = cur->next;
  }
  printf("  Elem: %p\n", cur);
}

void list::insert(list *l) {
  if (end)
    end->next = l->start;
  else {
    start = l->start;
  }
  end = l->end;
};

void *list::push_front(void *data) {
  list_elem *e;

  e = new list_elem;
  if (e) {
    e->data = data;
    e->next = start;
    start = e;
    if (!end)
      end = e;
  }
  return e;
};

void *list::push_back(void *data) {
  list_elem *e;

  e = new list_elem;
  if (e) {
    e->data = data;
    e->next = 0;
    if (start && end)
      end->next = e;
    else
      start = e;
    end = e;
  }
  return e;
};



