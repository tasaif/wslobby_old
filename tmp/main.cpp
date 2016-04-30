#include "application.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t done = 0;

Application* app;
void term(int signum)
{
  done = 1;
  app->quit();
  delete app;
  exit(signum);
}


int main(int argc, char* argv[]) {
  struct sigaction action;
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = term;
  sigaction(SIGINT, &action, NULL);

  app = new Application;

  if (app->init("config.json")){
    app->run();
  }
  // This should never happen
  return -1;
}
