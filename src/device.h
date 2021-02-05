//
// Created by ernest on 11-01-21.
//

#ifndef PJSDV_PRODUCT_H
#define PJSDV_PRODUCT_H

#include "wemos.h"
#include <cmath>

class Wall : public Wemos{

public:
    Wall(int fd, std::string clientName, std::string senderName);
    ~Wall();
    std::string handleSensorUpdate(XmlReader *);
    std::string handleWebsiteUpdate(XmlReader *,int);

};


class Door : public Wemos{
private:
    bool PBStateOutside;
    bool PBStateInside;
    bool DoorState;

public:
    Door(int fd, std::string clientName, std::string senderName);
    ~Door();
    std::string handleSensorUpdate(XmlReader *);
    std::string handleWebsiteUpdate(XmlReader *,int);

};


class Column : public Wemos {
private:
    bool stateBuzzer;
    bool stateLED;

public:
    Column(int fd, std::string clientName, std::string senderName);
    ~Column();
    std::string handleSensorUpdate(XmlReader *);
    std::string handleWebsiteUpdate(XmlReader *,int);

};


class Bed : public Wemos{
private:
    bool PBState;
    bool FSState;
    struct timeval timer;

public:
    Bed(int fd, std::string clientName, std::string senderName);
    ~Bed();
    std::string handleSensorUpdate(XmlReader *);
    std::string handleWebsiteUpdate(XmlReader *,int);

};


class TableLamp : public Wemos{
private:
    bool PIRState;

public:
    TableLamp(int fd, std::string clientName, std::string senderName);
    ~TableLamp();
    std::string handleSensorUpdate(XmlReader *);
    std::string handleWebsiteUpdate(XmlReader *,int);
};

class Fridge : public Wemos{



public:
    Fridge(int fd, std::string clientName, std::string senderName);
    ~Fridge();
    std::string handleSensorUpdate(XmlReader *);
    std::string handleWebsiteUpdate(XmlReader *,int);

};


class Website : public Wemos{
private:


public:
    Website(int fd, std::string clientName, std::string senderName);
    ~Website();
    std::string handleSensorUpdate(XmlReader *);
    std::string handleWebsiteUpdate(XmlReader *,int);

    //XmlReader* sendStatusRequest(fd_set* all_sockets);
};


class Chair : public Wemos {
private:
    bool TEState;
    bool PBState;
    bool stepState;
protected:

public:
    Chair(int fd, std::string clientName, std::string senderName);
    ~Chair();
    std::string handleSensorUpdate(XmlReader *);
    std::string handleWebsiteUpdate(XmlReader *,int);
    //XmlReader* sendStatusRequest(fd_set*);
};

#endif //PJSDV_PRODUCT_H
