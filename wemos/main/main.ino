// This is testcode for now, since we need to make 7 different wemos codes eventually
// authentication problably works, but didn't have time to test yet
// based on the "wall" wemos from the excel file on blackboard 


// I also want to make very clear that i HATE the arduino IDE.



//allen loops hebben een delay nodig kan gewoon van 0 zijn maar anders crasht esp. hier meer over watchdogs
// https://www.sigmdel.ca/michel/program/esp8266/arduino/watchdogs_en.html
//https://forum.arduino.cc/index.php?topic=622991.0


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <tinyxml.h>
#include <string>
#include <sstream> 

#define AMOUNTOFSENSORS 2
#define AMOUNTOFACTUATORS 1
#define BUFFERSIZE 20

// voor parser
int NUMBER_OF_STRING = 10;

// authentication macros
std::string wemosNaam = "stoel2";
std::string server = "testServer";
std::string wachtwoord = "jemoeder";
std::string type = "stoel";

// Network SSID
const char *ssid = "oop";
const char *password = "programmer";
const char *ip = "home.dutchellie.nl";

// sensor globals
int sensor[AMOUNTOFSENSORS][3] = {{0,0,16},{0,0,12}}; // sensor array will be {currentvalue,previousvalue, pinnumber} // please put this in te right order otherwise crash
std::string sensorNames[AMOUNTOFSENSORS][2] = {{"sensorBool","drukKnop"},{"sensorBool","naam"}}; // each sensor has a name, but this can't be stored in an int array. {type,name}

int actuator[AMOUNTOFACTUATORS][3] = {{12,12,15}}; // actuator array will be {currentvalue, wantedvalue, pinnumber}
std::string actuatorNames[AMOUNTOFACTUATORS][2] = {"actuatorInt","lamp"}; // each sensor has a name, but this can't be stored in an int array. {type,name} 
/* if we receive a message to change an actuatorvalue, put the received value in the wanted value entry of the array.
this way we don't have to worry about the different types of actuators, like twi of analog or binairy, etc when we handle the message*/


//function declarations xml
std::string buildcapabilities();
std::string Buildheader();
std::string buildStatusMsg();
std::string buildAuthenticationMsg();
void parser(std::string, std::string);
std::string intToString(int i);


//function declaration wifi
void setupWifi();

const char* receiveData();

const char* receiveData(int*);
void sendData(const char *);


WiFiClient client;

void setup()
{

  
  Serial.begin(115200);
  Serial.print("Test Message");

  setupWifi();
  /*
  pinMode(0, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);//https://www.sigmdel.ca/michel/program/esp8266/arduino/watchdogs_en.html
  */
  setupPins(); 
  
  // first we must authenticate with the server, if this can't happen we can't send any data.
  authenticating();

  Serial.println("Entering main program loop now.");
  delay(0);
}

void loop()
{
  // placeholder: readSensors is different for every device
  Serial.println("reading sensors now");
  for (int i = 0; i < AMOUNTOFSENSORS; i++) {
    if (sensorNames[i][0].compare("sensorBool") == 0) {
      sensor[i][0] = digitalRead(sensor[i][2]);
      delay(0);
    } else if (sensorNames[i][0].compare("sensorInt") == 0) {
      sensor[i][0] = analogRead(sensor[i][2]);
    }
  }
  
   
  // if we receive a message, handle it  
  Serial.println("checking if we received a msg");
  
  std::string receivedMsg(receiveData()); // receive some data, if there is nothing to receive, the string is empty / this is broken dont use
  delay(0);

  
  if (receivedMsg.compare("NULL") != 0){
    std::string parsedMsg[BUFFERSIZE];
    parser(receivedMsg, parsedMsg); // parse the message, 
    handleMessage(parsedMsg);
  }
  

  Serial.println("updating actuators");
  updateActuators();

  // send msg
    Serial.println("sending sensorupdate");
  int sendStatus = 0;
  for(int i = 0; i < AMOUNTOFSENSORS; i++){
     delay(1);
     if (sensor[i][0] != sensor[i][1]) { // if (current sensorvalue != previous sensorvalue); logic could be different in different devices
      sendStatus = 1;
     }
     sensor[i][1] = sensor[i][0]; // update the previous value
  }
  
  if (sendStatus){
    sendData(buildStatusMsg());
  }
  delay(1000);
}

bool handleMessage(std::string parsedMsg[BUFFERSIZE]) {
  
  if(parsedMsg[1].compare("getStatusBroadcast") == 0) { // can't do switch statements with strings so giant if else it's gonna have to be.

      sendData(buildStatusMsg());
      return 1;
  } else  
  return 0;

}

void updateActuators() {
    for(int i = 0; i < AMOUNTOFACTUATORS; i++){
        delay(1);
    if (actuator[i][1] != actuator[i][0]) { // if the wanted value != current value we have to change the current value
      
      Serial.print("we changed the ");
        Serial.print(actuatorNames[i][1].c_str());
        Serial.print("'s value to: ");
        Serial.print(actuator[i][1]);
        
      // change the current value 
      if (actuatorNames[i][0] == "actuateBool"){ // if the actuator is boolean, write a bool to the pin.
        digitalWrite(actuator[i][2],actuator[i][1]);
        Serial.println(" using digitalWrite.");
        
      } else if(actuatorNames[i][0] == "actuateInt"){ // if the actuator is analogue, write using PWM.
        if(actuator[i][1] >= 255) // failsafe to catch values higher then 255.
          actuator[i][1] = 255;
        analogWrite(actuator[i][2],actuator[i][1]); // write the wanted value to the current value using analogwrite
        Serial.println(" using analogWrite.");
      }
      
      actuator[i][0] = actuator[i][1]; //update the currentvalue
    }
  }
}

void authenticating(){
  // goes through the whole authentication procedure.
  Serial.println("Starting authentication procedure");

  
  // build the message, and send it to the server

  sendData(buildAuthenticationMsg());

  // wait for some sort of reply, if received do the assignment thing.
  Serial.println("Waiting for a response.");
  
  int receivedResponse = 0;
  const char* receivedMsg = "";
  do {receivedMsg = receiveData(&receivedResponse);
  delay(0);
  }
  while(receivedResponse==0);

  // now extract the usefull bits
  std::string parsedMsg[10];
  std::string receivedMsgString(receivedMsg);// turn the character stream into a std::string
  parser(receivedMsgString ,parsedMsg);


  // if we receive the wrong clientName of msgtype something has probably gone wrong, so we try again.
  if (parsedMsg[2].compare("ack") == 0) {  
    Serial.println("authentication procedure sucessfully completed");

  }else 
    Serial.println("authentication procedure not completed, trying again.");
  
}


void setupPins() {
  // sets the wemos' pins in output, input or input_pullup mode depending on actuator/sensor type.
  setupSensors();
  setupActuators();
  Serial.println("pin setup complete");
}
void setupSensors() {
  // puts the pins in input or input_pullup mode depending on the type of sensor
  // TODO: also enables the needed TWI if we are using a TWI sensor.
  for (int i = 0; i < AMOUNTOFSENSORS; i++) {
      delay(1);
    if(sensorNames[i][1].compare("drukKnop") == 0) {
      pinMode(sensor[i][2],INPUT_PULLUP);
      Serial.println("input_pullup");
    }else {
      pinMode(sensor[i][2],INPUT);
      Serial.println("input");
    }
  }
}

void setupActuators() {
  // sets the actuator's pins in output mode
  for (int i = 0; i < AMOUNTOFACTUATORS; i++) {
    delay(1);
    pinMode(actuator[i][2],OUTPUT);
    Serial.println("output");
  }
}
