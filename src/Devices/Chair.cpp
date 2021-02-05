//
// Created by ernest on 20-01-21.
//

#include "../device.h"


Chair::Chair(int fd, std::string clientName, std::string senderName){
    this->fd = fd;
    this->clientName = clientName;
    this->senderName = senderName;
    this->wemosType = "chair";
    PBState = false;
    TEState = false;
    stepState = false;
}

Chair::~Chair(){
}

std::string Chair::handleSensorUpdate(XmlReader *xml_r){
    /* init variables to be used */
    std::string destination;
    std::vector<float> sentStatus;
    std::vector<float> sendStatus;
    //std::cout << "reading destination" << std::endl;
    destination = xml_r->getSenderName();
    //std::cout << "getting data" << std::endl;
    sentStatus = xml_r->getData();
    //std::cout << "data gotten" << std::endl;
    std::string toBeReturned;

    std::cout << "PBState status: " << PBState << std::endl;


    if ((int)std::round(sentStatus[0]) && (int)std::round(sentStatus[1]))
    {
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

std::string Chair::handleWebsiteUpdate(XmlReader * xml_r, int i) {
    std::vector<float> data;
    std::string destination;
    destination = xml_r->getClientName();
    std::string toBeSend;

    if (i == 1){
        data.push_back(1);
        data.push_back(1);
    }else {
        data.push_back(0);
        data.push_back(0);
    }

    std::cout << data[0] << data[1] << std::endl;
    XmlWriter xml_w("actuateBool", destination);
    xml_w.buildXMLActuate(data);
    toBeSend = xml_w.getXML();
    xml_w.~XmlWriter();
    return toBeSend;

}

/*XmlReader *Chair::sendStatusRequest(fd_set* all_sockets){

}*/
