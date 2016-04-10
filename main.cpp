#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
using namespace std;

typedef websocketpp::server<websocketpp::config::asio> Server;

void on_message(websocketpp::connection_hdl hdl, Server::message_ptr msg) {
  std::cout << msg->get_payload() << std::endl;
}

int main() {
  Server wsserver;
  wsserver.clear_access_channels(websocketpp::log::alevel::all);
  wsserver.set_message_handler(&on_message);

  cout << "Initializing ASIO: ";
  wsserver.init_asio();
  cout << "ok" << endl;

  cout << "Listen on port 9002: ";
  wsserver.listen(9002);
  cout << "ok" << endl;

  cout << "Accept new connections: ";
  wsserver.start_accept();
  cout << "ok" << endl;

  cout << "Running server..." << endl;
  wsserver.run();
}
