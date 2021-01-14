#include "socketserver.h"



SocketServer::SocketServer(const char *port)
{
    /*server_fd is the File Descriptor for the socket. Calling socket() simply creates a file descriptor for the websocket. By itself it doesn't do anything.
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

    int flags = fcntl(listen_fd, F_GETFL);
    if (fcntl(listen_fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("Failed setting \"Non-block\" option for socket");
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

void SocketServer::ListenAndAccept()
{
    if (listen(listen_fd, 20))
    {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    fd_set ready_sockets, all_sockets;
    FD_ZERO(&all_sockets);
    FD_SET(listen_fd, &all_sockets);

    while (accepting)
    {
        ready_sockets = all_sockets;

        if(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0){
            perror("Select failed");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; i++){
            if(FD_ISSET(i, &ready_sockets)){
                if(i == listen_fd){
                    std::cout << "A new incoming connection" << std::endl;
                    int new_fd;
                    //This is a new incoming connection
                    new_fd = accept_connection(listen_fd);
                    FD_SET(new_fd, &all_sockets);
                }else{
                    handleRequest(i);
                    FD_CLR(i, &all_sockets);
                }
            }
        }
    }
}

void SocketServer::handleRequest(int fd){
    char buffer[4096] = {0};
    int bytesread;
    if ((bytesread = recv(fd, buffer, 4096, 0)) == -1)
    {
        perror("Error receiving");
        exit(EXIT_FAILURE);
    }else if(bytesread == 0){
        close(fd);
        return;
    }

    std::cout << "The following message was received:\n\r" << buffer << std::endl;
    XmlReader xml_r(buffer);
	std::cout << "reader made" << std::endl;
    xml_r.parseDocument();
	std::cout << "doc parsed" << std::endl;
    //Map xml = xml_r.getParsedDoc();
    //std::cout << "map made" << std::endl;
	/*if (xml.empty())
    {
        //Debug message
        std::cout << "The parsed XML was empty, check for errors.\n\rThis debug message exists at line " << __LINE__ << std::endl;
    }*/
    try
    {
        if(xml_r.getFunction() == "authentication"){ 
            std::cout << "The following device authenticated with the server:\n"
                      << xml_r.getClientName() << std::endl;
            std::string respondmsg;
            XmlWriter xml_w(xml_r);
            xml_w.buildXML();
            respondmsg = xml_w.getXML();
            send(fd, respondmsg.c_str(), strlen(respondmsg.c_str()), 0);
            std::cout << "Send message back" << std::endl;
        }else if(xml_r.getFunction() == "sensorUpdate"){
            std::cout << "Sensor update received from:\n"
                      << xml_r.getClientName() << std::endl;
            std::string respondmsg;
            XmlWriter xml_w(xml_r);
            xml_w.buildXML();
            respondmsg = xml_w.getXML();
            send(fd, respondmsg.c_str(), strlen(respondmsg.c_str()), 0);
        }
    }catch(const std::out_of_range& oor){
        std::cout << "function did not exist inside the thingy" << std::endl;
        send(fd, "no", 2, 0);
        return;
    }
    return;
}

int SocketServer::accept_connection(int fd){
    struct sockaddr_in their_addr;
    socklen_t addr_size;
    int r_fd;
    addr_size = sizeof(their_addr);

    r_fd = accept(listen_fd, (struct sockaddr *)&their_addr, &addr_size);
    if(r_fd == -1){
        perror("Accepting failed");
        exit(EXIT_FAILURE);
    }
    return r_fd;
}

bool SocketServer::checkIfWemosExists(std::vector<Wemos> &a, String name){
    for (uint i = 0; i < a.size(); i++){
        String senderName;
        //Debug message
        std::cout << "senderName comparison check\nThis debug message exists at line " << __LINE__ << std::endl;
        senderName = a[i].getSenderName();
        if (senderName == name)
        {
            return true;
        }
    }
    return false;
}
