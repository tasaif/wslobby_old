#include "application.h"
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
using namespace std;

WS_Server Application::__wsserver;
ConnectionList Application::__connection_list;
LobbyList Application::__lobby_list;

bool Application::on_connect(websocketpp::connection_hdl hdl){
  cout << "New Connection" << endl;
  return true;
}

void Application::on_close(websocketpp::connection_hdl hdl){
  cout << "Closing Connection" << endl;
  ConnectionListIterator cit;
  LobbyListIterator lit;
  Json::Reader reader;
  Json::Value verification_msg;
  Json::Value who_left;
  cit = __connection_list.find(hdl);
  // If the connection had a valid lobby id remove it from
  // the lobby and the connection list
  if (cit != __connection_list.end()){
    unsigned lobby_id = cit->second.__lobby_id;
    unsigned lobby_position = cit->second.__lobby_position;
    unsigned id = cit->second.__id;
    lit = __lobby_list.find(lobby_id);
    lit->second.erase(lit->second.begin() + lobby_position);
    __connection_list.erase(cit);
    // If that was the last connection in the lobby
    // remove the lobby as well and invalidate it
    // otherwise tell everyone who left
    if (lit->second.size() == 0){
      __lobby_list.erase(lit);
      cout << "Removing Lobby: " << lobby_id << endl;
      ostringstream url;
      ostringstream stringified_verification;
      url << "http://localhost:3000/games/remove_lobby/" << lobby_id;
      curlpp::Easy request;
      request.setOpt(new curlpp::options::Url(url.str()));
      stringified_verification << request;
      bool parsingSuccessful = reader.parse(stringified_verification.str(), verification_msg);
      if (parsingSuccessful){
        if (verification_msg["error"].asBool()){
          cout << "Error: lobby db responded with error" << endl;
        }
        cout << verification_msg["text"] << endl;
      } else {
        cout << "Error: expected json response from lobby db" << endl;
        cout << url << endl;
        cout << stringified_verification.str() << endl;
      }
    } else {
      who_left["from"] = Json::Value("server");
      who_left["type"] = Json::Value("client_disconnected");
      who_left["id"] = Json::Value(id);
      Json::StreamWriterBuilder wbuilder;
      string stringified_who_left = Json::writeString(wbuilder, who_left);
      broadcast(lit, stringified_who_left);
    }
  }
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
    if (msg["request"] == "send"){
      unsigned lobby_id = msg["lobby"].asInt();
      LobbyListIterator it = __lobby_list.find(lobby_id);
      cout << lobby_id << endl;
      cout << stringified_msg << endl;
      for(unsigned i=0; i<it->second.size(); i++){
        //send_con(it->second[i], stringified_msg);
        cout << it->second[i] << endl;
      }
      //broadcast(it, stringified_msg, hdl);
    } else if (msg["request"] == "connect"){
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
          cout << "Error: lobby db responded with error" << endl;
          cout << verification_msg["text"] << endl;
          close_con(hdl, stringified_verification.str());
        } else {
          cout << "Successful connection" << endl;
          ConnectionData connection_data = ConnectionData(lobby_id);
          LobbyListIterator it = __lobby_list.find(lobby_id);
          if (it == __lobby_list.end()){
            cout << "Creating Lobby" << endl;
            __lobby_list[lobby_id] = vector<connection_hdl>();
            it = __lobby_list.find(lobby_id);
          } else {
            cout << "Lobby existed: " << it->second.size() << " other connections" << endl;
          }
          connection_data.__lobby_position = it->second.size();
          if (it->second.size() == 0){
            connection_data.__id = 1;
          } else {
            connection_data.__id = __connection_list[it->second.back()].__id + 1;
          }
          __connection_list[hdl] = connection_data;
          it->second.push_back(hdl);
          verification_msg["from"] = Json::Value("server");
          verification_msg["type"] = Json::Value("assign_id");
          verification_msg["id"] = Json::Value(connection_data.__id);
          Json::StreamWriterBuilder wbuilder;
          std::string confirmation = Json::writeString(wbuilder, verification_msg);
          send_con(hdl, confirmation);
          verification_msg["type"] = Json::Value("new_client");
          string new_connection_message = Json::writeString(wbuilder, verification_msg);
          broadcast(it, new_connection_message);
        }
      } else {
        cout << "Error: lobby db responded with unparsible response" << endl;
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
  __wsserver.set_close_handler(&on_close);
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

void Application::broadcast(LobbyListIterator lit, string msg, websocketpp::connection_hdl from){
  ConnectionListIterator cit;
  for(unsigned i=0; i<lit->second.size(); i++){
    //cit = __connection_list.find(lit->second[i]);
    //if (cit != __connection_list.end()){
    //  cout << cit->second.__id << endl;
    //}
    //if (lit->second[i] != from){
    //  send_con(lit->second[i], msg);
    //}
  }
}

void Application::broadcast(LobbyListIterator lit, string msg){
  for(unsigned i=0; i<lit->second.size(); i++){
    send_con(lit->second[i], msg);
  }
}

void Application::close_con(websocketpp::connection_hdl hdl, string msg){
  cout << "Terminating Connection" << endl;
  websocketpp::lib::error_code ec;
  __wsserver.close(hdl, websocketpp::close::status::normal, msg, ec);
  if (ec) {
    cout << ec.message() << endl;
  }
}
