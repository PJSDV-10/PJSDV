#include "websocket.h"

int main(){
    /* There is quite enough comments in the websocket.cpp file. The main just creates a SocketServer class on port 8080, accepts the first incoming request and prints the message the remote sends. Then it sends the msg and closes the connection. */
    SocketServer sv(8080);
    sv.ListenAndAccept();
    sv.printMessage();

    const char* msg = "Hello, this is a message from the server!";
    sv.sendMessage(msg);
    sv.closeAll();

}