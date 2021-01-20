//
// Created by ernest on 20-01-21.
//

#include "../device.h"

Fridge ::Fridge(int fd, std::string clientName, std::string senderName) {
    this-> fd = fd;
    this-> clientName = clientName;
    this->senderName = senderName;
    this-> wemosType = "Fridge";
}
Fridge::~Fridge() {
}