#include "application.h"

Application::Application(){
}

Application::~Application(){
}

bool Application::init(string _config_fname){
  __config = new Config();
  __config->load(_config_fname);
}

void Application::run(){
  cout << __config->port() << endl;
}

void Application::quit(){
  delete __config;
}
