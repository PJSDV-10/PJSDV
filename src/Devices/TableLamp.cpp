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
    std::vector<float> sentStatus;
    std::vector<float> sendStatus;

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

std::string TableLamp::handleWebsiteUpdate(XmlReader * xml_r, int i) {

    std::vector<float> data;
    std::string destination;
    destination = xml_r->getClientName();
    std::string toBeSend;

    if (i == 1){
        data.push_back(255);
        data.push_back(255);
        data.push_back(255);
    }else {
        data.push_back(0);
        data.push_back(0);
        data.push_back(0);
    }

    XmlWriter xml_w("actuateBool", destination);
    xml_w.buildXMLActuate(data);
    toBeSend = xml_w.getXML();
    xml_w.~XmlWriter();
    return toBeSend;
}
