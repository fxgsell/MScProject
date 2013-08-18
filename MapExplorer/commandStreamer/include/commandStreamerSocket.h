
#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "protocol.h"

class CommandStreamerSocket {
 private:
  int _sock;
 public:
  CommandStreamerSocket(const char *addr);
  ~CommandStreamerSocket();

  void sendPacket(packet const &p);
};
