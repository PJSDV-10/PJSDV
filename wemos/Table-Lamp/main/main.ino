/*
 * authors: PJSDV group 10
 * Version: 2.0
 * 
 * 
 * This is code for the table-lamp wemos
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
 * the rgb led is connected to pin d5, and is written to using changeLedColour(int colour). 
 *  this doesn't actually change the colour, but the brightness.
 * 
 * sensors:
 *  bool PIR sensor on pin DI0
 *  
 * actuators
 *  rgb led on pin D5
 */


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <string>
#include <sstream> 
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define AMOUNTOFSENSORS 1
#define AMOUNTOFACTUATORS 1

#define BUFFERSIZE 20
#define WIBADRESD 0x38
#define WIBADRESA 0x36

//--- rgb led defines ---//
#define LED_PIN 14


//--- voor parser ---//
int NUMBER_OF_STRING = 10;

// authentication macros
std::string wemosNaam = "lamp1";
std::string server = "Server";
std::string wachtwoord = "solarwinds123";
std::string type = "tablelamp";

// Network SSID

const char *ssid = "PJSDV-10"; // ssid of the wifi
const char *password = "PJSDV-10"; // password of the wifi
const char *ip = "192.168.43.201"; // ip adress of the server


// sensor globals
// sensor pin number = de waarde van een 1 op de plek van het pin nummer in een byte. A0/1 + 300, D5 = 500;
unsigned int sensor[AMOUNTOFSENSORS][3] = {{0,0,1}}; // sensor array will be {currentvalue,previousvalue, pinnumber} // please put this in te right order otherwise crash
std::string sensorNames[AMOUNTOFSENSORS][2] = {{"bool","PIR"}}; // each sensor has a name, but this can't be stored in an int array. {type,name}

unsigned int actuator[AMOUNTOFACTUATORS][3] = {{0,0,500}}; // actuator array will be {currentvalue, wantedvalue, pinnumber}
std::string actuatorNames[AMOUNTOFACTUATORS][2] = {{"bool","RGB"}}; // each sensor has a name, but this can't be stored in an int array. {type,name} 
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
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {

  
  Serial.begin(115200);
  Wire.begin();


  // setup our wifi, connect to the server and connect to the i2c wemos interface board.
  setupWifi();
  setupPins(); 

  //setup the rgb led librar
  setupRGB();

  // first we must authenticate with the server, if this can't happen we can't send any data.
  authenticating();

  
  strip.clear();
  Serial.println("Entering main program loop now.");
  delay(0);
}






void loop() {
  //----------sensors------------//
  //Serial.println("reading sensors now");
  readSensors();
  Serial.println(sensor[0][0]);

  // if any of the sensors changed, we have to notify the server.
    //Serial.println("sending sensorupdate");
    
     delay(0);
     
     if ((sensor[0][0] == 1) && (sensor[0][1] == 0) || (sensor[0][0] == 0) && (sensor[0][1] == 1)) {
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


void setupRGB(){
  strip.begin();
  strip.show();
}

void setLedColour(int R, int G, int B) {
  // sets the led to the colour we 

  strip.setPixelColor(0, strip.Color(R,G,B));
  strip.show();
}
