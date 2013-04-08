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
  int sock;
  struct sockaddr_in echoserver;

  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    return 0;

  memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
  echoserver.sin_family = AF_INET;                  /* Internet/IP */
  echoserver.sin_addr.s_addr = inet_addr(av);       /* IP address */
  echoserver.sin_port = htons(PORT);                /* server port */

  /* Establish connection */
  if (connect(sock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {
          perror("socket");
          return 0;
  }


  return sock;
}

