
#include "PackageManager.h"
#include <signal.h>
#include <sstream>
std::string toString(int n) {
  std::stringstream ss;
  ss << n;
  return (ss.str());
}


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
  char tampon[4096];
  //FILE *output;
  std::string fullCmd = cmd + std::string(" 2>&1 > logPakage") + toString(_process.size()) + std::string(".log");
  std::cout << fullCmd << std::endl;
  if ((_process[_process.size() + 1] = popen(fullCmd.c_str(), "re")) == NULL) 
    std::cout <<"[ERROR] " << strerror(errno) << std::endl;
  //output = _process[_process.size()];
  //while (fgets (tampon, sizeof tampon, output) != NULL)
  //  std::cout << ">>" << tampon << std::endl;
  //fputs (tampon, stdout);
  //exit(0);
}

void  ProcessManager::launchNewPackage(std::string const &cmd) {
  /*  int pid;

  switch(pid = fork()) {
  case -1:
    std::cout << "[ERROR]: fork failed" << std::endl;
    exit(EXIT_FAILURE);  
  case 0:
  */
  launchCommand(cmd);
  /*    break;
	}*/
}

