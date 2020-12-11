#pragma once

#include <string>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <vector>
#include <thread>

class SocketConnection{
private:
    // Socket information:
    int f_desc;
    std::string name;
    std::string type;
    struct addrinfo a_data;

    int SendData(const char* d);
    int ReceiveData(std::string::pointer str_p);

public:
    SocketConnection(int fd, std::string name, std::string type, struct addrinfo data);
    void monitorSocket();
    inline const std::string getName() const { return this->name; }
    inline const std::string getType() const { return this->type; }

};