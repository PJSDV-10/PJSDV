#include "websocket.h"

int main(){
    SocketServer sv(8080);
    sv.ListenAndAccept();
    sv.printMessage();

    const char* msg = "Hello, this is a message from the server!";
    sv.sendMessage(msg);

}