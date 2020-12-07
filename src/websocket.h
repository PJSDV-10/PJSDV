#pragma once
#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include <map>
#include <vector>

class SocketServer
{
private:
    int remote_fd; //Legacy, don't use

    struct addrinfo hints, *serverInfo;
    int opt = 1;
    int clients;
    int server_fd;
    int status = 1;
    int accepting = 1;
    std::vector<std::thread *> threads;

    void listenFunc(int &server_fd);
    void fillInHints();
    const char *sendHTTPInfo();
    std::map<std::string, std::string> parseHTTPHeader(const char *h);

public:
    SocketServer(int port);
    void ListenAndAccept();
    void printMessage();
    void sendMessage(char msg[]);
    void closeAll();
    void monitorSocket(int fd, struct sockaddr_in remote_addr, socklen_t addr_size);
};
