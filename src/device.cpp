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

/* Completely unused */
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
    std::vector<double> sentStatus;
    std::vector<double> sendStatus;

    destination = xml_r->getSenderName();
    sentStatus = xml_r->getData();

    std::string toBeReturned;
    /* 
        force sensor + push button
        if both are on, then send 1, otherwise 0. XOR them
     */
    if(std::round(sentStatus[0]) && std::round(sentStatus[1])){
        sendStatus.push_back(1);
        sendStatus.push_back(1);
        TEState = 1;
        PBState = 1;
    }
    else{
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

void Stoel::turnOnTE(){

}

void Stoel::turnOffTE(){

}