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
};


class Door : public Wemos{

public:
    Door(int fd, std::string clientName, std::string senderName);
    ~Door();
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

public:
    Bed(int fd, std::string clientName, std::string senderName);
    ~Bed();
};


class TableLamp : public Wemos{

public:
    TableLamp(int fd, std::string clientName, std::string senderName);
    ~TableLamp();
};

class Fridge : public Wemos{

public:
    Fridge(int fd, std::string clientName, std::string senderName);
    ~Fridge();
    std::string handleSensorUpdate(XmlReader *);

};


class Website : public Wemos{
private:
    

public:
    Website(int fd, std::string clientName, std::string senderName);
    ~Website();
    std::string handleSensorUpdate(XmlReader *);
    //XmlReader* sendStatusRequest(fd_set* all_sockets);
};


class Chair : public Wemos {
private:
    bool TEState;
    bool PBState;

protected:

public:
    Chair(std::string clientName, std::string senderName);
    ~Chair();
    std::string handleSensorUpdate(XmlReader*);
    //XmlReader* sendStatusRequest(fd_set*);
};

#endif //PJSDV_PRODUCT_H
