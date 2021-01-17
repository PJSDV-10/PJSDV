//
// Created by ernest on 11-01-21.
//

#include "device.h"


Stoel::Stoel(std::string clientName, std::string senderName){
    this->clientName = clientName;
    this->senderName = senderName;
    this->wemosType = "stoel";
}

Stoel::~Stoel(){
}

void Stoel::doetniks(){
    std::cout << "dit doet niks" << std::endl;
}

void Stoel::sensorUpdate(bool newStatus){
    if(newStatus == status){ // aka status is the same, therefor useless
        return;
    }
    if(newStatus == true){
        turnOnTE();
    }else{
        turnOffTE();
    }
    return;
}

std::string Stoel::handleSensorUpdate(XmlReader *xml_r){
    /* init variables to be used */
    std::string destination;
    bool status;

    destination = xml_r->getSenderName();
    status = xml_r->getData();

    std::string toBeReturned;
    // Someone sits on chair boi, therefor turn it on
    XmlWriter xml_w("actuateBool", destination);
    xml_w.buildXMLActuateBool("trilElement", status);
    toBeReturned = xml_w.getXML();
    xml_w.~XmlWriter();
    return toBeReturned;
}

void Stoel::turnOnTE(){

}

void Stoel::turnOffTE(){

}