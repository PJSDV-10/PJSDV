#include "websocket.h"

SocketServer::SocketServer(int port)
{
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
    if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    fillInHints();
    std::string ports = std::to_string(port);
    const char *portch = ports.c_str();

    if ((status = getaddrinfo(NULL, portch, &hints, &serverInfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    int yes = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes))
    {
        perror("Failed setting \"Reuse Address\" option for socket");
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, serverInfo->ai_addr, serverInfo->ai_addrlen))
    {
        perror("Binding to port failed");
        exit(EXIT_FAILURE);
    }


    freeaddrinfo(serverInfo);
}


void SocketServer::ListenAndAccept()
{
    if (listen(server_fd, 20))
    {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }
    addr_size = sizeof(their_addr);
    remote_fd = accept(server_fd, &their_addr, &addr_size);

    if (remote_fd == -1)
    {
        perror("Accepting failed");
        exit(EXIT_FAILURE);
    }
}

void SocketServer::printMessage()
{
    char buffer[1024] = {0};
    recv(remote_fd, buffer, 1024, 0);
    std::cout << "Data read:\n"
              << buffer << std::endl;
}

void SocketServer::sendMessage(const char *msg)
{
    int len = strlen(msg);
    int flags = 0;
    send(remote_fd, &msg, len, flags);
}

void SocketServer::closeAll()
{
    if (close(remote_fd))
    {
        perror("Closing remote socket failed");
        exit(EXIT_FAILURE);
    }
    if (close(server_fd))
    {
        perror("Closing listening socket failed");
        exit(EXIT_FAILURE);
    }
}

void SocketServer::fillInHints()
{
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
}