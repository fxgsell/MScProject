#pragma once

#include <sys/select.h>

#include "list.hpp"
#include "../../../Protocol/protocol.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define FD_FREE    0
#define FD_SERV    1
#define FD_SERIAL  2
#define FD_NEW	   3
#define FD_CLIENT  4
#define FD_VIEWER  5

#define FDS 128

struct      s_fd
{
  int       fd;
  int       type;
  void      (*fct_read)(int);
  void      (*fct_write)(int);
  list      buf_write;
  list      buf_read;
};

extern s_fd fds[];

extern fd_set  fd_read;
extern fd_set  fd_write;

extern int cls;
extern int lastfd;


int       srv_create(int port, int serialfd);
void      init_fd();
void      check_fd(int r);
int       srv_create(int port);
