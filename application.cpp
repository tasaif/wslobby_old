#include "application.h"
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
using namespace std;

WS_Server Application::__wsserver;

bool Application::on_connect(websocketpp::connection_hdl hdl){
  cout << "New Connection" << endl;
  return true;
}

void Application::on_message(websocketpp::connection_hdl hdl, WS_Server::message_ptr ws_msg) {
  string stringified_msg = ws_msg->get_payload();
  Json::Value msg;   // will contains the root value after parsing.
  Json::Value verification_msg;
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
      ostringstream url;
      ostringstream stringified_verification;
      url << "http://localhost:3000/games/validate_lobby/" << lobby_id;
      curlpp::Easy request;
      request.setOpt(new curlpp::options::Url(url.str()));
      stringified_verification << request;
      parsingSuccessful = reader.parse(stringified_verification.str(), verification_msg);
      if (parsingSuccessful){
        if (verification_msg["error"].asBool()){
          cout << "Error: wslobby responded with error" << endl;
          cout << verification_msg["text"] << endl;
          close_con(hdl, stringified_verification.str());
        } else {
          cout << "Successful connection" << endl;
          send_con(hdl, stringified_verification.str());
        }
      } else {
        cout << "Error: wslobby responded with unparsible response" << endl;
        cout << stringified_verification << endl;
      }
    } else {
      cout << "Error: Unrecognized request." << endl;
      close_con(hdl, "Unrecognized request");
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

void Application::send_con(websocketpp::connection_hdl hdl, string msg){
  __wsserver.get_con_from_hdl(hdl)->send(msg);
}

void Application::close_con(websocketpp::connection_hdl hdl, string msg){
  cout << "Terminating Connection" << endl;
  websocketpp::lib::error_code ec;
  __wsserver.close(hdl, websocketpp::close::status::normal, msg, ec);
  if (ec) {
    cout << ec.message() << endl;
  }
}
