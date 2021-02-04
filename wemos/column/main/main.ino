/*
 * authors: PJSDV group 10
 * Version: 2.0
 * 
 * This is code for the column wemos
 * 
 * It works like this:
 * 
 * setup {
 *  setupwifi
 *  setup the I2C
 *  authenticate with server
 * }
 *  
 * loop {
 *  read sensors
 *  if a sensor has changed
 *    send senorupdate to server
 *  check if we received a message
 *  if we received a message:
 *    parse the message
 *    handle the request
 *  update the actuator's status.
 * }
 * 
 * sensors are connected to a Wemos Interface Board: WIB
 * we read them via I2C using the WIRE.h library
 * actuators are connected to the same board, and are written to in the same way.
 * 
 * 
 * The connected sensors:
 *  Push Button on pin DI0
 *  gas sensor on pin AI0
 * 
 * the connected actuators:
 *  buzzer on pin DO4
 *  LED om pin DO5
 */




#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <string>
#include <sstream> 
#include <Wire.h> // I2C

#define AMOUNTOFSENSORS 2
#define AMOUNTOFACTUATORS 2

#define BUFFERSIZE 20
#define WIBADRESD 0x38
#define WIBADRESA 0x36

// voor parser
int NUMBER_OF_STRING = 10;

// authentication macros
std::string wemosNaam = "column1";
std::string server = "Server";
std::string wachtwoord = "solarwinds123";
std::string type = "column";

// Network SSID
const char *ssid = "PJSDV-10";
const char *password = "PJSDV-10";
const char *ip = "192.168.43.201";

// sensor globals
// sensor pin number = de waarde van een 1 op de plek van het pin nummer in een byte.
unsigned int sensor[AMOUNTOFSENSORS][3] = {{0,0,1},{0,0,300}}; // sensor array will be {currentvalue,previousvalue, pinnumber} // please put this in te right order otherwise crash
std::string sensorNames[AMOUNTOFSENSORS][2] = {{"bool","pushButton"},{"int","gassensor"}}; // each sensor has a name, but this can't be stored in an int array. {type,name}


unsigned int actuator[AMOUNTOFACTUATORS][3] = {{0,0,16},{0,0,32}}; // actuator array will be {currentvalue, wantedvalue, pinnumber}
std::string actuatorNames[AMOUNTOFACTUATORS][2] = {{"bool","BUZZER"},{"bool","LED"}}; // each sensor has a name, but this can't be stored in an int array. {type,name} 
/* if we receive a message to change an actuatorvalue, put the received value in the wanted value entry of the array.
this way we don't have to worry about the different types of actuators, like twi of analog or binairy, etc when we handle the message*/


bool knopAan = 0; // global bool to turn a static button into a switch.

//function declarations xml
std::string buildcapabilities();
std::string Buildheader();
std::string buildStatusMsg(std::string, bool);
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

//function declaration  sensors
void setupPins();
void setupSensors() ;
void setupActuators();
void updateActuators();
void readSensors();



WiFiClient client;



void setup() {

  
  Serial.begin(115200); // start a serial session for debugging
  Wire.begin(); // start a wire session for i2c communication.


  // setup our wifi, connect to the server and connect to the i2c wemos interface board.
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

  Serial.print(sensor[0][0]);
  Serial.print("  ");
  Serial.println(sensor[1][0]);

  // if any of the sensors changed, we have to notify the server.
    //Serial.println("sending sensorupdate");
     
     if ((((sensor[1][0] > 200) && (sensor[1][1] < 200)) || ((sensor[1][0] < 200) && (sensor[1][1] > 200))) || ((sensor[0][0] == 1) && (sensor[0][1] == 0))) {
       // if ((force sensor has just turned of or off) or the pushbutton has just turned on);
       sendData(buildStatusMsg("sensorUpdate", knopAan).c_str());
     }
     
  for(int i = 0; i < AMOUNTOFSENSORS; i++){  
     sensor[i][1] = sensor[i][0]; // update the previous value
  }


  //-----------actuators-------------//
  // if we receive a message, handle it  
  //


  std::string receivedMsg = "NULL";
  
  if (client.peek() != -1) {
  receivedMsg = receiveData(); // receive some data, if there is nothing to receive, the string is "NULL"
  Serial.println("received data");
  }
  

  
    //std::string receivedMsg = receiveData(); // receive some data, if there is nothing to receive, the string is "NULL"
    
    if (receivedMsg.compare("NULL") != 0){
      
      std::string parsedMsg[BUFFERSIZE];
      parser(receivedMsg, parsedMsg); // parse the message, 
      handleMessage(parsedMsg);
      
    }
  
  

  updateActuators();


  //---------misc functions---------//
 delay(0); // delay can be 0, but still has to be present.
 
}
