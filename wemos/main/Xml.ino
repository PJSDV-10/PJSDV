//TiXmlDocument*//
std::string Buildheader() {  //build header with the gloals specified
    std::string temp ="<message><header><sender>"+ wemosNaam +"</sender><receiver>"+ server +"</receiver></header>"; 
    return temp;
}
/*std::string buildcapabilities() {
  std::string temp = "<capabilities>";

  for(int i = 0;i<AMOUNTOFSENSORS;i++){ //voeg elke keer neeiwe sensot toe
    temp += "<func><type>" + sensorNames[i][0] + "</type><funcName>" + sensorNames[i][1] + "</funcName></func>";
  }
  
   for(int i = 0;i<AMOUNTOFACTUATORS;i++){ //voeg elke keer neeiwe actuator toe
    temp += "<func><type>" + actuatorNames[i][0] + "</type><funcName>" + actuatorNames[i][1] + "</funcName></func>";
  }
  
  temp+= "</capabilities>";
  return temp;
} dam this isnt even necesary anymore*/ 


std::string buildAuthenticationMsg() {
   //dammit mad this entire thing only to notice im using the wrong file.............................
   //std::string temp = Buildheader() +"<function>authentication</function><context><password>"+wachtwoord+"</password><clientName>"+wemosNaam+"</clientName><type>"+type+"</type>"+ buildcapabilities()+" </context> </message>";
   std::string temp = Buildheader() + " <function>authentication</function><context><password>" +wachtwoord+ "</password><clientName>" + wemosNaam + "</clientName><type>" + type + "</type></context></message>" ;
   return temp;
}

/*char* buildStatusMsg(char* function){
  // builds an awnser message that's to be send to the server.
                                                                            //TiXmlDocument Msg;
                                                                            //Serial.println("Starting to build an AwnserMessage");
                                                                            //TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
                                                                             // TiXmlElement * message = new TiXmlElement( "message" );
    //header
     std::string build = Buildheader();
    //function
                                                                               /* TiXmlElement * functionElement = new TiXmlElement( "function" );
                                                                                  TiXmlText * functionText = new TiXmlText( function );
                                                                                functionElement->LinkEndChild(functionText);
                                                                              message->LinkEndChild(functionElement);
    // context
    //Serial.println("building context");
      TiXmlElement * context = new TiXmlElement( "context" );
      
      // voor elke sensor een apart sensor element toevoegen
      for (int i = 0; i < AMOUNTOFSENSORS; i++) { 
        
      TiXmlElement * sensorElement = new TiXmlElement( "sensor" );
        TiXmlElement * nameElement = new TiXmlElement( "name" );
          TiXmlText * nameText = new TiXmlText(sensorNames[i][1]);

        nameElement->LinkEndChild(nameText);
      sensorElement->LinkEndChild(nameElement);

        TiXmlElement * statusElement = new TiXmlElement( "status" );

        char strong[5];
        itoa(sensor[i][0], strong, 10);

          TiXmlText * statusText = new TiXmlText(strong); 
        statusElement->LinkEndChild(statusText);
      sensorElement->LinkEndChild(statusElement);
      context->LinkEndChild(sensorElement);
      
      }
      // TODO: the same for actuators
      for (int i = 0; i < AMOUNTOFACTUATORS; i++) { 
        
      TiXmlElement * actuatorElement = new TiXmlElement( "actuator" );
        TiXmlElement * nameElement = new TiXmlElement( "name" );
          TiXmlText * nameText = new TiXmlText(actuatorNames[i][1]);

        nameElement->LinkEndChild(nameText);
      actuatorElement->LinkEndChild(nameElement);

        TiXmlElement * statusElement = new TiXmlElement( "status" );

        char strong[5];
        itoa(actuator[i][0], strong, 10);

          TiXmlText * statusText = new TiXmlText(strong); 
        statusElement->LinkEndChild(statusText);
      actuatorElement->LinkEndChild(statusElement);
      context->LinkEndChild(actuatorElement);
      
      }
      
      message->LinkEndChild(context);

    Msg.LinkEndChild( message );
    
  // turn the message into a c string
  TiXmlPrinter printer;
    printer.SetIndent("\t");
    Msg.Accept(&printer);
    char* stringMsg = "";

    strcpy(stringMsg, printer.CStr());

    //TODO: free up the used memory
    //delete message;
    

  //delete(&Msg); werkt niet // check dit https://stackoverflow.com/questions/853559/what-memory-management-do-i-need-to-cleanup-when-using-tinyxml-for-c   dont have to be deleted
  return stringMsg;
}
*/
