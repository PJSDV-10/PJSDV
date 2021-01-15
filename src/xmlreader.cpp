#include "xmlreader.h"

void XmlReader::parseDocument(){
    using namespace rapidxml;

    if(function == "authentication"){
        //Password check turned off because Wemos team thought "not validating" meant "don't send it to me"
        /*if(!checkPassword(context_node->first_node("password")->value())){
            return;
        }*/
        clientName = context_node->first_node("clientName")->value();
        parsedContext.emplace("clientName", clientName);
        type = context_node->first_node("type")->value();
        parsedContext.emplace("type", type);
        /*Array capabilities;
        for (xml_node<> *func_node = context_node->first_node("capabilities")->first_node("func"); func_node; func_node = func_node->next_sibling())
        {
            std::string type, funcName;
            type = func_node->first_node("type")->value();
            funcName = func_node->first_node("funcName")->value();
            Map function;
            function.emplace("type", type);
            function.emplace("name", funcName);
            capabilities.push_back(Wrapper(function));
        }
        parsedContext.emplace("capabilities", capabilities);
        parsedXML.emplace("context", parsedContext);
        */
        /* 
        ParsedXML
            senderName : string
            receiverName : string
            function : string
            ParsedContext : map
                clientName : string
                capabilities : array
                    type : string
                    func : string
         */
    }else if(function == "sensorUpdate"){
        /*if(!checkPassword(context_node->first_node("password")->value())){
            return;
        }*/
        // Currently works only for stoel, waiting for Ernest to finish breaking up wemos types
        type = context_node->first_node("type")->value();
        parsedContext.emplace("type", type);
        data = atoi(context_node->first_node("data")->value());
        parsedContext.emplace("data", data);
        
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

    parsedXML.emplace("sender", senderName);
    parsedXML.emplace("receiver", receiverName);
    parsedXML.emplace("function", function);
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
    std::cout << "A check was done if an xmlreader was empty, however this function is not yet implemented.\n\rPlease don't use it, it will always return false for safety" << std::endl;
    return false;
}