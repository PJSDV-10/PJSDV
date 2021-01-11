#pragma once

#include <string>
#include "xmlreader.h"
#include "xmlwriter.h"

class Wemos {
private:

protected:
    int fd;
    std::string clientName;
    std::string senderName;
    std::string wemosType;
    // TODO fd and type can be removed

public:

    //Getters
    //virtual const bool getStatus(std::string sensor) = 0;
    virtual std::string handleSensorUpdate();
    virtual std::string handleSiteRequest();
};

