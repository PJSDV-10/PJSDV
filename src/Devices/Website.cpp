//
// Created by ernest on 11-01-21.
//

#include "../device.h"

Website::Website(int fd, std::string clientName, std::string senderName){
    this->fd = fd;
    this->clientName = clientName;
    this->senderName = senderName;
    this->wemosType = "website";
}

Website::~Website(){
}

std::string Website::handleSensorUpdate(XmlReader* xml_r){
    /* Realistically the actual function is called getStatusAll, but this works fine too */
    std::string destination;
    std::vector<double> sentStatus;
    std::vector<double> sendStatus;
    std::cout << "reading destination" << std::endl;
    destination = xml_r->getSenderName();

    sentStatus = xml_r->getData();

    std::string toBeReturned;

    toBeReturned = "<message><header><sender>server</sender><receiver>website</receiver></header><function>answerToStatusRequest</function><context></context></message>";
    return toBeReturned;
}

std::string Website::handleWebsiteUpdate(XmlReader* xml_r) {
    std::string destination;
    std::vector<double> sentStatus;
    std::vector<double> sendStatus;
    std::cout << "reading destination" << std::endl;
    destination = xml_r->getSenderName();

    sentStatus = xml_r->getData();

    std::string toBeReturned;

    toBeReturned = "<message><header><sender>server</sender><receiver>website</receiver></header><function>answerToUpdateRequest</function><context></context></message>";
    return toBeReturned;
}

/*XmlReader *Website::sendStatusRequest(fd_set* all_sockets){
    XmlWriter xml_w("getStatusBroadcast", "allWemos");
    xml_w.buildXMLStatusRequest();
    std::string toBeSent = xml_w.getXML();
    xml_w.~XmlWriter();
    std::cout << "Sending status request to all wemos devices" << std::endl;
    XmlWriter xml_w("answerToStatusRequest", "website");
    xml_w.buildXMLAnswerToSR();
    for (int i = 0; i < FD_SETSIZE; i++)
    {
        if(FD_ISSET(i, all_sockets)){
            ssize_t error = send(i, toBeSent.c_str(), strlen(toBeSent.c_str()), 0);
            if(error = -1){
                if(errno = ECONNRESET){
                    std::cout << "During sending of status request a socket appear closed or unresponsive\n\rNot closing the connection yet, but beware" << std::endl;
                    errno = 0;
                }
            }
            char buffer[4096] = {0};
            int bytesread;
            if ((bytesread = recv(i, buffer, 4096, 0)) == -1)
            {
                perror("Error receiving");
                exit(EXIT_FAILURE);
            }
            XmlReader xml_r(buffer);
            xml_r.parseDocument();
            xml_w.addDataToAnswer(xml_r.getType(), xml_r.getClientName(), xml_r.getData());
            xml_r.~XmlReader();
        }
    }
    xml_w.finalizeAnswerToSR();
    std::string sendBack;
    sendBack = xml_w.getXML();

}*/
