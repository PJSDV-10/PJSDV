#pragma once

#include "socketconn.h"

class SocketServer
{
private:
    

    std::vector<SocketConnection*> clients;

    int listen_fd;
    struct addrinfo hints, *serverInfo;
    int opt = 1;
    bool accepting = true;
    
    std::thread* launchConnection(int fd);
    void fillInHints();

public:

    SocketServer(const char* port);
    void ListenAndAccept();

};
