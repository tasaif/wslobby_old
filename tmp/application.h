#ifndef APPLICATION_H
#define APPLICATION_H

#include "global.h"
#include "connection_data.h"
//#include "config.h"

class Application {
  private:
    //Config* __config;
    static WS_Server __wsserver;
    static map<websocketpp::connection_hdl, unsigned> __metadata;
    static bool on_connect(websocketpp::connection_hdl);
    static void on_message(websocketpp::connection_hdl, WS_Server::message_ptr);
    static void on_close(websocketpp::connection_hdl);
    static void send_con(websocketpp::connection_hdl, string);
    static void broadcast(LobbyListIterator, string);
    static void close_con(websocketpp::connection_hdl, string);
    static ConnectionList __connection_list;
    static LobbyList __lobby_list;

  public:
    Application();
    ~Application();
    bool init(string);
    void run();
    void quit();
};

#endif
