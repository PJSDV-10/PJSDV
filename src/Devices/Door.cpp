//
// Created by ernest on 20-01-21.
//

#include "../device.h"

Door::Door(int fd, std::string clientName, std::string senderName) {
    this->fd = fd;
    this->clientName = clientName;
    this->senderName = senderName;
    this->wemosType = "door";
    PBStateInside = false;
    PBStateOutside = false;
}

Door::~Door() {
}

std::string Door::handleSensorUpdate(XmlReader * xml_r) {
    /* init variables to be used */
    std::string destination;
    std::vector<double> sentStatus;
    std::vector<double> sendStatus;

    destination = xml_r->getSenderName();
    sentStatus = xml_r->getData();

    std::string toBeReturned;

    /* 
        Receiving order:
        1: push button (buiten)
        2: push button (binnen)

        Sending order:
        1: LED (buiten)
        2: LED (binnen)
        3: Servo (0 is closed, is 1 open)

        first:
            if push button binnen and/or push button buiten en doorstate false
                servo 1
                led buiten 1
                led binnen 1
            if push button binnen and/or push button buiten en doorstate true
                servo 2
                led buiten 0
                led binnen 0
    */

    if(((int)std::round(sentStatus[0]) || (int)std::round(sentStatus[1])) && DoorState == false){
        sendStatus.push_back(1); //led buiten
        sendStatus.push_back(1); //led binnen
        sendStatus.push_back(1); //servo
        DoorState = true;
    }else if((int)std::round(sentStatus[0]) || (int)std::round(sentStatus[1]) && DoorState == true){
        sendStatus.push_back(0); //led buiten
        sendStatus.push_back(0); //led binnen
        sendStatus.push_back(0); //servo
        DoorState = false;
    }
    
    XmlWriter xml_w("actuateBool", destination);
    xml_w.buildXMLActuate(sendStatus);
    toBeReturned = xml_w.getXML();
    xml_w.~XmlWriter();
    return toBeReturned;
}