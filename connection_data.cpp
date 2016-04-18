#include "connection_data.h"

ConnectionData::ConnectionData(){
  __lobby_id = 0;
}

ConnectionData::ConnectionData(unsigned _lobby_id){
  __lobby_id = _lobby_id;
}

ConnectionData::~ConnectionData(){
}
