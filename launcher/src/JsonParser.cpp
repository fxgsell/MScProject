
#include "JsonParser.h"

#include <map>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"

#include <boost/filesystem.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/assign.hpp>

JsonParser::JsonParser(std::string const &filename): cpt(0) {
  parseFile(filename);
}

JsonParser::~JsonParser() {
}

void JsonParser::parseFile(std::string const &file) {
  char readBuffer[65536];
  FILE *f = fopen(file.c_str(), "r");

  if (!f)
    throw std::runtime_error("could not open " + file);
  FileReadStream is(f, readBuffer, sizeof(readBuffer));

  d.ParseStream<0, UTF8<> >(is);
  fclose(f);
  if (d.HasParseError()) {
    throw std::runtime_error(file + " : " + d.GetParseError());
  }
}

bool JsonParser::parseNextPackage(std::string &cmd, std::string &topic, std::string &type, int &c) {
  Value &data = d["LaunchProcess"];
  
  if (cpt >= data.Size())
    return (false);

  Value &current = data[cpt];
  
  cmd = current["cmd"].GetString();
  topic = current["topicName"].GetString();
  type = current["topicType"].GetString();
  ++cpt;
  c = cpt;
  return (true);
}

