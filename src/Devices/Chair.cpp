//
// Created by ernest on 20-01-21.
//

#include "../device.h"


Chair::Chair(int fd, std::string clientName, std::string senderName){
    this->fd = fd;
    this->clientName = clientName;
    this->senderName = senderName;
    this->wemosType = "stoel";
}

Chair::~Chair(){
}

std::string Chair::handleSensorUpdate(XmlReader *xml_r){
    /* init variables to be used */
    std::string destination;
    std::vector<double> sentStatus;
    std::vector<double> sendStatus;
    std::cout << "reading destination" << std::endl;
    destination = xml_r->getSenderName();
    std::cout << "getting data" << std::endl;
    sentStatus = xml_r->getData();
    std::cout << "data gotten" << std::endl;
    std::string toBeReturned;
    /*
        force sensor + push button
        if both are on, then send 1, otherwise 0. XOR them
     */
    if(std::round(sentStatus[0]) && std::round(sentStatus[1])){
        std::cout << "both are on" << std::endl;
        sendStatus.push_back(1);
        sendStatus.push_back(1);
        TEState = 1;
        PBState = 1;
    }
    else{
        std::cout << "not both are on" << std::endl;
        sendStatus.push_back(0);
        sendStatus.push_back(0);
        TEState = 0;
        PBState = 0;
    }

    XmlWriter xml_w("actuateBool", destination);
    xml_w.buildXMLActuate(sendStatus);
    toBeReturned = xml_w.getXML();
    xml_w.~XmlWriter();
    return toBeReturned;
}

/*XmlReader *Chair::sendStatusRequest(fd_set* all_sockets){

}*/
