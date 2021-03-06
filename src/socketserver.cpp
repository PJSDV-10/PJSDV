#include "socketserver.h"



SocketServer::SocketServer(const char *port)
{



    /*server_fd is the File Descriptor for the socket. Calling socket() simply creates a file descriptor for the websocket.
     * By itself it doesn't do anything.
    The arguments passed are:
    PF_INET:
        This specifies that the socket should operate on the Profile Family for Internet addresses.
    SOCK_STREAM:
        Making the websocket a stream, instead of a diagram
    0:
        This autoselects the right protocol (tcp or udp) based on the socket type STREAM or DGRAM

    This function returns 0 on a failure and a -1 on a success, making it suitable for an if statement.
    */
    struct addrinfo hints, *serverInfo;
    int opt = 1;

    if ((listen_fd = socket(PF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    /*
    Here we populate a addrinfo struct with zeros and then set the following settings:
    ai_family = AF_UNSPEC (IPv4 and IPv6 are fine)
    ai_socktype = SOCK_STREAM (TCP stream socket)
    ai_flags = AI_PASSIVE (automatically choose my IP)
     */
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

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

    /* The serverInfo is not needed anymore, as the socket is already bound at this point. This essentially just frees
     * all the struct. It's a specialized function because serverInfo is a addrinfo struct, which is a linked list.
     * It's made specially for this struct and is a native function. */
    freeaddrinfo(serverInfo);
}

void SocketServer::ListenAndAccept()
{
    if (listen(listen_fd, 20))
    {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }
    /*
        all_sockets here is the set of all sockets. This includes the listening socket
        as well as all the open sockets to wemos devices.
        ready_sockets is filled with the select function, making it the set of all sockets
        that are ready to read.
        error_checking_sockets checks for every socket if there can be messages sent to it.
        if you cannot, that means the connection is broken. So close the socket you fool!
    */
    //fd_set ready_sockets, all_sockets, error_checking_sockets;
    FD_ZERO(&all_sockets);
    FD_ZERO(&error_checking_sockets);
    FD_SET(listen_fd, &all_sockets);
    FD_SET(listen_fd, &error_checking_sockets);
    bool accepting = true;
    while (accepting)
    {
        ready_sockets = all_sockets;
        error_checking_sockets = all_sockets;

        std::cout << "\nWaiting for client to read from" << std::endl;
        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            perror("Select failed");
            exit(EXIT_FAILURE);
        }

        std::cout << "Found one!" << std::endl;

        for (int i = 0; i <= FD_SETSIZE; i++){
            if(FD_ISSET(i, &ready_sockets)){
                if(i == listen_fd){
                    std::cout << "A new incoming connection" << std::endl;
                    int new_fd;
                    //This is a new incoming connection
                    new_fd = accept_connection(listen_fd);
                    FD_SET(new_fd, &all_sockets);
                }else{
                    handleRequest(i);
                    /* Technically most sockets here are not supposed to be closed
                    the FD_CLR function here removes the socket from all sockets that are
                    connected, so it's not monitored anymore, which is bad */
                    //FD_CLR(i, &all_sockets);
                }
            }
        }

        /*for (int i = 0; i < FD_SETSIZE; i++)
        {
            if(FD_ISSET(i, &error_checking_sockets) && i != listen_fd){
                int error;
                if (error = send(i, keepalive_msg.c_str(), strlen(keepalive_msg.c_str()), 0); error == -1)
                {
                    if(errno == ECONNRESET){
                        // A client disconnected
                        std::cout << "A client failed to receive any data.\n\rClosing socket" << std::endl;
                        FD_CLR(i, &all_sockets);
                        close(i);
                        removeWemosByFD(i);
                    }
                }
            }
        }*/
    }
}

void SocketServer::handleRequest(int fd){
    char buffer[4096] = {0};
    int bytesread;
    if ((bytesread = recv(fd, buffer, 4096, 0)) == -1)
    {
        perror("Error receiving at handleRequest");
        closeConnection(fd);
        return;
    }
    else if (bytesread == 0)
    {
        std::cout << "Closing socket from handleRequest function" << std::endl;
        closeConnection(fd);
        return;
    }

    std::cout << "The following message was received:\n\r" << buffer << std::endl;
    XmlReader xml_r(buffer);
    xml_r.parseDocument();
    if(xml_r.error() == PARSING_ERROR){
        xml_r.reseterr();
        std::cout << "Parsing error occured" << std::endl;
        return;
    }
    //Map xml = xml_r.getParsedDoc();
    if(xml_r.getFunction() == "authentication"){
        std::cout << "The following device authenticated with the server:\n"
                    << xml_r.getClientName() << "\nThis one has file descriptor: " << fd << std::endl;
        if(authWemos(fd, xml_r) == 1){ // 1 means error
            std::cout << "Wemos failure with authentication" << std::endl;

        }
        std::string respondmsg;
        XmlWriter xml_w(xml_r);
        xml_w.buildXMLAck();
        respondmsg = xml_w.getXML();
        std::cout << "Message to be sent to client:\n"
                    << respondmsg << std::endl;
        send(fd, respondmsg.c_str(), strlen(respondmsg.c_str()), 0);
        std::cout << "Reply to authentication sent" << std::endl;

        /* Close socket in case that the client is the website */
        if(xml_r.getType() == "website"){
            closeConnection(fd);
        }
        // Destroyer
        return;
    }
    else if (xml_r.getFunction() == "sensorUpdate")
    {
        /*std::cout << "Sensor update received from:\n" << xml_r.getSenderName() << std::endl;*/
        std::string respondmsg;
        for (std::size_t i = 0; i < wemosjes.size(); i++){
            //std::cout << "trying to find right wemos" << std::endl;
            if (wemosjes[i]->getSenderName() == xml_r.getSenderName())
            {
                respondmsg = wemosjes[i]->handleSensorUpdate(&xml_r);
                break;
            }
        }
        send(fd, respondmsg.c_str(), strlen(respondmsg.c_str()), 0);
        //std::cout << "Reply to sensorUpdate sent" << std::endl;

        /* Close socket in case that the client is the website */
        if(xml_r.getType() == "website"){
            closeConnection(fd);
        }
        // Destroy
        return;
    }else if(xml_r.getFunction() == "getStatusAll"){
        /*
            Handling for the getStatusAll request from the website.
            Let's hope this actually goddamn works XD
            senderName compa
            A thing you can try when it doesn't work is to add a select() call to make sure you can actually
            write to the sockets.
            */
        std::cout << "getStatusAll request received, handling it by sending broadcast" << std::endl;

        XmlWriter xml_w("getStatusBroadcast", "allWemos");
        xml_w.buildXMLStatusRequest();
        std::string toBeSent = xml_w.getXML();
        std::cout << "Sending status request to all wemos devices" << std::endl;
        XmlWriter xml_ww("answerToStatusRequest", "website");
        xml_ww.buildXMLAnswerToSR();
        fd_set wemosSet = all_sockets;
        timeval shortTime;

        for (int j = 0; j < 1; j++)
        {
            shortTime.tv_usec = 100000;
            if (select(FD_SETSIZE, NULL, &wemosSet, NULL, &shortTime) < 0)
            {
                perror("Select failed");
                exit(EXIT_FAILURE);
            }
            if(shortTime.tv_usec < 10){
                std::cout << "timeout expired" << std::endl;
            }
            for (int i = 0; i < FD_SETSIZE; i++)
            {
                //std::cout << "Checking socket number: " << i << std::endl;
                if (FD_ISSET(i, &wemosSet) && i != fd)
                {
                    std::cout << "Sending broadcast to this device" << std::endl;
                    // Getting garbage before actually doing anything
                    // recv(i, NULL, 4096, 0);
                    if (send(i, toBeSent.c_str(), strlen(toBeSent.c_str()), 0) == -1)
                    {
                        perror("Error sending status broadcast to wemos devices");
                        exit(EXIT_FAILURE);
                    }
                    char buffer2[4096] = {0};
                    int bytesread2;
                    if ((bytesread2 = recv(i, buffer2, 4096, 0)) == -1)
                    {
                        std::cout << "Error receiving answer to broadcast, closing connection" << std::endl;
                        closeConnection(i);
                        //errno = 0;
                    }
                    std::cout << "message received: " << buffer2 << std::endl;
                    XmlReader xml_rr(buffer2);
                    xml_rr.parseDocument();
                    xml_ww.addDataToAnswer(xml_rr.getType(), xml_rr.getSenderName(), xml_rr.getData());
                }
            }
        }

        std::cout << "Gotten all stuff, finalizing answer, then sending data" << std::endl;
        xml_ww.finalizeAnswerToSR();
        std::string sendBack;
        sendBack = xml_ww.getXML();
        std::cout << "Sending the following message back to the website: \n\r" << sendBack << std::endl;
        if(send(fd, sendBack.c_str(), strlen(sendBack.c_str()), 0) == -1){
            perror("Error sending");
            exit(EXIT_FAILURE);
        }/*else{
            std::cout << "The connection to the website was aborted during the building of the answer to it's status request.\n\rStuff will be fine, closing the socket was the next thing anyway." << std::endl;
            //errno = 0;
        }*/
        // Close connection to website
        closeConnection(fd);
        std::cout << "Returning" << std::endl;
        // Destroy
   } /*else if(xml_r.getFunction() == "changeStatus"){

        std::string statusmsg;
        std::cout << "update for wemos: " << xml_r.getClientName() << '\n';
        std::vector<float> tmp =  xml_r.getData();
        std::cout << "data for wemos: " << tmp[0] << tmp[1] << std::endl;

        for (std::size_t i = 0; i < wemosjes.size(); i++){
            if (wemosjes[i]->getClientName() == xml_r.getClientName())
            {
                statusmsg = wemosjes[i]->handleWebsiteUpdate(&xml_r);
                break;
            }
        }
        std::cout << statusmsg << std::endl;
        send(fd, statusmsg.c_str(), strlen(statusmsg.c_str()), 0);
        //std::cout << "Reply to sensorUpdate sent" << std::endl;

        // Close socket in case that the client is the website
        if(xml_r.getSenderName() == "website"){
            closeConnection(fd);
        }
        // Destroy
        return;
    }*/

    else if(xml_r.getFunction() == "changeStatusAan"){

        std::string statusmsg;
        int fd_tmp;

        for (std::size_t i = 0; i < wemosjes.size(); i++){
            std::cout << (wemosjes[i]->getClientName() == xml_r.getClientName()) << std::endl;
            if (wemosjes[i]->getClientName() == xml_r.getClientName())
            {
                statusmsg = wemosjes[i]->handleWebsiteUpdate(&xml_r,1);
                fd_tmp = wemosjes[i]->getFD();

                break;
            }
        }
        std::cout << statusmsg << std::endl;
        send(fd_tmp, statusmsg.c_str(), strlen(statusmsg.c_str()), 0);
        std::cout << "Reply to sensorUpdate sent" << std::endl;

        /* Close socket in case that the client is the website */
        if(xml_r.getSenderName() == "website"){
            closeConnection(fd);
        }
        // Destroy
        return;
    }
    else if(xml_r.getFunction() == "changeStatusUit"){

        std::string statusmsg;
        int fd_tmp;
        for (std::size_t i = 0; i < wemosjes.size(); i++){
            std::cout << (wemosjes[i]->getClientName() == xml_r.getClientName()) << std::endl;
            if (wemosjes[i]->getClientName() == xml_r.getClientName())
            {
                statusmsg = wemosjes[i]->handleWebsiteUpdate(&xml_r,0);
                fd_tmp = wemosjes[i]->getFD();
                break;
            }
        }
        std::cout << statusmsg << std::endl;
        send(fd_tmp, statusmsg.c_str(), strlen(statusmsg.c_str()), 0);
        std::cout << "Reply to sensorUpdate sent" << std::endl;

        /* Close socket in case that the client is the website */
        if(xml_r.getSenderName() == "website"){
            closeConnection(fd);
        }
        // Destroy
        return;
    }

}


/* Returns a 1 if an error occurred */
int SocketServer::authWemos(int fd, XmlReader& msg){
    if(checkIfWemosExists(msg.getSenderName()) == 1){
        std::cout << "Wemos existed in table" << std::endl;
        return 1;
    }
    if(msg.getType() == "chair"){
        wemosjes.emplace_back(new Chair(fd, msg.getClientName(), msg.getSenderName()));
    }else if(msg.getType() == "website"){
        wemosjes.emplace_back(new Website(fd, msg.getClientName(), msg.getSenderName()));
    }else if(msg.getType() == "column"){
        wemosjes.emplace_back(new Column(fd, msg.getClientName(), msg.getSenderName()));
    }else if(msg.getType() == "bed"){
        wemosjes.emplace_back(new Bed(fd, msg.getClientName(), msg.getSenderName()));
    }else if(msg.getType() == "tablelamp"){
        wemosjes.emplace_back(new TableLamp(fd, msg.getClientName(), msg.getSenderName()));
    }else if(msg.getType() == "door"){
        wemosjes.emplace_back(new Door(fd, msg.getClientName(), msg.getSenderName()));
    }else if(msg.getType() == "wall"){
        wemosjes.emplace_back(new Wall(fd, msg.getClientName(), msg.getSenderName()));
    }else if(msg.getType() == "fridge"){
        wemosjes.emplace_back(new Fridge(fd, msg.getClientName(), msg.getSenderName()));
    }else{
        std::cout << "Wrong or unknown type" << std::endl;
        return 1;
    }
    return 0;
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
    int opt = 1;
    setsockopt(r_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof opt);
    return r_fd;
}

/* Returns a 1 if the wemos device already existed in the wemos vector */
bool SocketServer::checkIfWemosExists(String name){
    for (uint i = 0; i < wemosjes.size(); i++){
        String senderName;
        //Debug message
        std::cout << "senderName comparison check\nThis debug message exists at line " << __LINE__ << std::endl;
        senderName = wemosjes[i]->getSenderName();
        if (senderName == name)
        {
            return true;
        }
    }
    return false;
}

/* Removes the wemos that can be identified by it's fd. */
void SocketServer::removeWemosByFD(int fd){
    for (auto it = wemosjes.begin(); it != wemosjes.end(); )
    {
        if((*it)->getFD() == fd){
            it = wemosjes.erase(it);
        }else{
            ++it;
        }
    }
}

void SocketServer::closeConnection(int fd){
    std::cout << "\n\n\n\nClosing website connection\n\n\n\n\n\n" << std::endl;
    close(fd);
    //removeWemosByFD(fd);
    FD_CLR(fd, &all_sockets);
    return;
}
