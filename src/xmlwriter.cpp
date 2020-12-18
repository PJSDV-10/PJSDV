#include "xmlwriter.h"


std::string XmlWriter::getXML(){
    std::stringstream ss;
    ss << doc;
    return ss.str();
}

void XmlWriter::buildXML(){
    using namespace rapidxml;
    
    if(function == "authentication"){
        buildHeader();
        buildAckContext();
        doc->append_node(root_node);
    }
}

void XmlWriter::buildAckContext(){
    using namespace rapidxml;
    function_node = doc->allocate_node(node_element, "function", function.c_str());
    context_node = doc->allocate_node(node_element, "context", 0);
    root_node->append_node(function_node);
    root_node->append_node(context_node);
}

void XmlWriter::buildHeader(){
    using namespace rapidxml;
    header_node = doc->allocate_node(node_element, "header", 0);
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
