#pragma once

#include <map>
#include <vector>
#include <string>
#include <variant>

struct Wrapper;
using Number = double;
using String = std::string;
using Array = std::vector<Wrapper>;
using Map = std::map<String, Wrapper>;
using Value = std::variant<Number, String, Array, Map>;

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