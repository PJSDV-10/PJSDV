#pragma once

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

    //Parsing
    void parseDocument();

    //Misc
    bool checkPassword(std::string password);

    //Getters
    inline const Map getParsedDoc() const { return parsedXML; };
    inline const Map getParsedContext() const { return parsedContext; };
    inline const std::string getFunction() const { return function; };
    inline const std::string getSenderName() const { return senderName; };
    inline const std::string getReceiverName() const { return receiverName; };
    inline const std::string getClientName() const { return clientName; };
    inline const std::string getType() const { return type; };
};