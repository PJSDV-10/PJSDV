#include "socketconn.h"

SocketConnection::SocketConnection(int fd, std::string name, std::string type, struct addrinfo data){
    this->f_desc = fd;
    this->name = name;
    this->type = type;
    this->a_data = data;

}