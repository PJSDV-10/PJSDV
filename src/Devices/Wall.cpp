//
// Created by ernest on 20-01-21.
//

#include "../device.h"

Column::Column(int fd, std::string clientName, std::string senderName) {
    this->fd = fd;
    this->clientName = clientName;
    this->senderName = senderName;
    this->wemosType = "Column";
}

Column::~Column() {
}