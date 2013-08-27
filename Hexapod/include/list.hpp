#pragma once

struct list_elem {
    list_elem *next;
    void *data;
};

class list {
  public:
    list();
    list( void *data);

    void *push_back( void * data);
    void *push_front( void * data);
    void insert(list *l);
    void *pop();
    bool empty();
    void print();
    bool clear();

    list_elem *start;
    list_elem *end;
};

