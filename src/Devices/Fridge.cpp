//
// Created by ernest on 20-01-21.
//

#include "../device.h"

Fridge::Fridge(int fd, std::string clientName, std::string senderName) {
    this-> fd = fd;
    this-> clientName = clientName;
    this->senderName = senderName;
    this-> wemosType = "fridge";
}
Fridge::~Fridge() {
}

std::string Fridge::handleSensorUpdate(XmlReader * xml_r) {
    /* init variables to be used */
    std::string destination;
    std::vector<double> sentStatus;
    std::vector<double> sendStatus;

    destination = xml_r->getSenderName();
    sentStatus = xml_r->getData();

    std::string toBeReturned;

    /*
        Sensors order:
        1: switch
        2: NTC (koelkast)
        3: NTC (peltier)

        Actuators:
        1. fan peltier
        2. peltier

        Logic:
            if switch == false
                peltier = 0
            else if switch == true && NTC (koelkast) > -30
                peltier = 1
            else if switch == true && NTC (koelkast) < -35
                peltier = 0

            if NTC (peltier) > 60
                fan = 1
            else
                fan = 0
    */

    if((int)std::round(sentStatus[0]) == false){
        sendStatus.push_back(0);
    }else if((int)std::round(sentStatus[1]) == true && sentStatus[2] >= -30){
        sendStatus.push_back(1);
    }else if((int)std::round(sentStatus[1]) == true && sentStatus[2] <= -35){
        sendStatus.push_back(0);
    }else{
        sendStatus.push_back(1);
    }

    if(sentStatus[2] > 60){
        sendStatus.push_back(1);
    }else{
        sendStatus.push_back(0);
    }


    XmlWriter xml_w("actuateBool", destination);
    xml_w.buildXMLActuate(sendStatus);
    toBeReturned = xml_w.getXML();
    xml_w.~XmlWriter();
    return toBeReturned;
}

std::string Fridge::handleWebsiteUpdate(XmlReader * xml_r) {
    /* init variables to be used */
    std::string destination;
    std::vector<double> sentStatus;
    std::vector<double> sendStatus;
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
std::string Fridge::website(XmlReader * xml_r, int i) {

    return 0;
}
