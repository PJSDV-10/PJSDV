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
    virtual ~Wemos();

    //Getters
    inline const std::string getSenderName() { return senderName; };
    inline const std::string getClientName() { return clientName; };
    inline const std::string getWemosType() { return wemosType; };

    //virtual const bool getStatus(std::string sensor) = 0;
    virtual void doetniks() = 0;
    virtual std::string handleSensorUpdate(XmlReader *) = 0;
    //    virtual std::string handleSiteRequest();
    
};
