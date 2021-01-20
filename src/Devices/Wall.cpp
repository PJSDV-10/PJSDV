//
// Created by ernest on 20-01-21.
//

#include "../device.h"

Wall::Wall(int fd, std::string clientName, std::string senderName) {
    this->fd = fd;
    this->clientName = clientName;
    this->senderName = senderName;
    this->wemosType = "wall";
}

Wall::~Wall() {
}

std::string Wall::handleSensorUpdate(XmlReader * xml_r) {

}
