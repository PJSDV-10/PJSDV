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
        2: L

        first:
            make pushbutton to switch using PBState
        second:
            turn on LED is pbstate is true
        third: 
            save the time and compare it ever time before the select function in socketServer...
            The LED has to turn off after 30 minutes automatically.
    */

    
    XmlWriter xml_w("actuateBool", destination);
    xml_w.buildXMLActuate(sendStatus);
    toBeReturned = xml_w.getXML();
    xml_w.~XmlWriter();
    return toBeReturned;
}