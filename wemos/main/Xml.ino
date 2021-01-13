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

TiXmlDocument buildAuthenticationMsg() {
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
        TiXmlElement * passwordElement = new TiXmlElement( "password" );
          TiXmlText * passwordText = new TiXmlText( password );
          passwordElement->LinkEndChild(passwordText);
        context->LinkEndChild(passwordElement);
        TiXmlElement * clientNameElement = new TiXmlElement( "clientName" );
          TiXmlText * clientNameText = new TiXmlText( wemosNaam );
          clientNameElement->LinkEndChild(clientNameText);
        context->LinkEndChild(clientNameElement);
        TiXmlElement * typeElement = new TiXmlElement( "type" );
          TiXmlText * typeText = new TiXmlText( type );
          typeElement->LinkEndChild(typeText);
        context->LinkEndChild(typeElement); 

        // all the functions of the given wemos board, needs to be edited for every wemos
        TiXmlElement * func1Element = new TiXmlElement( "func" );
          TiXmlElement * type1Element = new TiXmlElement( "type" );
            TiXmlText * type1Text = new TiXmlText( "sensorBool" );
            type1Element->LinkEndChild(type1Text);
          func1Element->LinkEndChild(type1Element);
        
          TiXmlElement * funcName1Element = new TiXmlElement( "funcName" );
            TiXmlText * funcName1Text = new TiXmlText( "lampKnop" );
          funcName1Element->LinkEndChild(funcName1Text);
        func1Element->LinkEndChild(funcName1Element);
        
        context->LinkEndChild(func1Element);
          
      message->LinkEndChild(context);
  
  Msg.LinkEndChild( message );
  return Msg;
}

TiXmlDocument buildStatusMsg(char * function){
  // builds an awnser message that's to be send to the server.
  TiXmlDocument anwserMsg;
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
          TiXmlText * nameText = new TiXmlText( sensorNames[i][1] );

        nameElement->LinkEndChild(nameText);
      sensorElement->LinkEndChild(nameElement);

        TiXmlElement * statusElement = new TiXmlElement( "status" );

        char strong[5];
        itoa(sensor[i][1], strong, 10);

          TiXmlText * statusText = new TiXmlText(strong); 
        statusElement->LinkEndChild(statusText);
      sensorElement->LinkEndChild(statusElement);
      context->LinkEndChild(sensorElement);
      
      }
      // TODO: the same for actuators
      //Serial.println("Finished up context");
      
      message->LinkEndChild(context);
  
  anwserMsg.LinkEndChild( message );
  return anwserMsg;
}
