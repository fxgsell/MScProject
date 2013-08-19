
#include <iostream>

#include <errno.h>
#include <string.h>
#include "PackageManager.h"
#include "ProcessManager.h"
#include "Subscriber.hpp"
#include <unistd.h>
#include <fcntl.h>
#include "JsonParser.h"

int              main(int ac, char **av) {
  PackageManager pkgMan;
  JsonParser     parser;
  std::string    cmd, topic, type;
  int            cpt;
  int            size = parser.getSize();;

  while (parser.parseNextPackage(cmd, topic, type, cpt)) {
    std::cout << "launch package:" << cmd << "   " << topic << "   " << type <<std::endl;
    pkgMan.launchPackage(cmd, topic, type);
    std::cout << "Package " << cpt << "/" << size << " launched..." << std::endl;
    sleep(2);
  }
  std::cout << std::endl << "<<< MscProject launched >>>" << std::endl << std::endl;
  return 0;
}
