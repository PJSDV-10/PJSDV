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

#define AMOUNTOFSENSORS 2
#define BUFFERSIZE 20


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
int sensor[AMOUNTOFSENSORS][4] = {{1,255,0,2},{2,17,0,4}};    // sensor array will be {id,currentvalue,previousvalue, pinnumber}
  char* sensorNames[AMOUNTOFSENSORS][2] = {{"1","drukKnop"},{"2","ldr"}}; // each sensor has a name, but this can't be stored in an int array. {id,name}

//function declarations xml
TiXmlDocument buildStatusMsg();
TiXmlDocument buildAuthenticationMsg();
void parser(std::string S1 ,std::string arr[]);

//function declaration wifi
void setupWifi();
char* receiveData();

char* receiveData(int* receivedResponse);
void sendData(const char *msg);


WiFiClient client;

void setup()
{
  //setupsensor() - needed when using TWI sensor.
  
  Serial.begin(115200);
  Serial.print("Test Message");

  setupWifi();
  
  // first we must authenticate with the server, if this can't happen we can't send any data.
  // authenticating() is not finished yet though
  int i = 0;
  while (authenticating()) {
       delay(1000);
    Serial.print(++i);
    Serial.print(' ');
    }
  
}




void loop()
{
   // for the time being, the old sending function will be used. 
   // a string formatted like an xml file wil be send via said function.
  
   
   // readSensors 
   for (int i = 0; i < AMOUNTOFSENSORS; i++) {
    sensor[i][1] = analogRead(sensor[i][3]);
   }
   
  // if we receive a message, handle it
   
  std::string receivedMsg(receiveData()); // receive some data
  if (receivedMsg != ""){
    std::string parsedMsg[BUFFERSIZE];
    parser(receivedMsg, parsedMsg);
    handleMessage(parsedMsg);
    
  }
  
  TiXmlDocument statusMsg = buildStatusMsg("");

  // send msg
  for(int i = 0; i < AMOUNTOFSENSORS; i++){
     if (sensor[i][1] != sensor[i][2]) { // if (current sensorvalue != previous sensorvalue); logic could be different in different devices
      TiXmlPrinter pronter;
      pronter.SetIndent("\t");
      statusMsg.Accept(&pronter);
     
      sendData(pronter.CStr());
    }
  }
  
  
 
}

bool handleMessage(std::string parsedMsg[BUFFERSIZE]) {
  
  if(parsedMsg[2] == "getStatusBroadcast") { // can't do switch statements with strings so giant if else it's gonna have to be.
    TiXmlDocument AnswerMsg = buildStatusMsg("answerToStatusRequest");
    TiXmlPrinter pranter;
      pranter.SetIndent("\t");
      AnswerMsg.Accept(&pranter);
      sendData(pranter.CStr());
      return 1;
  } else  
  return 0;

}

bool authenticating(){
  // goes through the whole authentication procedure.
  // function is pretty long because of the long message strings.
  // !!!not finished yet!!!
  
  // first format the initial message.
  Serial.println("Starting authentication procedure");
  TiXmlDocument initialMsg = buildAuthenticationMsg();
  
  TiXmlPrinter printer;
  printer.SetIndent("\t");
  initialMsg.Accept(&printer);
  
  // now send this to the server
  sendData(printer.CStr());

  // wait for some sort of reply, if received do the assignment thing.
  Serial.println("Waiting for a response.");
  int receivedResponse = 0;
  char *receivedMsg;
  do {receivedMsg = receiveData(&receivedResponse); if (receivedResponse != 1){delay(100);}}
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