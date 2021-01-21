#pragma once

#define DEBUG 1

#include <string>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <vector>
#include <iostream>
#include <map>
#include <unistd.h>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "device.h"
#include "fcntl.h"

#include <sstream>

class SocketServer
{
private:
    fd_set ready_sockets, all_sockets, error_checking_sockets;
    int listen_fd;
    //struct addrinfo hints, *serverInfo;
    //int opt = 1;
    //bool accepting = true;
    std::vector<Wemos*> wemosjes;

    std::string password = "solarwinds123";

    int authWemos(int fd, XmlReader& msg);
    bool checkPassword(std::string);
    int accept_connection(int fd);
    bool checkIfWemosExists(String name);
    void removeWemosByFD(int fd);
    void closeConnection(int fd);
    void handleRequest(int fd);


public: 
    SocketServer(const char *port);
    void ListenAndAccept();

};

