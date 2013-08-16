
#include "PackageManager.h"
#include <signal.h>

ProcessManager::ProcessManager() {}
ProcessManager::~ProcessManager() {
  for (std::map<int, FILE*>::iterator it = _process.begin();
       it != _process.end();++it)
    fclose(it->second);
}

void ProcessManager::killThemAll() {
  for (std::map<int, FILE*>::iterator it = _process.begin();
       it != _process.end();++it)
  kill(it->first, SIGKILL);
}

void ProcessManager::launchCommand(std::string const &cmd) {
  char tampon[256];
  FILE *output;

  if ((_process[getpid()] = popen(cmd.c_str(), "r")) == NULL) 
    std::cout <<"[ERROR] " << strerror(errno) << std::endl;
  output = _process[getpid()];
  //while (fgets (tampon, sizeof tampon, output) != NULL)
  //  std::cout << "        " << tampon << std::endl;
  //fputs (tampon, stdout);
  exit(0);
}

void  ProcessManager::launchNewPackage(std::string const &cmd) {
  int pid;

  switch(pid = fork()) {
  case -1:
    std::cout << "[ERROR]: fork failed" << std::endl;
    exit(EXIT_FAILURE);  
  case 0:
    launchCommand(cmd);
    break;
  }
}

