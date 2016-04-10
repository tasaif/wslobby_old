#include <iostream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
using namespace std;

typedef websocketpp::server<websocketpp::config::asio> server;

void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
  std::cout << msg->get_payload() << std::endl;
}

int main() {
  server print_server;

  print_server.clear_access_channels(websocketpp::log::alevel::all);
  print_server.set_message_handler(&on_message);

  cout << "Initializing ASIO: ";
  print_server.init_asio();
  cout << "ok" << endl;
  cout << "Listen on port 9002: ";
  print_server.listen(9002);
  cout << "ok" << endl;
  cout << "Accept new connections: ";
  print_server.start_accept();
  cout << "ok" << endl;
  cout << "Running server..." << endl;
  print_server.run();
  cout << "Program end" << endl;
}
