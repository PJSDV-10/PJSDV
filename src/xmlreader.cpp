#include "xmlreader.h"


void XmlReader::parseDocument(){
    using namespace rapidxml;

    if(function == "authentication"){
        //Password check turned off because Wemos team thought "not validating" meant "don't send it to me"
        if(!checkPassword(context_node->first_node("password")->value())){
            return;
        }

        clientName = context_node->first_node("clientName")->value();
        //parsedContext.emplace("clientName", clientName);
        type = context_node->first_node("type")->value();
        //parsedContext.emplace("type", type);
    }
    else if (function == "sensorUpdate")
    {
        //std::cout << "Sensorupdate handling" << std::endl;
        if (!checkPassword(context_node->first_node("password")->value()))
        {
            return;
        }
        type = context_node->first_node("type")->value();
        //parsedContext.emplace("type", type);

        // Currently works only for stoel, waiting for Ernest to finish breaking up wemos types
        parseDeviceData();
    }
    else if (function == "answerToStatusRequest")
    {
        if (!checkPassword(context_node->first_node("password")->value()))
        {
            return;
        }
        type = context_node->first_node("type")->value();
        //parsedContext.emplace("type", type);
        parseDeviceData();
    }
    else if (function == "changeStatus")
    {
        clientName = context_node->first_node("clientName")->value();
        type = context_node->first_node("type")->value();
        //parsedContext.emplace("type", type);
        parseDeviceData();
    }
}

void XmlReader::parseDeviceData(){
    using namespace rapidxml;
    if(type == "chair"){
        std::cout << "Stoel identified" << std::endl;

        data.emplace_back(atoi(context_node->first_node("data1")->value())); //Force sensor
        data.emplace_back(atoi(context_node->first_node("data2")->value())); //Push button
    }else if(type == "column"){
        std::cout << "Column" << std::endl;

        data.emplace_back(atoi(context_node->first_node("data1")->value()));
        data.emplace_back(atoi(context_node->first_node("data2")->value()));
    }else if(type == "bed"){
        data.emplace_back(atoi(context_node->first_node("data1")->value()));
        data.emplace_back(atoi(context_node->first_node("data2")->value()));
    }else if(type == "tablelamp"){
        data.emplace_back(atoi(context_node->first_node("data1")->value()));

    }else if(type == "door"){
        data.emplace_back(atoi(context_node->first_node("data1")->value()));
        data.emplace_back(atoi(context_node->first_node("data2")->value()));

    }else if(type == "wall"){
        data.emplace_back(atoi(context_node->first_node("data1")->value()));
        data.emplace_back(atoi(context_node->first_node("data2")->value()));

    }else if(type == "fridge"){
        data.emplace_back(atoi(context_node->first_node("data1")->value()));
        data.emplace_back(atoi(context_node->first_node("data2")->value()));
        data.emplace_back(atoi(context_node->first_node("data3")->value()));


    }else{
        std::cout << "Unknown device type" << std::endl;
    }
}

XmlReader::XmlReader(const char *xmldoc)
{
    using namespace rapidxml;
    xml_document<> *doc = new xml_document<>;
    this->doc = doc;

    char xml[4096] = {};
    strcpy(xml, xmldoc);
    try{
        doc->parse<0>(xml);
    }catch(parse_error){
        std::cout << "A parsing error occured. Document is not valid XML" << std::endl;
        err = PARSING_ERROR;
        return;
    }
    xml_node<> *root_node = doc->first_node("message");
    this->root_node = root_node;

    /* Parsing the sender and receiver out of the header */
    xml_node<> *header_message = root_node->first_node("header");
    this->header_node = header_message;

    senderName = header_message->first_node("sender")->value();
    receiverName = header_message->first_node("receiver")->value();

    xml_node<> *function_node = root_node->first_node("function");
    this->function_node = function_node;

    function = function_node->value();

    xml_node<> *context_node = root_node->first_node("context");
    this->context_node = context_node;

    //parsedXML.emplace("sender", senderName);
    //parsedXML.emplace("receiver", receiverName);
    //parsedXML.emplace("function", function);
}

XmlReader::XmlReader(){
    std::cout << "Please do not call the constructor without any arguments" << std::endl;
}

XmlReader::~XmlReader(){

}

bool XmlReader::checkPassword(std::string password)
{
    //This has to be implemented still

    return true;
}

bool XmlReader::empty(){
    std::cout << "A check was done if an xmlreader was empty, however this function is not yet implemented."
                 "\n\rPlease don't use it, it will always return false for safety" << std::endl;
    return false;
}
