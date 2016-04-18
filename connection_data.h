#ifndef CONNECTION_H
#define CONNECTION_H

struct ConnectionData {
  unsigned __lobby_id;
  unsigned __lobby_position;
  ConnectionData();
  ConnectionData(unsigned);
  ~ConnectionData();
};

#endif
