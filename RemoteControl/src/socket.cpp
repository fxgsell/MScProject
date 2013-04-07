#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "protocol.h"

packet p;

int initNet(char *av) {
  struct sockaddr_in si_other;
  int s;

  if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP))==-1)
    return 0;

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  if (inet_aton(av, &si_other.sin_addr) == 0) {
    return 0;
  }
  return s;
}

