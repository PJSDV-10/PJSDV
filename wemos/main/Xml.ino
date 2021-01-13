//TiXmlDocument*//
void Buildheader(TiXmlElement * message) { 
  //this function wil make the start of evry masage to the end of the header
  //deze functie maakt het begin van het bericht totenmet de einde van de header

      TiXmlElement * header = new TiXmlElement( "header" );
        TiXmlElement * senderElement = new TiXmlElement( "sender" );
          TiXmlText * senderText = new TiXmlText( wemosNaam );
        senderElement->LinkEndChild(senderText);
      header->LinkEndChild(senderElement);
        TiXmlElement * receiverElement = new TiXmlElement( "receiver" );
          TiXmlText * receiverText = new TiXmlText( server );
        receiverElement->LinkEndChild(receiverText);
       header->LinkEndChild(receiverElement);
    message->LinkEndChild(header);
}

char* buildAuthenticationMsg() {
  TiXmlDocument Msg;
  
  TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * message = new TiXmlElement( "message" );
    //header
      Buildheader(message);
    //function
      TiXmlElement * functionElement = new TiXmlElement( "function" );
        TiXmlText * functionText = new TiXmlText( "authentication" );
      functionElement->LinkEndChild(functionText);
    message->LinkEndChild(functionElement);
    // context
      TiXmlElement * context = new TiXmlElement( "context" );
        TiXmlElement * clientNameElement = new TiXmlElement( "clientName" );
          TiXmlText * clientNameText = new TiXmlText( wemosNaam );
          clientNameElement->LinkEndChild(clientNameText);
        context->LinkEndChild(clientNameElement);
        TiXmlElement * typeElement = new TiXmlElement( "type" );
          TiXmlText * typeText = new TiXmlText( type );
          typeElement->LinkEndChild(typeText);
        context->LinkEndChild(typeElement); 

        TiXmlElement * capabilitiesElement = new TiXmlElement( "capabilities" );
        // all the functions of the given wemos board, needs to be edited for every wemos
        for (int i = 0; i < AMOUNTOFSENSORS; i++) { 
         TiXmlElement * funcElement = new TiXmlElement( "func" );
            TiXmlElement * typeElement = new TiXmlElement( "type" );
              TiXmlText * typeText = new TiXmlText( sensorNames[i][0] );
              typeElement->LinkEndChild(typeText);
            funcElement->LinkEndChild(typeElement);
        
            TiXmlElement * funcNameElement = new TiXmlElement( "funcName" );
              TiXmlText * funcNameText = new TiXmlText( sensorNames[i][1] );
            funcNameElement->LinkEndChild(funcNameText);
          funcElement->LinkEndChild(funcNameElement);
        
          capabilitiesElement->LinkEndChild(funcElement);
        }

        // do the same for the actuators
        for (int i = 0; i < AMOUNTOFACTUATORS; i++) { 
        
      TiXmlElement * funcElement = new TiXmlElement( "func" );
        TiXmlElement * typeElement = new TiXmlElement( "type" );
          TiXmlText * typeText = new TiXmlText(actuatorNames[i][0]);

        typeElement->LinkEndChild(typeText);
      funcElement->LinkEndChild(typeElement);

        TiXmlElement * funcNameElement = new TiXmlElement( "funcName" );

          TiXmlText * funcNameText = new TiXmlText(actuatorNames[i][1]); 
        funcNameElement->LinkEndChild(funcNameText);
      funcElement->LinkEndChild(funcNameElement);
      
      capabilitiesElement->LinkEndChild(funcElement);
      
      }
      context->LinkEndChild(capabilitiesElement);
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
    

    
  return stringMsg;
}

char* buildStatusMsg(char* function){
  // builds an awnser message that's to be send to the server.
  TiXmlDocument Msg;
  //Serial.println("Starting to build an AwnserMessage");
  TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * message = new TiXmlElement( "message" );
    //header
      Buildheader(message);
    //function
      TiXmlElement * functionElement = new TiXmlElement( "function" );
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
    

    
  return stringMsg;
}
