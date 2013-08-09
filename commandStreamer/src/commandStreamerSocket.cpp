
#include <iostream>
#include <stdlib.h>
#include "commandStreamerSocket.h"

CommandStreamerSocket::CommandStreamerSocket(const char *addr) {
  int sock;
  struct sockaddr_in echoserver;

  if ((_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    std::cerr << "Cannot create a new socket... exit" << std::endl;
    exit(-1);
  }

  memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
  echoserver.sin_family = AF_INET;                  /* Internet/IP */
  echoserver.sin_addr.s_addr = inet_addr(addr);       /* IP address */
  echoserver.sin_port = htons(PORT);                /* server port */

  /* Establish connection */
  if (connect(_sock, (struct sockaddr *) &echoserver,
	      sizeof(echoserver)) < 0) {
    perror("socket");
    exit(-1);
  }
  if (send(_sock, "CLIENT", sizeof("CLIENT"), 0) == -1)
    fprintf(stderr, "Error: send packet failed\n");
}

CommandStreamerSocket::~CommandStreamerSocket() {
}

void CommandStreamerSocket::sendPacket(packet const &p) {
    if (send(_sock, &p, sizeof(p), 0) == -1)
      fprintf(stderr, "Error: send packet failed\n");
}
