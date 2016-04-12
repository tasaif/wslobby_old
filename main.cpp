#include "global.h"
#include "application.h"


int main() {
  Application* app;

  app = new Application;

  if (app->init("config.json")){
    app->run();
  }
  app->quit();
}
