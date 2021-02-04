#pragma once

#include <string>
#include <sys/socket.h>
#include "xmlreader.h"
#include "xmlwriter.h"

class Wemos {
private:

protected:
    std::string clientName;
    std::string senderName;
    std::string wemosType;
    int fd;

public:

    //Getters
    inline const std::string getSenderName() { return senderName; };
    inline const std::string getClientName() { return clientName; };
    inline const std::string getWemosType() { return wemosType; };
    inline const int getFD() { return fd; };

    //virtual const bool getStatus(std::string sensor) = 0;
    virtual std::string handleSensorUpdate(XmlReader *) = 0;
    virtual std::string handleWebsiteUpdate(XmlReader *) = 0;
    //virtual XmlReader* sendStatusRequest();
    //    virtual std::string handleSiteRequest();
};
