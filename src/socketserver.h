#pragma once

#include "socketconn.h"
#include "device.h"
#include "fcntl.h"

#include <sstream>

class SocketServer
{
private:
    std::vector<SocketConnection *> clients;

    int listen_fd;
    struct addrinfo hints, *serverInfo;
    int opt = 1;
    bool accepting = true;
    std::vector<Wemos*> wemosjes;

    std::string password = "solarwinds123";

    int authWemos(XmlReader& msg);
    void fillInHints();
    bool checkPassword(std::string);
    int accept_connection(int fd);
    rapidxml::xml_node<>* buildHeader(std::string dest,rapidxml::xml_document<> &doc);
    bool checkIfWemosExists(String name);
    //Structs

public: 
    SocketServer(const char *port);
    void ListenAndAccept();
    void handleRequest(int fd);
    std::string respond(std::string type, std::string dest);
};