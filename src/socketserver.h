#pragma once

#include "socketconn.h"
#include "wrapper.h"
#include "fcntl.h"

class SocketServer
{
private:
    std::vector<SocketConnection *> clients;

    int listen_fd;
    struct addrinfo hints, *serverInfo;
    int opt = 1;
    bool accepting = true;
    
    std::string password = "solarwinds123";

    void fillInHints();
    Map parseFunction(rapidxml::xml_document<> &doc, rapidxml::xml_node<> *context_node, std::string function);
    bool checkPassword(std::string);

    //Structs
    

public: 
    SocketServer(const char *port);
    void ListenAndAccept();

    Map parseXML(const char *xml);

};
