#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "action.hpp"
#include "socket.hpp"
#include "robot.hpp"

extern Body *robot;

fd_set  fd_read;
fd_set  fd_write;

s_fd fds[128];
int cls;

int die (const char *s) {
  perror(s);
  return -1;
}
  
void      clean_fd(int fd)
{
  fds[fd].type = FD_FREE;
  fds[fd].fct_read = NULL;
  fds[fd].fct_write = NULL;
  fds[fd].buf_write = 0;
  fds[fd].buf_read = 0;
}


void    client_read(int cs)
{
  int   r;
  packet   buf;

  bzero(&buf, sizeof(buf));
  r = recv(cs, &buf, sizeof(buf), 0);
  if (r <= 0) {
    close(cs);
    clean_fd(cs);
  }
  else {
    if (buf.flags & B11) {
   // robot->addAction(standUp());
      printf("Recv Up!\n");
    }
    else if (buf.flags & B01) {
      printf("Recv Shot!\n");
    }
    else if (buf.flags & B12) {
  //  robot->addAction(standDown());
      printf("Recv Down!\n");
    }
    else if (buf.flags & B06) {
      printf("Recv clear!\n");
    }
    else if (buf.flags & B05) {
      robot->events.clear();
      printf("Recv clear!\n");
    }
    
    if (robot->turn != buf.turn)  {
      robot->turn = buf.turn;
      printf("Recv Turn:%d, !\n", buf.turn);
    }
    if (robot->height != buf.height)  {
      robot->height = buf.height;
//      robot->addAction(updateHeight());
      printf("Recv Height:%d, !\n", buf.height);
    }
    if (buf.y != robot->y)  {
      robot->y = buf.y;
      printf("Recv y:%d, !\n", buf.y);
    }
    if (buf.x != robot->x)  {
      robot->x = buf.x;
      printf("Recv x:%d, !\n", buf.x);
    }
  }
}

int lastfd;

void      init_fd()
{ 
  int     i;

  i = 0;
  lastfd = 0;
  FD_ZERO(&fd_read);
  FD_ZERO(&fd_write);
  while (i < FDS)
  { 
    if (fds[i].type != FD_FREE)
    {
      FD_SET(i, &fd_read);
      //if (get_buf(&fds[i].buf_write))
      //  FD_SET(i, &fd_write);
      lastfd = MAX(lastfd, i); 
    }
    i++;
  }
}

void      check_fd(int r)
{ 
  int     i;

  i = 0;  
  while ((i < FDS) && (r > 0))
  {     
    if (FD_ISSET(i, &fd_read))
      fds[i].fct_read(i);
//    if (FD_ISSET(i, &e->fd_write))
//      e->fds[i].fct_write(e, i);
    if (FD_ISSET(i, &fd_read) || FD_ISSET(i, &fd_write))
      r--;
    i++;
  }
}

void      srv_accept(int s)
{
  int     cs;
  struct sockaddr_in  csin;
  unsigned int    csin_len;

  bzero(&csin, sizeof(csin));
  csin_len = 0;
  cs = accept(s, (struct sockaddr*)&csin, &csin_len);
  if (cs == -1)
  {
    perror("accept in srv_accept");
    return;
  }
  fds[cs].type = FD_CLIENT;
  fds[cs].fct_read = client_read;
  //fds[cs].fct_write = client_write;
  fds[cs].buf_read = 0;
  fds[cs].buf_write = 0;
  ++cls;
}


int       srv_create(int port)
{
  int     s;
  int     on;
  struct sockaddr_in sin;
  struct protoent *pe;

  cls = 0;
  if ((pe = getprotobyname("tcp")) == NULL)
    return (die("getprotobyname in srv_create"));
  if ((s = socket(PF_INET, SOCK_STREAM, pe->p_proto)) == -1)
    return (die("socket in srv_create"));
  on = 1;
  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
    return (die("setsockopt in srv_create"));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(port);
  if (bind(s, (struct sockaddr*)&sin, sizeof(sin)) == -1)
    return (die("bind in srv_create"));
  if (listen(s, 42) == -1)
    return (die("listen in srv_create"));
  fds[s].type = FD_SERV;
  fds[s].fct_read = srv_accept;
  return (0);
}




