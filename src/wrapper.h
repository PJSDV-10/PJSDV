#pragma once

#include <map>
#include <vector>
#include <string>
#include <variant>

struct Wrapper;
class Wemos{
private:
    int fd;
    std::string clientName;
    std::string senderName;
    std::vector<Wrapper> capabilities;

public:
    Wemos(std::vector<Wrapper> cap, std::string senderName, std::string clientName);
    
    //Getters
    inline const std::vector<Wrapper> getCapabilities() const { return capabilities; };
    inline const std::string getSenderName() const { return senderName; };
};
using Number = double;
using String = std::string;
using Array = std::vector<Wrapper>;
using Map = std::map<String, Wrapper>;
using Value = std::variant<Number, String, Array, Map, Wemos>;

struct Wrapper
{
    Wrapper() = default;
    Wrapper(Wrapper &) = default;
    Wrapper(const Wrapper &) = default;
    Wrapper(Value &value);
    Wrapper(const Value &value);

    Wrapper &operator=(Wrapper const &) = default;

    operator Value &();
    operator const Value &() const;

    Value &value() ;
    Value const &value() const ;

private:
    Value v{};
};

//using Value = std::variant<double,std::string,std::vector<Wrapper>,std::map<std::string,Wrapper>>;