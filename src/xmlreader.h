#pragma once

#define PARSING_ERROR 1

#include "wrapper.h"
#include <string.h>
#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

class XmlReader{
private:
    //Support variables
    std::string function;
    std::string senderName;
    std::string receiverName;
    std::string clientName;
    std::string type;
    std::vector<double> data;
    int err;

    Map parsedXML;
    Map parsedContext;

    //RapidXML variables
    rapidxml::xml_document<> *doc;
    rapidxml::xml_node<> *root_node;
    rapidxml::xml_node<> *header_node;
    rapidxml::xml_node<> *function_node;
    rapidxml::xml_node<> *context_node;

    //Different function context parsers
    Map parseAuthContext();

public:
    XmlReader();
    XmlReader(const char *xmldoc);
    ~XmlReader();

    //Parsing
    void parseDocument();

    //Misc
    bool checkPassword(std::string password);
    bool empty();
    inline void reseterr() { err = 0; };

    //Getters
    inline const Map getParsedDoc() const { return parsedXML; };
    inline const Map getParsedContext() const { return parsedContext; };
    inline const std::string getFunction() const { return function; };
    inline const std::string getSenderName() const { return senderName; };
    inline const std::string getReceiverName() const { return receiverName; };
    inline const std::string getClientName() const { return clientName; };
    inline const std::string getType() const { return type; };
    inline std::vector<double> &getData() { return data; };
    inline const int error() const { return err; };
};
