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

//function declarations xmlwrite
TiXmlDocument buildAnwserMsg();
TiXmlDocument buildInitialMsg();
void sendData(const char *msg);
bool handleMessage(std::string parsedMsg[BUFFERSIZE]);

//function declaration xmlread
char* receiveData();
void parser(std::string S1 ,std::string arr[]);
char* receiveData(int* receivedResponse);
bool authenticating();

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
  
TiXmlDocument AnswerMsg = buildAnwserMsg();

  // send msg
  for(int i = 0; i < AMOUNTOFSENSORS; i++){
     if (sensor[i][1] != sensor[i][2]) { // if (current sensorvalue != previous sensorvalue); logic could be different in different devices
      TiXmlPrinter pronter;
      pronter.SetIndent("\t");
      AnswerMsg.Accept(&pronter);
     
      sendData(pronter.CStr());
    }
  }
  
  
 
}
