#ifndef APPLICATION_H
#define APPLICATION_H

#include "global.h"
#include "config.h"

class Application {
  private:
    Config* __config;

  public:
    Application();
    ~Application();
    bool init(string);
    void run();
    void quit();
};

#endif
