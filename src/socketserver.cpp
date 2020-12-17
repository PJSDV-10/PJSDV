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
    Map xml = parseXML(buffer);
    try
    {
        if(std::get<std::string>(xml.at("function").value()) == "authentication"){ 
            std::cout << "The following device authenticated with the server:\n"
                      << std::get<std::string>(std::get<Map>(xml.at("context").value()).at("clientName").value()) << std::endl;
            std::string respondmsg;
            respondmsg = respond("authentication", std::get<std::string>(xml.at("sender").value()));
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

Map SocketServer::parseXML(const char *x)
{
    using namespace rapidxml;

    Map parsedMessage;
    xml_document<> doc;
    xml_node<> *root_node;

    char xml[4096] = {};
    strcpy(xml, x);
    doc.parse<0>(xml);

    /* The root node is called "message". Everything falls under here. */
    root_node = doc.first_node("message");

    /* Parsing the sender and receiver out of the header */
    xml_node<> *header_message = root_node->first_node("header");
    std::string sender, receiver;
    sender = header_message->first_node("sender")->value();
    receiver = header_message->first_node("receiver")->value();
    parsedMessage.emplace("sender", sender);
    parsedMessage.emplace("receiver", receiver);

    xml_node<> *function_node = root_node->first_node("function");
    std::string function;
    function = function_node->value();
    parsedMessage.emplace("function", function);

    xml_node<> *context_node = root_node->first_node("context");
    Map parsedContext = parseFunction(doc, context_node, function);
    parsedMessage.emplace("context", parsedContext);

    return parsedMessage;
}

Map SocketServer::parseFunction(rapidxml::xml_document<> &doc, rapidxml::xml_node<> *context_node, std::string function)
{
    using namespace rapidxml;
    Map parsedContext;
    if (function == "authentication")
    {
        if (!checkPassword(context_node->first_node("password")->value()))
        {
            return {};
        }
        std::string clientName;
        clientName = context_node->first_node("clientName")->value();
        parsedContext.emplace("clientName", clientName);
        Array capabilities;
        for (xml_node<> *func_node = context_node->first_node("capabilities")->first_node("func"); func_node; func_node = func_node->next_sibling())
        {
            std::string type, funcName;
            type = func_node->first_node("type")->value();
            funcName = func_node->first_node("funcName")->value();
            Map function;
            function.emplace("type", type);
            function.emplace("name", funcName);
            capabilities.push_back(Wrapper(function));
        }
        parsedContext.emplace("capabilities", capabilities);
        return parsedContext;
    }
    return {};
}

std::string SocketServer::respond(std::string type, std::string dest){
    using namespace rapidxml;

    if(type == "authentication"){
        xml_document<> *doc = new xml_document<>;
        xml_node<> *root_node = doc->allocate_node(node_element, "message");
        xml_node<> *header_node = buildHeader(dest, *doc);
        xml_node<> *function_node = doc->allocate_node(node_element, "function", "ack");

        xml_node<> *context_node = doc->allocate_node(node_element, "context", 0);
        root_node->append_node(header_node);
        root_node->append_node(function_node);
        root_node->append_node(context_node);
        doc->append_node(root_node);
        std::stringstream ss;
        ss << *doc;
        return ss.str();
    }
    return {};
}

rapidxml::xml_node<>* SocketServer::buildHeader(std::string dest, rapidxml::xml_document<> &doc){
    using namespace rapidxml;
    xml_node<> *header_node = doc.allocate_node(node_element, "header", 0);
    xml_node<> *sender_node = doc.allocate_node(node_element, "sender", "server");
    char *destination = doc.allocate_string(dest.c_str());
    xml_node<> *receiver_node = doc.allocate_node(node_element, "receiver", destination);
    header_node->append_node(sender_node);
    header_node->append_node(receiver_node);
    return header_node;
}

bool SocketServer::checkPassword(std::string p)
{
    if (p == password)
    {
        return true;
    }
    return false;
}

template <typename T>
T getValue(Wrapper &w){
    return std::get<T>(w.value());
}