//
// Created by ernest on 20-01-21.
//

#include "../device.h"

Bed::Bed(int fd, std::string clientName, std::string senderName) {
    this->fd = fd;
    this->clientName = clientName;
    this->senderName = senderName;
    this->wemosType = "bed";
}

Bed::~Bed() {
}

std::string Bed::handleSensorUpdate(XmlReader * xml_r) {

}