//
// Created by ernest on 11-01-21.
//

#ifndef PJSDV_PRODUCT_H
#define PJSDV_PRODUCT_H


class product {

};

class Lights : public Wemos{

};

class Door : public Wemos{

};

class thermostat : public Wemos {

};

class bed : public Wemos{

};

class window : public Wemos{

};

class Stoel : public Wemos {
private:
    bool status;

protected:

public:
    Stoel(int fd, std::string clientName, std::string senderName);
    ~Stoel();
    inline const bool getStatus(std::string sensor)
    void sensorUpdate(bool newStatus);
    std::string handleSensorUpdate();
    void turnOnTE();
    void turnOffTE();
};

#endif //PJSDV_PRODUCT_H
