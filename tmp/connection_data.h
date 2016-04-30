#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
using namespace std;

struct ConnectionData {
  unsigned __lobby_id;
  unsigned __lobby_position;
  unsigned __id;
  ConnectionData();
  ConnectionData(unsigned);
  ~ConnectionData();
};

#endif
