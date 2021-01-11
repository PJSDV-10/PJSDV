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



void Stoel::sensorUpdate(bool newStatus){
    if(newStatus = status){ // aka status is the same, therefor useless
        return;
    }
    if(newStatus == true){
        turnOnTE();
    }else{
        turnOffTE();
    }
    return;
}

void Stoel::turnOnTE(){

}

void Stoel::turnOffTE(){

}