#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <jsoncpp/json/json.h>
#include "connection_data.h"

using namespace std;
using websocketpp::connection_hdl;

typedef websocketpp::server<websocketpp::config::asio> WS_Server;
typedef std::map<connection_hdl, ConnectionData> ConnectionList;
typedef std::map<connection_hdl, ConnectionData>::iterator ConnectionListIterator;
typedef map<unsigned, vector< connection_hdl > > LobbyList;
typedef map<unsigned, vector< connection_hdl > >::iterator LobbyListIterator;

class Application;

#endif
