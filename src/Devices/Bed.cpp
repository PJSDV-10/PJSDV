//
// Created by ernest on 20-01-21.
//

#include "../device.h"

Bed::Bed(int fd, std::string clientName, std::string senderName) {
    this->fd = fd;
    this->clientName = clientName;
    this->senderName = senderName;
    this->wemosType = "bed";
    PBState = false;
    FSState = false;
    timer.tv_sec = 0;
    timer.tv_usec = 0;
}

Bed::~Bed() {
}

std::string Bed::handleSensorUpdate(XmlReader * xml_r) {
    /* init variables to be used */
    std::string destination;
    std::vector<float> sentStatus;
    std::vector<float> sendStatus;

    destination = xml_r->getSenderName();
    sentStatus = xml_r->getData();

    std::string toBeReturned;

    /*
        Receiving order:
        1: push button
        2: force sensor

        Sending order:
        1: LED

        first:
            make pushbutton to switch using PBState
        second:
            turn on LED is pbstate is true
        third:
            save the time and compare it ever time before the select function in socketServer...
            The LED has to turn off after 30 minutes automatically.
    */

    // Make pushbutton to switch
    if((int)std::round(sentStatus[0]) != PBState){
        PBState = !PBState;
    }

    // Second logic
    if(PBState){
        sendStatus.push_back(1);
    }else{
        sendStatus.push_back(0);
    }



    /* Todo: add a third logic thing */
    XmlWriter xml_w("actuateBool", destination);
    xml_w.buildXMLActuate(sendStatus);
    toBeReturned = xml_w.getXML();
    xml_w.~XmlWriter();
    return toBeReturned;
}

std::string Bed::handleWebsiteUpdate(XmlReader * xml_r) {
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
std::string Bed::website(XmlReader * xml_r, int i) {
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

    XmlWriter xml_w("actuateBool", destination);
    xml_w.buildXMLActuate(data);
    toBeSend = xml_w.getXML();
    xml_w.~XmlWriter();
    return toBeSend;
}
