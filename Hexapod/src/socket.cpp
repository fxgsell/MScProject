#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>

#include "action.hpp"
#include "socket.hpp"
#include "robot.hpp"

extern Body *robot;

fd_set  fd_read;
fd_set  fd_write;

s_fd fds[128];
int cls;
int fdserial;
int fdviewer;

int die (const char *s) {
  perror(s);
  return -1;
}
  
void      clean_fd(int fd)
{
  fds[fd].type = FD_FREE;
  fds[fd].fct_read = NULL;
  fds[fd].fct_write = NULL;
  fds[fd].buf_read.clear();
  fds[fd].buf_write.clear();
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
    //  robot->addAction(standUp());
    //  printf("Recv Up!\n");
    }
    else if (buf.flags & B01) {
    //  printf("Recv Shot!\n");
    }
    else if (buf.flags & B12) {
    //  robot->addAction(standDown());
    //  printf("Recv Down!\n");
    }
    else if (buf.flags & B06) {
    //  printf("Recv clear!\n");
    }
    else if (buf.flags & B05) {
      robot->events.clear();
    //  printf("Recv clear!\n");
    }
    if (robot->turn != buf.turn)  {
      robot->turn = buf.turn;
      printf("Recv Turn:%d\n", buf.turn);
    }
    if (robot->height != buf.height)  {
      robot->height = buf.height;
    //  robot->addAction(updateHeight());
    //  printf("Recv Height:%d\n", buf.height);
    }
    if (buf.y != robot->y)  {
      robot->y = buf.y;
      printf("Recv y:%d\n", buf.y);
    }
    if (buf.x != -robot->x)  {
      robot->x = -buf.x;
      printf("Recv x:%d\n", buf.x);
    }
  }
}

enum SerialReadStatus {HEAD = 0, BODYSENS = 1, BODYMAP = 2, TAIL = 3};

void serial_write(int fd) {
  static int len = 0;
  static const char *wbuf = "";

  if (len == 0) {
    if (!fds[fd].buf_write.empty()) {
      wbuf = (char *)fds[fd].buf_write.start->data;
    }
    else
      return;
  }
  else if (!wbuf[len]) {
    delete (char *)fds[fd].buf_write.pop();
    len = 0;
    return;
  }

  int i = write(fd, wbuf+len, 1);
  #ifdef DEBUG
  if (wbuf[len] == '\x0d')
    printf("\\x0d\n");
  else
    printf("%c", wbuf[len]);
  #endif
  len++;
  
  if (i < 0) {
    fprintf (stderr, "Serial: Read error: %s\n", strerror (errno));
  } else if (!i) {
    fprintf (stderr, "Serial: Unexpected EOF\n");
  }
}


void serial_read(int fd) {
  unsigned char rbuf = 0;

  static SerialReadStatus status = HEAD;
  static int total_len = 0;
  static int stop = 0;

  int i = read (fd, &rbuf, 1);
  
  if (i < 0) {
    fprintf (stderr, "Serial: Read error: %s\n", strerror (errno));
  } else if (!i) {
    fprintf (stderr, "Serial: Unexpected EOF\n");
  } else {
    if (status == HEAD){
      if (rbuf == 'R') {
        printf ("%c: ", rbuf);
        status = BODYSENS;
        stop = 50;
      }
      if (rbuf == 'P') {
        printf ("%c: ", rbuf);
        status = BODYMAP;
        stop = 100;
      }
      else
        return;
    } else if (status == TAIL) {
      printf (" : %c\n", rbuf);
      status = HEAD;
      total_len = 0;
    } else if (status == BODYMAP) {
      total_len++;
      printf ("%c", rbuf);
    } else if (status == BODYSENS) {
      total_len++;
      printf ("%c", rbuf);
    }
    if (total_len == stop)
      status = TAIL;
  }
}

void viewer_write(int fd) {
  char *wbuf = 0;

  if (!fds[fd].buf_write.empty()) 
    wbuf = (char *)fds[fd].buf_write.pop();
  else
    return;

  int i = write(fd, wbuf, strlen(wbuf));
  printf("Sending: %s\n", wbuf);
  delete wbuf;
  
  if (i < 0) {
    fprintf (stderr, "Serial: Read error: %s\n", strerror (errno));
  } else if (!i) {
    fprintf (stderr, "Serial: Unexpected EOF\n");
  }
}

void      new_read(int fd)
{
  char rbuf[6];
  int i = read (fd, &rbuf, 6);

  if (i < 0) {
    fprintf (stderr, "Socket: Read error: %s\n", strerror (errno));
  } else if (!i) {
    clean_fd(fd);
  } else {
    printf("read New\n");
    if (!strncmp(rbuf, "VIEWER", 6)) {
      printf("New viewer\n");
      fds[fd].type = FD_VIEWER;
      fdviewer = fd;
      fds[fd].fct_write = viewer_write;
    }
    else if (!strncmp(rbuf, "CLIENT", 6)) {
      printf("New client\n");
      fds[fd].type = FD_CLIENT;
      fds[fd].fct_read = client_read;
      //fds[fd].fct_write = client_write;
    }
    fds[fd].buf_read.clear();
    fds[fd].buf_write.clear();
    ++cls;
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
      if (!fds[i].buf_write.empty()) {
        FD_SET(i, &fd_write);
      }
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
    if (FD_ISSET(i, &fd_write))
      fds[i].fct_write(i);
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
  fds[cs].type = FD_NEW;
  fds[cs].fct_read = new_read;
  fds[cs].buf_read.clear();
  fds[cs].buf_write.clear();
  ++cls;
}


int       srv_create(int port, int serialfd)
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
  if (setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on)) == -1)
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
  fds[s].buf_read.clear();
  fds[s].buf_write.clear();
  printf("Serial fd:%d, Server fd:%d\n",serialfd, s);
  fdserial = serialfd;
  fds[serialfd].type = FD_SERIAL;
  fds[serialfd].fct_read = serial_read;
  fds[serialfd].fct_write = serial_write;
  fds[serialfd].buf_read.clear();
  fds[serialfd].buf_write.clear();
  return (0);
}




