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

Website::Website(std::string clientName, std::string senderName){
    this->clientName = clientName;
    this->senderName = senderName;
    this->wemosType = "stoel";
}

Website::~Website(){

}

std::string Website::handleSensorUpdate(XmlReader* xml_r){
    /* Realistically the actual function is called getStatusAll, but this works fine too */
    std::string destination;
    std::vector<double> sentStatus;
    std::vector<double> sendStatus;

    destination = xml_r->getSenderName();
    sentStatus = xml_r->getData();

    std::string toBeReturned;

    toBeReturned = "<message><header><sender>server</sender><receiver>website</receiver></header><function>answerToStatusRequest</function><context></context></message>";
    return toBeReturned;
}
