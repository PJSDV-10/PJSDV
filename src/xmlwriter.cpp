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

void XmlWriter::buildAckContext(){
    using namespace rapidxml;
    function_node = doc->allocate_node(node_element, "function", "ack");
    context_node = doc->allocate_node(node_element, "context", 0);
    root_node->append_node(function_node);
    root_node->append_node(context_node);
}

void XmlWriter::buildActuateContext(std::vector<double> &status){
    using namespace rapidxml;
    function_node = doc->allocate_node(node_element, "function", "actuateBool");
    context_node = doc->allocate_node(node_element, "context", 0);
    for (int i = 0; i < status.size(); i++){
        xml_node<> *dataNode = doc->allocate_node(node_element, "data" + i, std::to_string(std::round(status[i])).c_str());
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