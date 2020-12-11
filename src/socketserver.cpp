#include "socketserver.h"

SocketServer::SocketServer(const char* port){
    /*server_df is the File Descriptor for the socket. Calling socket() simply creates a file descriptor for the websocket. By itself it doesn't do anything.
    The arguments passed are:
    PF_INET:
        This specifies that the socket should operate on the Profile Family for Internet addresses.
    SOCK_STREAM:
        Making the websocket a stream, instead of a diagram
    0:
        This autoselects the right protocol (tcp or udp) based on the socket type STREAM or DGRAM

    This function returns 0 on a failure and a -1 on a success, making it suitable for an if statement.
    */
    if ((listen_fd = socket(PF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    fillInHints();

    //We retrieve the address information for the socket
    int status;
    if ((status = getaddrinfo(NULL, port, &hints, &serverInfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    /*This is a little setting we need to change. If we don't set SO_REUSEADDR to true then the program during bind() might get blocked because the port might already be bound. This can happen if we run this program a couple times in succession.*/
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT | SO_KEEPALIVE, &opt, sizeof opt))
    {
        perror("Failed setting \"Reuse Address\" option for socket");
        exit(EXIT_FAILURE);
    }

    /* Bind the socket server_fd to the port we specified earlier with getaddrinfo(). Finally the socket is bound and can be used. */
    if (bind(listen_fd, serverInfo->ai_addr, serverInfo->ai_addrlen))
    {
        perror("Binding to port failed");
        exit(EXIT_FAILURE);
    }

    /* The serverInfo is not needed anymore, as the socket is already bound at this point. This essentially just frees all the struct. It's a specialized function because serverInfo is a addrinfo struct, which is a linked list. It's made specially for this struct and is a native function. */
    freeaddrinfo(serverInfo);
}

/* fillInHints() sets the static settings for the later to be used getAddrInfo() function.
Here we populate a addrinfo struct with zeros and then set the following settings:
    ai_family = AF_UNSPEC (IPv4 and IPv6 are fine)
    ai_socktype = SOCK_STREAM (TCP stream socket)
    ai_flags = AI_PASSIVE (automatically choose my IP)*/
void SocketServer::fillInHints()
{
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
}

void SocketServer::ListenAndAccept(){
    if (listen(listen_fd, 20))
    {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in their_addr;
    socklen_t addr_size;
    int r_fd;
    addr_size = sizeof(their_addr);
    while (accepting)
    {
        if ((r_fd = accept(listen_fd, (struct sockaddr *)&their_addr, &addr_size)))
        {
=            std::thread thing()

        }
        if (r_fd == -1)
        {
            perror("Accepting failed");
            exit(EXIT_FAILURE);
        }
    }
}

std::thread* SocketServer::launchConnection(int fd){
    SocketConnection conn(fd, )
}