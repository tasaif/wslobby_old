#ifndef APPLICATION_H
#define APPLICATION_H

#include "global.h"
//#include "config.h"

class Application {
  private:
    //Config* __config;
    WS_Server* __wsserver;
    static void on_message( \
        websocketpp::connection_hdl,
        WS_Server::message_ptr
    );

  public:
    Application();
    ~Application();
    bool init(string);
    void run();
    void quit();
};

#endif
