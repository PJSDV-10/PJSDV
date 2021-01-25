#include "xmlwriter.h"


std::string XmlWriter::getXML(){
    std::stringstream ss;
    ss << *doc;
    return ss.str();
}

void XmlWriter::buildXMLAck(){
    using namespace rapidxml;
    
    if(function == "authentication"){
        buildHeader();
        buildAckContext();
        doc->append_node(root_node);
    }
}

void XmlWriter::buildXMLActuate(std::vector<double> &status){
    using namespace rapidxml;

    if(function == "actuateBool"){
        buildHeader();
        buildActuateContext(status);
        doc->append_node(root_node);
    }
}

void XmlWriter::buildXMLStatusRequest(){
    using namespace rapidxml;
    buildHeader();
    buildStatusRequestContext();
    doc->append_node(root_node);
}

void XmlWriter::buildAckContext(){
    using namespace rapidxml;
    function_node = doc->allocate_node(node_element, "function", "ack");
    context_node = doc->allocate_node(node_element, "context", 0);
    root_node->append_node(function_node);
    root_node->append_node(context_node);
}

void XmlWriter::buildXMLAnswerToSR(){
    using namespace rapidxml;
    buildHeader();
    function_node = doc->allocate_node(node_element, "function", function.c_str());
    wemosjes_node = doc->allocate_node(node_element, "wemosjes");
    context_node = doc->allocate_node(node_element, "context");
    root_node->append_node(function_node);
}

void XmlWriter::finalizeAnswerToSR(){
    using namespace rapidxml;
    context_node->append_node(wemosjes_node);
    root_node->append_node(context_node);
    doc->append_node(root_node);
}

void XmlWriter::addDataToAnswer(std::string type, std::string clientName, std::vector<double> data){
    using namespace rapidxml;
    xml_node<> *wemos_node = doc->allocate_node(node_element, "wemos");
    xml_node<> *clientName_node = doc->allocate_node(node_element, "name", clientName.c_str());
    xml_node<> *type_node = doc->allocate_node(node_element, "type", type.c_str());
    wemos_node->append_node(clientName_node);
    wemos_node->append_node(type_node);
    for (std::size_t i = 0; i < data.size(); i++)
    {
        std::string dataNumber("data" + std::to_string(i + 1));
        xml_node<> *data_node = doc->allocate_node(node_element, doc->allocate_string(dataNumber.c_str()), doc->allocate_string(std::to_string((int)std::round(data[i])).c_str()));
        wemos_node->append_node(data_node);
    }
    wemosjes_node->append_node(wemos_node);
}

void XmlWriter::buildActuateContext(std::vector<double> &status){
    using namespace rapidxml;
    function_node = doc->allocate_node(node_element, "function", "actuateBool");
    context_node = doc->allocate_node(node_element, "context", 0);
    for (size_t i = 0; i < status.size(); i++){
        std::string dataNumber("data" + std::to_string(i + 1));
        xml_node<> *dataNode = doc->allocate_node(node_element, doc->allocate_string(dataNumber.c_str()), doc->allocate_string(std::to_string((int)std::round(status[i])).c_str()));
        context_node->append_node(dataNode);
    }
    root_node->append_node(function_node);
    root_node->append_node(context_node);
}

void XmlWriter::buildHeader(){
    using namespace rapidxml;
    doc = new xml_document<>;
    root_node = doc->allocate_node(node_element, "message");
    char *node_name = doc->allocate_string("header");
    header_node = doc->allocate_node(node_element, node_name); 
    sender_node = doc->allocate_node(node_element, "sender", senderName.c_str());
    receiver_node = doc->allocate_node(node_element, "receiver", receiverName.c_str());
    header_node->append_node(sender_node);
    header_node->append_node(receiver_node);
    root_node->append_node(header_node);

    
}

void XmlWriter::buildStatusRequestContext(){
    using namespace rapidxml;
    context_node = doc->allocate_node(node_element, "context", 0);
    function_node = doc->allocate_node(node_element, "function", function.c_str());
    xml_node<> *password_node = doc->allocate_node(node_element, "password", "solarwinds123");
    context_node->append_node(password_node);
    root_node->append_node(function_node);
    root_node->append_node(context_node);
}

XmlWriter::XmlWriter(std::string type, std::string dest){
    senderName = "server";
    receiverName = dest;
    function = type;
}

XmlWriter::XmlWriter(){
    std::cout << "Please do not call the constructor without any arguments" << std::endl;
}

XmlWriter::XmlWriter(XmlReader &xml_r){
    //These are reversed because when you send something back you want to send it to the one who was previously a sender
    //Insert roll safe meme here
    senderName = xml_r.getReceiverName();
    receiverName = xml_r.getSenderName();
    function = xml_r.getFunction();
}

XmlWriter::~XmlWriter(){

}