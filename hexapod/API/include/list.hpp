#pragma once

struct list_elem {
    list_elem *next;
    void      *data;
};

class list {
  public:
    list(): start(0), end(0) {}
    list(void *data): start(0), end(0) {
      push_back(data);
    }

    void *push_back(void * data);
    void insert(list *l);
    void *pop();
    bool empty() {
      if (start != 0)
        return true;
      return false;
    }


    list_elem *start;
  private:
    list_elem *end;
};

