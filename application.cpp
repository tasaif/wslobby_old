#include "application.h"

WS_Server Application::__wsserver;

bool Application::on_connect(websocketpp::connection_hdl hdl){
  cout << "New Connection" << endl;
  return true;
}

void Application::on_message(websocketpp::connection_hdl hdl, WS_Server::message_ptr ws_msg) {
  string stringified_msg = ws_msg->get_payload();
  Json::Value msg;   // will contains the root value after parsing.
  Json::Reader reader;
  bool parsingSuccessful = reader.parse(stringified_msg, msg);
  if ( !parsingSuccessful )
  {
    std::cout << "Failed to parse request\n" << reader.getFormattedErrorMessages();
    return;
  } else {
    if (msg["request"] == "connect"){
      unsigned lobby_id = msg["lobby"].asInt();
      cout << "Requested to join lobby: " << lobby_id << endl;
    } else {
      cout << "Error: Unrecognized request." << endl;
      cout << "Terminating Connection" << endl;
      websocketpp::lib::error_code ec;
      __wsserver.close(hdl, websocketpp::close::status::normal, "Unrecognized request", ec);
      if (ec) {
        cout << ec.message() << endl;
      }
    }
  }
}

Application::Application(){
}

Application::~Application(){
}

bool Application::init(string _config_fname){
 // __config = new Config();
 // __config->load(_config_fname);
  return true;
}

void Application::run(){
  __wsserver.clear_access_channels(websocketpp::log::alevel::all);
  __wsserver.set_validate_handler(&on_connect);
  __wsserver.set_message_handler(&on_message);
  __wsserver.init_asio();
  __wsserver.listen(8081);
  __wsserver.start_accept();
  cout << "Starting WS_Server..." << endl;
  __wsserver.run();
}

void Application::quit(){
 // delete __config;
}

