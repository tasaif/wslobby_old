#include "config.h"
#include "global.h"

Config::Config(){
  __port = -1;
}

Config::~Config(){
}

void Config::load(string _fname){
  ifstream t(_fname.c_str());
  stringstream buffer;
  buffer << t.rdbuf();
  __fname = _fname;
  __file = buffer.str();
  __info = json::parse(__file);
  __port = __info["port"];
  cout << __fname << ": " << endl;
  cout << __file << endl;
}

int Config::port(){
  return __port;
}
