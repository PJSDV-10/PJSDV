//
// Created by ernest on 20-01-21.
//

#include "../device.h"

Wall::Wall(int fd, std::string clientName, std::string senderName) {
    this->fd = fd;
    this->clientName = clientName;
    this->senderName = senderName;
    this->wemosType = "wall";
}

Wall::~Wall() {
}

std::string Wall::handleSensorUpdate(XmlReader * xml_r) {
    /* init variables to be used */
    std::string destination;
    std::vector<float> sentStatus;
    std::vector<float> sendStatus;

    destination = xml_r->getSenderName();
    sentStatus = xml_r->getData();

    std::string toBeReturned;

    /*
        Receiving order:
        1: LDR (1-1024)
        2: potentiometer (1-1024)

        Sending order:
        1: lcd ( 1 - 100 )
        2: rgb

        first:
            if ldr / 10 > 255
                lcd = 100
            else
                lcd = ldr / 10
        second:
            if potentiometer / 4 > 255
                R = 255
            else
                R = potentiometer / 4
                G = 0
                B = 0
    */

    if((int)std::round(sentStatus[0]/10) > 100) {
        sendStatus.push_back(100);
    }else{
        sendStatus.push_back((int)std::round(sentStatus[0]/10));
    }

    if((int)std::round(sentStatus[1]/4) > 255){
        sendStatus.push_back(255);
    }else{
        sendStatus.push_back((int)std::round(sentStatus[1]/4));
    }

    sendStatus.push_back(0);
    sendStatus.push_back(0);

    XmlWriter xml_w("actuateBool", destination);
    xml_w.buildXMLActuate(sendStatus);
    toBeReturned = xml_w.getXML();
    xml_w.~XmlWriter();
    return toBeReturned;
}

std::string Wall::handleWebsiteUpdate(XmlReader * xml_r) {
    /* init variables to be used */
    std::string destination;
    std::vector<float> sentStatus;
    std::vector<float> sendStatus;
    //std::cout << "reading destination" << std::endl;
    destination = xml_r->getClientName();
    //std::cout << "getting data" << std::endl;
    sentStatus = xml_r->getData();
    //std::cout << "data gotten" << std::endl;
    std::string toBeSend;

    // if site is 1 data is 1 else data is 0
    if (sentStatus[0]){
        sendStatus.push_back(1);
        sendStatus.push_back(1);
    } else {
        sendStatus.push_back(0);
        sendStatus.push_back(0);
    }

    XmlWriter xml_w("actuateBool", destination);
    xml_w.buildXMLActuate(sendStatus);
    toBeSend = xml_w.getXML();
    xml_w.~XmlWriter();
    return toBeSend;
}
std::string Wall::website(XmlReader * xml_r, int i) {
    std::vector<float> data;
    std::string destination;
    destination = xml_r->getClientName();
    std::string toBeSend;

    if (i == 1){
        data.push_back(100);
    }else {
        data.push_back(0);
    }

    XmlWriter xml_w("actuateBool", destination);
    xml_w.buildXMLActuate(data);
    toBeSend = xml_w.getXML();
    xml_w.~XmlWriter();
    return toBeSend;
    return 0;
}
