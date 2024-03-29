
#pragma once

#include "rapidjson/document.h"
#include <string>

using namespace rapidjson;

class JsonParser {
 private:
  Document d;
  int cpt;
  
  void parseFile(std::string const &);
 public:
  JsonParser(std::string const &filename = "launcher.msc");
  ~JsonParser();
  
  bool parseNextPackage(std::string &, std::string&, std::string &, int &);
  int getSize() {   Value &data = d["LaunchProcess"]; return data.Size();}
};
