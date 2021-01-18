//
// Created by ernest on 11-01-21.
//

#ifndef PJSDV_PRODUCT_H
#define PJSDV_PRODUCT_H

#include "wemos.h"
#include <cmath>

class product {

};

class Wall : public Wemos{

};

class Door : public Wemos{

};

class Column : public Wemos {
private:
    bool stateBuzzer;
    bool stateLED;

public:
    Column(int fd, std::string clientName, std::string senderName);
    ~Column();
    std::string handleSensorUpdate(XmlReader *);
};

class Bed : public Wemos{

};

class TableLamp : public Wemos{

};

class Fridge : public Wemos{

};

class Website : public Wemos{
private:
    

public:
    Website(int fd, std::string clientName, std::string senderName);
    ~Website();
    std::string handleSensorUpdate(XmlReader *);
    //XmlReader* sendStatusRequest(fd_set* all_sockets);
};

class Stoel : public Wemos {
private:
    bool TEState;
    bool PBState;

protected:

public:
    Stoel(int fd, std::string clientName, std::string senderName);
    ~Stoel();
    std::string handleSensorUpdate(XmlReader*);
    //XmlReader* sendStatusRequest(fd_set*);
};

#endif //PJSDV_PRODUCT_H
