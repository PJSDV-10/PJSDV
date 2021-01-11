// This is testcode for now, since we need to make 7 different wemos codes eventually
// authentication problably works, but didn't have time to test yet
// based on the "wall" wemos from the excel file on blackboard 
// connections:
// A0 - LDR     A1 - potmeter       DO4 - LCDPanel     D5 - RGBLED

// I also want to make very clear that i HATE the arduino IDE.


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <tinyxml.h>
#include <string>

#define RESPTIME 300
#define MIN_TIME 5 // Skip arrivals sooner than this (minutes)
#define AMOUNTOFSENSORS 1


int NUMBER_OF_STRING = 10;


const char *definedwachtwoord= "";

// authentication macros
const char *wemosNaam = "wall";
const char *server = "testServer";
const char* type = "lamp";

// Network SSID
const char *ssid = "oop";
const char *password = "programmer";
const char *ip = "dutchellie.nl";

// sensor globals
int previousSensorValue = 0;

WiFiClient client;

void setup()
{
  //setupsensor() - needed when using TWI sensor.
  
  Serial.begin(115200);
  Serial.print("Test Message");


  WiFi.begin(ssid, password); // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println(" ...");
  
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i);
    Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer
  
  // connect to server
  if (client.connect(ip, 4500)) {
    Serial.println("Connected to server");
  }else{
    Serial.println("Not connected");
  }
  Serial.println();
  // first we must authenticate with the server, if this can't happen we can't send any data.
  // authenticating() is not finished yet though
  while (authenticating()) {
       //delay(1000);
    Serial.print(++i);
    Serial.print(' ');
    }
  
}




void loop()
{
   // for the time being, the old sending function will be used. 
   // a string formatted like an xml file wil be send via said function.
  int sensor[AMOUNTOFSENSORS][3] = {1,0,0};    // sensor array will be {id,currentvalue,previousvalue}
  char* sensorNames[AMOUNTOFSENSORS][2] = {{"1","lamp"}};
   
   // readSensors --- placeholder for now, depends on the test setup.
   int sensorValue1 = analogRead(2);
   
   // format msg
   // i don't have time to add in the real message now, will come on friday i hope.
   TiXmlDocument AnswerMsg = buildAnwserMsg(sensor, sensorNames);


  // send msg
  for(int i = 0; i < AMOUNTOFSENSORS-1; i++){
     if (sensor[i][1] != sensor[i][2]) { // if (current sensorvalue != previous sensorvalue);
      TiXmlPrinter pronter;
      pronter.SetIndent("\t");
      AnswerMsg.Accept(&pronter);
     
      sendData(pronter.CStr());
    }
  }
  
  
 
}

bool authenticating(){
  // goes through the whole authentication procedure.
  // function is pretty long because of the long message strings.
  // !!!not finished yet!!!
  
  // first format the initial message.
  Serial.println("Starting authentication procedure");
  TiXmlDocument initialMsg = buildInitialMsg();
  TiXmlPrinter printer;
  printer.SetIndent("\t");
  initialMsg.Accept(&printer);
  
  // now send this to the server
  sendData(printer.CStr());

  // wait for some sort of reply, if received do the assignment thing.
  Serial.println("Waiting for a response.");
  int receivedResponse = 0;
  char *receivedMsg;
  do {receivedMsg = receiveData(&receivedResponse);}
  while (receivedResponse != 1);

   
  // now extract the usefull bits
  std::string parsedMsg[10];
  std::string receivedMsgString(receivedMsg);// turn the character stream
  parser(receivedMsgString ,parsedMsg);

  // if we receive the wrong clientName of msgtype something has probably gone wrong, so we try again.
  if (!(parsedMsg[2] == "OK")) {  
    return 0;
  }
    Serial.println("authentication procedure sucessfully completed");
return 1;
}


char* receiveData(int* receivedResponse) {
  // pretty rudementairy, if we can read a string we return it, if not we return 0.
  // TODO: add fancy error checking mechanism.
if (client.connect(ip, 8080)) {
    if(client.available()){
      char *poep;
      strcpy(poep, client.readString().c_str());
      *receivedResponse = 1;
      return poep;
    }
    else return 0;
  }
  else return 0;
}


void sendData(const char *msg){
  // don't know if this still works, problably does though
  Serial.println("starting a transmission of the following message:");
  Serial.println(msg);
  
    //if(client.availableForWrite()&&client.connected()){
          client.write(msg);
          Serial.println("The message has been send succesfully.");
    //}
}

//TiXmlDocument*//
TiXmlDocument Buildheader(TiXmlDocument I) { 
  //this function wil make the start of evry masage to the end of the header
  //deze functie maakt het begin van het bericht totenmet de einde van de header
  TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * message = new TiXmlElement( "message" );
    //header
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
    return I ;// sure if i need to return or if that wil just make a unecsacery copy
}

TiXmlDocument buildInitialMsg() {
  TiXmlDocument Msg;
  
  TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * message = new TiXmlElement( "message" );
    //header
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
  Msg.SaveFile( "anwserMsg.xml" );
  return Msg;
}

TiXmlDocument buildAnwserMsg(int sensor[AMOUNTOFSENSORS][3], char* sensorNames[AMOUNTOFSENSORS][2])
{ // builds an awnser message that's to be send to the server.
  TiXmlDocument anwserMsg;
  
  TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * message = new TiXmlElement( "message" );
    //header
      TiXmlElement * header = new TiXmlElement( "header" );
        TiXmlElement * senderElement = new TiXmlElement( "sender" );
          TiXmlText * senderText = new TiXmlText( wemosNaam );
        senderElement->LinkEndChild(senderText);
        TiXmlElement * receiverElement = new TiXmlElement( "receiver" );
          TiXmlText * receiverText = new TiXmlText( server );
        receiverElement->LinkEndChild(receiverText);
    message->LinkEndChild(header);
    //function
      TiXmlElement * functionElement = new TiXmlElement( "function" );
        TiXmlText * functionText = new TiXmlText( "answerToStatusRequest" );
      functionElement->LinkEndChild(functionText);
    message->LinkEndChild(functionElement);
    // context
      TiXmlElement * context = new TiXmlElement( "context" );
      
      // voor elke sensor een apart sensor element toevoegen
      for (int i = 0; i < AMOUNTOFSENSORS-1; i++) { 
      TiXmlElement * sensorElement = new TiXmlElement( "sensor" );
        TiXmlElement * nameElement = new TiXmlElement( "name" );
          TiXmlText * nameText = new TiXmlText( sensorNames[i][1] );
        nameElement->LinkEndChild(nameText);
      sensorElement->LinkEndChild(nameElement);
        TiXmlElement * statusElement = new TiXmlElement( "status" );
        char* strong;
        itoa(sensor[i][1], strong, 10);
          TiXmlText * statusText = new TiXmlText( strong ); //TODO: tostring convert
        statusElement->LinkEndChild(statusText);
      sensorElement->LinkEndChild(statusElement);
      context->LinkEndChild(sensorElement);
      }
      message->LinkEndChild(context);
  
  anwserMsg.LinkEndChild( message );
  anwserMsg.SaveFile( "anwserMsg.xml" );
  return anwserMsg;
}





void parser(std::string S1 ,std::string arr[]){
//    this parser result in the folowing
//    0 = status en errors
//    1=sender
//    2=function
//    3 and up = context

// dont ferget to define NUMBER_OF_STRING and wemosnaam

    int  SUB1 = S1.find("<message>");
    int  SUB2 = S1.find("</message>"); // look for the right format

    if(SUB1 == -1 || SUB2 == -1){
        arr[0]= "Verkeerde soort message";
    }
    else {

        SUB1 = S1.find("<receiver>");
        SUB2 = S1.find("</receiver>"); //search for reciever
        SUB1 +=11 ; //want <receiver> is 11 char big
        std::string naam = S1.substr(SUB1, SUB2 - SUB1);
        if ((naam == "allWemos" || wemosNaam == naam)){ // look if the mesage is for this wemos
            arr[0]= "Niet voor deze wemos bedoelt"; 
        }
        else {
            SUB1 = S1.find("<sender>"); // zoek sender
            SUB2 = S1.find("</sender>"); // zoek einde van sender
            SUB1 += 8 ; //want <sender> is 8 groot
            std::string sender = S1.substr(SUB1, SUB2 - SUB1); //defieneer sender SUB2 -  Sub1 want substr needs a int no a laction
            arr[1] = sender; 


            S1.erase(0,SUB2+9); // delete evrything we already searched
            SUB1 = S1.find("<function>"); 
            SUB2 = S1.find("</function>"); 
            SUB1 += 10 ; //want <function> is 10 groot
            std::string functie= S1.substr(SUB1, SUB2 - SUB1);
            arr[2] = functie;

            SUB1 = S1.find("<password>"); // zoek password
            SUB2 = S1.find("</password>"); // zoekunctie pasword einde
            SUB1 += 10; // want pasword is 10 groot
            std::string wachtwoord = S1.substr(SUB1, SUB2 - SUB1);
          //signaleerd
                    S1.erase(0,SUB1+11);
                    SUB1 = S1.find(wemosNaam); //find wemosNaam in bestand
                    if( SUB1 == -1){
                        arr[0]= "geen acties"; // geen actie uit te voeren want deze wemos komt niet voor in context
                    }
                    else{
                        SUB2 = S1.find("</wemos>");
                        S1.erase(SUB2,100000); // erase evrything after what this wemos needs
                        SUB1 = S1.find("<sensor>"); //find sensor
                        SUB2 = S1.find("</sensor>"); // find end sensor

                        for(int i = 3; SUB1==-1 || !(SUB2==-1|| i == NUMBER_OF_STRING); i++){// look for context
                            SUB1 = S1.find("<sensor>"); //zfind sensor
                            SUB2 = S1.find("</sensor>"); // find end sensor
                            SUB1 += 8 ;
                            std::string temp = S1.substr(SUB1, SUB2 - SUB1);
                            arr[i] = temp;
                        }
                }

        }
    }
}
