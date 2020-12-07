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

    //Convert the int port into a const char* (which has to be more difficult than it should be of course...)
    std::string ports = std::to_string(port);
    const char *portch = ports.c_str();

    //We retrieve the address information for the socket
    if ((status = getaddrinfo(NULL, portch, &hints, &serverInfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    /*This is a little setting we need to change. If we don't set SO_REUSEADDR to true then the program during bind() might get blocked because the port might already be bound. This can happen if we run this program a couple times in succession.*/
    int yes = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT | SO_KEEPALIVE, &yes, sizeof yes))
    {
        perror("Failed setting \"Reuse Address\" option for socket");
        exit(EXIT_FAILURE);
    }

    /* Bind the socket server_fd to the port we specified earlier with getaddrinfo(). Finally the socket is bound and can be used. */
    if (bind(server_fd, serverInfo->ai_addr, serverInfo->ai_addrlen))
    {
        perror("Binding to port failed");
        exit(EXIT_FAILURE);
    }

    /* The serverInfo is not needed anymore, as the socket is already bound at this point. This essentially just frees all the struct. It's a specialized function because serverInfo is a addrinfo struct, which is a linked list. It's made specially for this struct and is a native function. */
    freeaddrinfo(serverInfo);
}

/* Start listening on the server_fd socket and accept any incoming connections. */
void SocketServer::ListenAndAccept()
{
    /* Start listening on server_fd. This is essentially a setting and therefor non-blocking. The 20 in there is the limit of how many concurrent requests there can be. Unfortunately at this point there is no forking, nor any multithreading yet. */
    if (listen(server_fd, 20))
    {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    /* Set the addr_size to the size of the their_addr sockaddr struct. This is a little off from how it's supposed to be though. The their_addr struct is not supposed to be a sockaddr but a sockaddr_storage, but I couldn't get this to work. 
    The accept() function is blocking here and waits for an incoming signal. It then creates a new socket called remote_fd. This is a new socket that can be used. The old server_fd is still listening. Normally this would be put into an accept loop and the processes would then be handled by forks or other threads. */
    struct sockaddr_in their_addr;
    socklen_t addr_size;
    int r_fd;
    addr_size = sizeof(their_addr);
    while (accepting)
    {
        if ((r_fd = accept(server_fd, (struct sockaddr *)&their_addr, &addr_size)))
        {
            //std::thread monitor(&SocketServer::monitorSocket, this, r_fd, their_addr, addr_size);
            //threads.emplace_back(&monitor);
            monitorSocket(r_fd, their_addr, addr_size);
        }
    }

    if (r_fd == -1)
    {
        perror("Accepting failed");
        exit(EXIT_FAILURE);
    }
}

/* Prints any incoming message to the terminal */
void SocketServer::printMessage()
{
    char buffer[1024] = {0};
    recv(remote_fd, buffer, 1024, 0);
    std::cout << "Data read:\n"
              << buffer << std::endl;
    //printf("%s\n",buffer );
}

/* Sends a message to the remote host. */
void SocketServer::sendMessage(char msg[])
{
    int len = strlen(msg);
    int flags = 0;
    send(remote_fd, msg, len, flags);
}

/* Closes the sockets. This will be made cleaner later once I figure out how it works exactly. */
void SocketServer::closeAll()
{
    if (close(server_fd))
    {
        perror("Closing listening socket failed");
        exit(EXIT_FAILURE);
    }
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

void SocketServer::monitorSocket(int fd, struct sockaddr_in remote_addr, socklen_t addr_size)
{
    const char *acceptMsg = "Source:Server,Action:Accept\n\r";
    if (send(fd, acceptMsg, strlen(acceptMsg), 0) == -1)
    {
        perror("Failed sending accept message");
        exit(EXIT_FAILURE);
    }
    char msgBuf[1024] = {0};
    while (true)
    {
        memset(msgBuf, 0, 1024);
        recv(fd, msgBuf, 1024, 0);
        std::cout << "Received Message: \n"
                  << msgBuf << std::endl;
        std::map<std::string, std::string> parsed = parseMessage(msgBuf);
        try
        {
            std::string func = parsed.at("Function");
            if (func == "Close")
            {
                std::cout << "Closing connection with remote" << std::endl;
                close(fd);
                return;
            }
            else if (func == "KA")
            {
                std::cout << "Keeping alive." << std::endl;
                send(fd, "OK", 2, 0);
                continue;
            }
        }
        catch (const std::out_of_range &oor)
        {
            std::cout << "oh no" << std::endl;
            send(fd, "Error", 5, 0);
            continue;
        }
    }
    return;
}

std::map<std::string, std::string> SocketServer::parseMessage(const char *h)
{
    std::map<std::string, std::string> parsed;
    std::string::iterator ptr;
    std::string msg = h;
    std::string delim = ",";
    std::string idelim = ":";
    std::string ending = "\r\n";

    size_t start = 0;
    size_t pos = 0;
    size_t pos2 = 0;
    while (true)
    {
        if (msg == "")
        {
            return {};
        }
        else if ((pos = msg.find(delim)) != std::string::npos)
        {
            if ((pos2 = msg.find(idelim)) != std::string::npos)
            {
                std::string tmpkey, tmpval;
                /* 
             Start  pos2  pos
                |     |    |
                Source:Host,Function:Accept\n\r
                 */
                tmpkey = msg.substr(start, pos2);
                tmpval = msg.substr(pos2 + 1, (pos - (pos2 + 1)));
                parsed.emplace(tmpkey, tmpval);
                start = pos + 1;
                continue;
            }
            else
            {
                return {};
            }
        }
        else if ((pos = msg.find(ending)) != std::string::npos)
        {
            if ((pos2 = msg.find(idelim)) != std::string::npos)
            {
                std::string tmpkey, tmpval;
                tmpkey = msg.substr(start, pos2);
                tmpval = msg.substr(pos2 + 1, (pos - (pos2 + 1)));
                parsed.emplace(tmpkey, tmpval);
                break;
            }
            else
            {
                return {};
            }
        }
        else
        {
            return {};
        }
    }
    return parsed;
}