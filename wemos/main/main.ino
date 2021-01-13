// This is testcode for now, since we need to make 7 different wemos codes eventually
// authentication problably works, but didn't have time to test yet
// based on the "wall" wemos from the excel file on blackboard 


// I also want to make very clear that i HATE the arduino IDE.


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <tinyxml.h>
#include <string>

#define AMOUNTOFSENSORS 1
#define AMOUNTOFACTUATORS 1
#define BUFFERSIZE 20

// voor parser
int NUMBER_OF_STRING = 10;

// authentication macros
const char *wemosNaam = "wall";
const char *server = "testServer";
const char* type = "lamp";

// Network SSID
const char *ssid = "oop";
const char *password = "programmer";
const char *ip = "dutchellie.nl";

// sensor globals
int sensor[AMOUNTOFSENSORS][3] = {{0,0,16}}; // sensor array will be {currentvalue,previousvalue, pinnumber}
char* sensorNames[AMOUNTOFSENSORS][2] = {{"sensorBool","drukKnop"}}; // each sensor has a name, but this can't be stored in an int array. {type,name}

int actuator[AMOUNTOFACTUATORS][3] = {{12,12,15}}; // actuator array will be {currentvalue, wantedvalue, pinnumber}
char* actuatorNames[AMOUNTOFACTUATORS][2] = {"actuatorInt","lamp"}; // each sensor has a name, but this can't be stored in an int array. {type,name} 
/* if we receive a message to change an actuatorvalue, put the received value in the wanted value entry of the array.
this way we don't have to worry about the different types of actuators, like twi of analog or binairy, etc when we handle the message*/


//function declarations xml
char* buildStatusMsg(char*);
char* buildAuthenticationMsg();
void parser(std::string S1 ,std::string arr[]);


//function declaration wifi
void setupWifi();
char* receiveData();
char* receiveData(int* receivedResponse);
void sendData(const char *msg);


WiFiClient client;

void setup()
{

  
  Serial.begin(115200);
  Serial.print("Test Message");

  setupWifi();
  setupPins(); //needed when using TWI sensor. 
  // first we must authenticate with the server, if this can't happen we can't send any data.
  // authenticating() is not finished yet though
  ///*
  int i = 0;
  while (authenticating()) {
       delay(1000);
    Serial.print(++i);
    Serial.print(' ');
    }
  //*/
}

void loop()
{ 

   // placeholder: readSensors is different for every device
   for (int i = 0; i < AMOUNTOFSENSORS; i++) {
    sensor[i][0] = digitalRead(sensor[i][2]);
   }
   
  // if we receive a message, handle it  
  std::string receivedMsg(receiveData()), empty; // receive some data, if there is nothing to receive, the string is empty
  if (!(receivedMsg == empty)){
    std::string parsedMsg[BUFFERSIZE];
    parser(receivedMsg, parsedMsg); // parse the message, 
    handleMessage(parsedMsg);
  }
   

  updateActuators();
 ///*
  // send msg
  int sendStatus = 0;
  for(int i = 0; i < AMOUNTOFSENSORS; i++){
     if (sensor[i][0] != sensor[i][1]) { // if (current sensorvalue != previous sensorvalue); logic could be different in different devices
      sendStatus = 1;
     }
     sensor[i][1] = sensor[i][0]; // update the previous value
  }
  
  if (sendStatus){
     
    sendData(buildStatusMsg(""));
    
  }
  
//*/
}

bool handleMessage(std::string parsedMsg[BUFFERSIZE]) {
  
  if(parsedMsg[1] == "getStatusBroadcast") { // can't do switch statements with strings so giant if else it's gonna have to be.

      sendData(buildStatusMsg("answerToStatusRequest"));
      return 1;
  } else  
  return 0;

}

void updateActuators() {
    for(int i = 0; i < AMOUNTOFACTUATORS; i++){
    if (actuator[i][1] != actuator[i][0]) { // if the wanted value != current value we have to change the current value
      
      Serial.print("we changed the ");
        Serial.print(actuatorNames[i][1]);
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

bool authenticating(){
  // goes through the whole authentication procedure.
  // function is pretty long because of the long message strings.
  // !!!not finished yet!!!
  
  // first format the initial message.
  Serial.println("Starting authentication procedure");

  
  // now send this to the server
  sendData(buildAuthenticationMsg());
  

  // wait for some sort of reply, if received do the assignment thing.
  Serial.println("Waiting for a response.");
  int receivedResponse = 0;
  char *receivedMsg;
  do {receivedMsg = receiveData(&receivedResponse); if (receivedResponse != 1){delay(100);}}
  while (receivedResponse != 1);

   
  // now extract the usefull bits
  std::string parsedMsg[10];
  std::string receivedMsgString(receivedMsg);// turn the character stream into a std::string
  parser(receivedMsgString ,parsedMsg);

  // if we receive the wrong clientName of msgtype something has probably gone wrong, so we try again.
  if (!(parsedMsg[2] == "OK")) {  
    return 0;
  }
    Serial.println("authentication procedure sucessfully completed");
return 1;
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
    if(sensorNames[i][1] == "drukKnop") {
      pinMode(sensor[i][2],INPUT_PULLUP);
    }else if (sensorNames[i][1] == "TWISensor") {
      // do the twi thing
      
    }else
      pinMode(sensor[i][2],INPUT);
  }
}

void setupActuators() {
  // sets the actuator's pins in output mode
  for (int i = 0; i < AMOUNTOFACTUATORS; i++) {
    pinMode(actuator[i][2],OUTPUT);
  }
}
