// This is testcode for now, since we need to make 7 different wemos codes eventually
// authentication problably works, but didn't have time to test yet
// based on the "wall" wemos from the excel file on blackboard 


// I also want to make very clear that i HATE the arduino IDE.



//alle loops hebben een delay nodig kan gewoon van 0 zijn maar anders crasht esp. 
// https://www.sigmdel.ca/michel/program/esp8266/arduino/watchdogs_en.html
//https://forum.arduino.cc/index.php?topic=622991.0


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <tinyxml.h>
#include <string>
#include <sstream> 
#include <Wire.h>

#define AMOUNTOFSENSORS 2
#define AMOUNTOFACTUATORS 1

#define BUFFERSIZE 20
#define WIBADRESD 0x38
#define WIBADRESA 0x36

// voor parser
int NUMBER_OF_STRING = 10;

// authentication macros
std::string wemosNaam = "bed1";
std::string server = "Server";
std::string wachtwoord = "solarwinds123";
std::string type = "bed";

// Network SSID
const char *ssid = "WatEenRotTaart";
const char *password = "KankerKanker";
const char *ip = "home.dutchellie.nl";

// sensor globals
// sensor pin number = de waarde van een 1 op de plek van het pin nummer in een byte.
unsigned int sensor[AMOUNTOFSENSORS][3] = {{0,0,1},{0,0,300}}; // sensor array will be {currentvalue,previousvalue, pinnumber} // please put this in te right order otherwise crash
std::string sensorNames[AMOUNTOFSENSORS][2] = {{"bool","pushButton"},{"int","forceSensor"}}; // each sensor has a name, but this can't be stored in an int array. {type,name}

unsigned int actuator[AMOUNTOFACTUATORS][3] = {{0,1,16}}; // actuator array will be {currentvalue, wantedvalue, pinnumber}
std::string actuatorNames[AMOUNTOFACTUATORS][2] = {{"bool","LED"}}; // each sensor has a name, but this can't be stored in an int array. {type,name} 
/* if we receive a message to change an actuatorvalue, put the received value in the wanted value entry of the array.
this way we don't have to worry about the different types of actuators, like twi of analog or binairy, etc when we handle the message*/

//function declarations xml
std::string buildcapabilities();
std::string Buildheader();
std::string buildStatusMsg();
std::string buildAuthenticationMsg();
void parser(std::string S1 ,std::string arr[]);
std::string intToString(int i);
bool handleMessage(std::string parsedMsg[BUFFERSIZE]);
void authenticating();



//function declaration wifi
void setupWifi();
const char* receiveData();
const char* receiveData(int*);
void sendData(const char *);

//function declaration 
void setupPins();
void setupSensors() ;
void setupActuators();
void updateActuators();
void readSensors();



std::string test = "<message> <header> <sender>server</sender> <receiver>wemosnaam</receiver> </header> <function>actuateBool</function> <context> <data1>0</data1> <!-- Vibration motor --> <data2>1</data2> <!-- LED --> </context> </message>";

WiFiClient client;



void setup() {

  
  Serial.begin(115200);
  Wire.begin();
  
  setupWifi();
  setupPins(); 

  // first we must authenticate with the server, if this can't happen we can't send any data.
  authenticating();

  Serial.println("Entering main program loop now.");
  delay(0);
}






void loop() {

  //----------sensors------------//
  //Serial.println("reading sensors now");


  readSensors();

  // if any of the sensors changed, we have to notify the server.
    //Serial.println("sending sensorupdate");
  int sendStatus = 0;
  for(int i = 0; i < AMOUNTOFSENSORS; i++){
     delay(0);
     if(sensorNames[i][1].compare("pushButton") == 0) {
      if (( sensor[i][0] == 1 )&&( sensor[i][1] == 1 ) {
        sendStatus = 1;
      }
     } else if(sensorNames[i][0].compare("bool") == 0 && sensorNames[i][1].compare("pushButton") != 0) {
       if (sensor[i][0] != sensor[i][1]) { // if (current sensorvalue != previous sensorvalue); logic could be different in different devices
         sendStatus = 1;
       }
     } else if ((sensor[i][0] >= (sensor[i][1] + 10) || sensor[i][0] <= (sensor[i][1] - 10)) && sensorNames[i][1].compare("pushButton") != 0) {
          sendStatus = 1;
        }
     sensor[i][1] = sensor[i][0]; // update the previous value
  }
  
  if (sendStatus){
    sendData(buildStatusMsg("sensorUpdate").c_str());
  }


  //-----------actuators-------------//
  // if we receive a message, handle it  
  //
  
  std::string receivedMsg(receiveData()); // receive some data, if there is nothing to receive, the string is "NULL"
  
  if (receivedMsg.compare("NULL") != 0){
    //Serial.println("The received message is not empty.");
    std::string parsedMsg[BUFFERSIZE];
    parser(receivedMsg, parsedMsg); // parse the message, 
    handleMessage(parsedMsg);
    //Serial.println("The received message has been parsed");
  }
  

  //Serial.println("updating actuators");
  updateActuators();


  //---------misc functions---------//
 delay(0); // delay can be 0, but still has to be present.
 
}
