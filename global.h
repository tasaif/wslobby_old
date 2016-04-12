#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

using namespace std;

typedef websocketpp::server<websocketpp::config::asio> WS_Server;

class Application;

#endif
