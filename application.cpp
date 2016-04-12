#include "application.h"


void Application::on_message(websocketpp::connection_hdl hdl, WS_Server::message_ptr msg) {
  std::cout << msg->get_payload() << std::endl;
}

Application::Application(){
}

Application::~Application(){
}

bool Application::init(string _config_fname){
 // __config = new Config();
 // __config->load(_config_fname);
  __wsserver = new WS_Server();
  return true;
}

void Application::run(){
    __wsserver->set_message_handler(&on_message);
    __wsserver->init_asio();
    __wsserver->listen(9002);
    __wsserver->start_accept();
    __wsserver->run();
}

void Application::quit(){
 // delete __config;
 delete __wsserver;
}

