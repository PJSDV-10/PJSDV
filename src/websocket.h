#pragma once
#include <string>
#include <iostream>
#include <cstring>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <sys/types.h>
#include <netinet/in.h> 
#include <netdb.h>
#include <thread>

class SocketServer{
private:
    //Structs
    /*struct Connections{
        struct sockaddr *their_addr;
        socklen_t *addr_size;
        int remote_fd;
        int status;
    };

    struct Connections conns[20];*/

    struct sockaddr their_addr;
    socklen_t addr_size;
    int remote_fd;

    struct addrinfo hints, *serverInfo;
    int opt = 1;
    int clients;
    int server_fd;
    int status;
    
    void listenFunc(int &server_fd);
    void fillInHints();

public:
    SocketServer(int port);
    void ListenAndAccept();
    void printMessage();
    void sendMessage(const char* msg);
};



