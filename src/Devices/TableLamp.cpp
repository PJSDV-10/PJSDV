//
// Created by ernest on 20-01-21.
//

#include "../device.h"

TableLamp::TableLamp(int fd, std::string clientName, std::string senderName) {
    this->fd = fd;
    this->clientName = clientName;
    this->senderName = senderName;
    this->wemosType = "TableLamp";
}

TableLamp::~TableLamp() {
}

std::string TableLamp::handleSensorUpdate(XmlReader * xml_r) {

}