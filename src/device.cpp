//
// Created by ernest on 11-01-21.
//

#include "device.h"


Stoel::Stoel(int fd, std::string clientName, std::string senderName){
    this->clientName = clientName;
    this->senderName = senderName;
    this->fd = fd;
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

    destination = std::get<std::string>(xml_r->getParsedDoc().find("sendername")->second.value());
    status = std::round(std::get<double>(xml_r->getParsedContext().find("data")->second.value()));

    std::string toBeReturned;
    // Someone sits on chair boy, therefor turn it on
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