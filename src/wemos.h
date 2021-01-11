#pragma once

#include <string>
#include "xmlreader.h"
#include "xmlwriter.h"

class Wemos {
protected:
    int fd;
    std::string clientName;
    std::string senderName;
    std::string wemosType;

public:

    //Getters
    virtual const bool getStatus(std::string sensor) = 0;
};

class Stoel : private Wemos {
private:
    bool status;

public:
    Stoel(int fd, std::string clientName, std::string senderName);
    ~Stoel();
    inline const bool getStatus(std::string sensor)
    void sensorUpdate(bool newStatus);
    void turnOnTE();
    void turnOffTE();
};