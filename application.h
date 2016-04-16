#ifndef APPLICATION_H
#define APPLICATION_H

#include "global.h"
//#include "config.h"

class Application {
  private:
    //Config* __config;
    static WS_Server __wsserver;
    static void on_message( \
        websocketpp::connection_hdl,
        WS_Server::message_ptr
    );
    static bool on_connect(websocketpp::connection_hdl);
    static void send_con(websocketpp::connection_hdl, string);
    static void close_con(websocketpp::connection_hdl, string);

  public:
    Application();
    ~Application();
    bool init(string);
    void run();
    void quit();
};

#endif
