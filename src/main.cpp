#define RAPIDXML_NO_EXCEPTIONS

#include "socketserver.h"

void rapidxml::parse_error_handler(const char *what, void *where);

int main()
{
    /* There is quite enough comments in the websocket.cpp file. The main just creates a SocketServer class on port 8080, accepts the first incoming request and prints the message the remote sends. Then it sends the msg and closes the connection. */
    SocketServer sv("8080");
    sv.ListenAndAccept();
}

void rapidxml::parse_error_handler(const char *what, void *where){
    std::cout << "Parsing error: " << what << std::endl;
}