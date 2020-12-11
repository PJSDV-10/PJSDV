#include "socketconn.h"

SocketConnection::SocketConnection(int fd, std::string name, std::string type, struct addrinfo data){
    this->f_desc = fd;
    this->name = name;
    this->type = type;
    this->a_data = data;
}
SocketConnection::SocketConnection(int fd, std::string name, std::string type){
    this->f_desc = fd;
    this->name = name;
    this->type = type;
}

void SocketConnection::monitorSocket()
{
    /*const char *acceptMsg = "Source:Server,Action:Accept\n\r";
    if (send(fd, acceptMsg, strlen(acceptMsg), 0) == -1)
    {
        perror("Failed sending accept message");
        exit(EXIT_FAILURE);
    }*/
    char msgBuf[1024] = {0};
    while (true)
    {
        strncpy(msgBuf, "", 0);
        recv(f_desc, msgBuf, 1024, 0);
        std::cout << "Received Message: \n\""
                  << msgBuf << "\"" << std::endl;
        std::map<std::string, std::string> parsed = parseMessage(msgBuf);
        try
        {
            std::string func = parsed.at("Function");
            if (func == "Close")
            {
                std::cout << "Closing connection with remote" << std::endl;
                close(f_desc);
                return;
            }
            else if (func == "KA")
            {
                std::cout << "Keeping alive." << std::endl;
                send(f_desc, "OK", 2, 0);
                continue;
            }
        }
        catch (const std::out_of_range &oor)
        {
            std::cout << "oh no" << std::endl;
            send(f_desc, "Error", 5, 0);
            continue;
        }
    }
    return;
}


std::map<std::string, std::string> SocketConnection::parseMessage(const char *h)
{
    std::map<std::string, std::string> parsed;
    std::string::iterator ptr;
    std::string msg = h;
    std::string delim = ",";
    std::string idelim = ":";
    std::string ending = "\r\n";

    size_t start = 0;
    size_t pos = 0;
    size_t pos2 = 0;
    while (true)
    {
        if (msg == "")
        {
            return {};
        }
        else if ((pos = msg.find(delim)) != std::string::npos)
        {
            if ((pos2 = msg.find(idelim)) != std::string::npos)
            {
                std::string tmpkey, tmpval;
                /*
             Start  pos2  pos
                |     |    |
                Source:Host,Function:Accept\n\r
                 */
                tmpkey = msg.substr(start, pos2);
                tmpval = msg.substr(pos2 + 1, (pos - (pos2 + 1)));
                parsed.emplace(tmpkey, tmpval);
                start = pos + 1;
                continue;
            }
            else
            {
                return {};
            }
        }
        else if ((pos = msg.find(ending)) != std::string::npos)
        {
            if ((pos2 = msg.find(idelim)) != std::string::npos)
            {
                std::string tmpkey, tmpval;
                tmpkey = msg.substr(start, pos2);
                tmpval = msg.substr(pos2 + 1, (pos - (pos2 + 1)));
                parsed.emplace(tmpkey, tmpval);
                break;
            }
            else
            {
                return {};
            }
        }
        else
        {
            return {};
        }
    }
    return parsed;
}