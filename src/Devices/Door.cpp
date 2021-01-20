//
// Created by ernest on 20-01-21.
//

#include "../device.h"

Door::Door(int fd, std::string clientName, std::string senderName) {
    this->fd = fd;
    this->clientName = clientName;
    this->senderName = senderName;
    this->wemosType = "door";
}

Door::~Door() {
}

std::string Door::handleSensorUpdate(XmlReader * xml_r) {

}