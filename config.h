#ifndef CONFIG_H
#define CONFIG_H

#include "global.h"

class Config {
  private:
    int __port;
    string __fname;
    string __file;
    json __info;

  public:
    Config();
    ~Config();
    void load(string);
    int port();
};

#endif
