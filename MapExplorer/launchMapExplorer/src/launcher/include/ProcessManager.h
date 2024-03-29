
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork
 
#include <sys/wait.h> //wait
#include <list>
#include <map>
#include <string>
#include <iostream>

#include "errno.h"
#include <string.h>


class ProcessManager {
 private:
  std::map<int, FILE*>   _process;

  void launchCommand(std::string const &cmd);

  void sensor_msgs_Image_callback(std::string const &cmd, std::string const &topic);

 public:
  ProcessManager();
  ~ProcessManager();

  void  launchNewPackage(std::string const &cmd);
  void killThemAll();
};
