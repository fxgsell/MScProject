
#include <iostream>

#include <errno.h>
#include <string.h>
#include "PackageManager.h"
#include "ProcessManager.h"
#include "Suscriber.hpp"
#include <unistd.h>
#include <fcntl.h>
#include "JsonParser.h"

PackageManager pkgMan;

void sig_handler(int signo)
{
  std::cout << "kill them all" << std::endl;
  if (signo == SIGINT)
    pkgMan.killThemAll();
  kill(getpid(), SIGQUIT);
  exit(-1);
}


void init(int ac, char **av) {
  if (signal(SIGINT, sig_handler) == SIG_ERR)
    printf("\ncan't catch SIGINT\n");
  freopen("launcher.log", "w+", stderr);
  ros::init(ac, av, "launcher");
}

void callback(const ros::MessageEvent<sensor_msgs::Image>&event) 
{ 
  std::cout << "package received" << std::endl;
}

int              main(int ac, char **av) {
  JsonParser     parser;
  std::string    cmd, topic, type;
  int            cpt;
  int            size = parser.getSize();;

  init(ac, av);

  std::cout << "...start..." << std::endl;
  while (parser.parseNextPackage(cmd, topic, type, cpt)) {
    std::cout << "launch package:" << cmd << "   " << topic << "   " << type <<std::endl;
      pkgMan.launchPackage(cmd, topic, type);
      std::cout << "Package " << cpt << "/" << size << " launched..." << std::endl;
      sleep(1);
    }
  std::cout << "< MscProject launched >" << std::endl;
  sleep(3);
  return 0;
}