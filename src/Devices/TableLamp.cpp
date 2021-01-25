//
// Created by ernest on 20-01-21.
//

#include "../device.h"

TableLamp::TableLamp(int fd, std::string clientName, std::string senderName) {
    this->fd = fd;
    this->clientName = clientName;
    this->senderName = senderName;
    this->wemosType = "TableLamp";
    this->PIRState = false;
}

TableLamp::~TableLamp() {
}

std::string TableLamp::handleSensorUpdate(XmlReader * xml_r) {
    /* init variables to be used */
    std::string destination;
    std::vector<double> sentStatus;
    std::vector<double> sendStatus;

    destination = xml_r->getSenderName();
    sentStatus = xml_r->getData();

    std::string toBeReturned;

    /* 
        Sensors:
            1. PIR
        
        Actuators:
            1. RGB Led

        Logic:
            If PIR is true, turn on a pleasant light

    */

    if((int)std::round(sentStatus[0])){
        sendStatus.push_back(255);
        sendStatus.push_back(255);
        sendStatus.push_back(255);
        PIRState = true;
    }else{
        sendStatus.push_back(0);
        sendStatus.push_back(0);
        sendStatus.push_back(0);
        PIRState = false;
    }


    XmlWriter xml_w("actuateBool", destination);
    xml_w.buildXMLActuate(sendStatus);
    toBeReturned = xml_w.getXML();
    xml_w.~XmlWriter();
    return toBeReturned;
}