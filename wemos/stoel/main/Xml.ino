//TiXmlDocument*//
std::string Buildheader() {  //build an xnml header with the gloals specified, and return it as a std::string.
    std::string temp ="<message><header><sender>"+ wemosNaam +"</sender><receiver>"+ server +"</receiver></header>"; 
    return temp;
}

std::string buildAuthenticationMsg() {
   // builds a message that authenticates the wemos at the server, and returns it as std::string
   
   std::string temp = Buildheader() + "<function>authentication</function><context><password>" +wachtwoord+ "</password><clientName>" + wemosNaam + "</clientName><type>" + type + "</type></context></message>" ;
   return temp;
}

std::string buildStatusMsg(std::string function, bool knop){
  // composes a string that has all the sensor data formatted in xml.
  
  std::string temp = Buildheader(); // message starts off with a header
  temp += "<function>"+ function + "</function><context><password>" +wachtwoord+ "</password><type>" + type + "</type>";
  
    for (int i = 0;i<AMOUNTOFSENSORS;i++) { //voeg elke keer nieuwe sensor toe
      delay(0);
      
       std::string roundd = intToString(i + 1); // this will be the number x in <datax> in the xml message
       std::string worth = intToString(sensor[i][0]); // this varable will contain the data y in <datax>y</datax>
       
       if (sensorNames[i][1].compare("forceSensor") == 0) { // if the sensor is a force sensor:
        if (sensor[i][0] > 200) { // if the sensor is higher than 200, someone is problably sitting on the chair. We send a '1', otherwise we send '0'.
          worth = "1";
        } else {
          worth = "0";
          knopAan = "0";
        }
       }
       
       if (sensorNames[i][1].compare("pushButton") == 0 && sensor[i][0] == 1) { // the pushbutton has to function as a switch, so we toggle a bool variable "knopAan". 
        
        if (knop) { // if the switch is currently on, and the button was pressed: turn the switch off.
          worth = "0";
          knopAan = 0;
        } else { // if the switch is off, and the button is pressed: turn it on and send a one as data.
          worth = "1";
          knopAan = 1;
        }
       }
       
      temp +=  "<data"+roundd+">"+ worth +"</data"+roundd+">";
      delay(0);
  }
  
  delay(0);
  temp +=   "</context></message>";
  
  return temp;
}

std::string intToString(int i){ //conver int to string
   std::stringstream str;
   str << i;
   std::string s;
   s += str.str();
   return s;
}


bool handleMessage(std::string parsedMsg[BUFFERSIZE]) {
  
  if(parsedMsg[2].compare("getStatusBroadcast") == 0) { // can't do switch statements with strings so giant if else it's gonna have to be.
    Serial.println("Received an answerToStatusRequest msg");
    client.write(answerBuildStatusMsg("answerToStatusRequest").c_str()); //stuur hier antwoord naar site actuators
    Serial.println("send a reply to the broadcast request\n\r");
    return 1;
    
  } else if (parsedMsg[2].compare("actuateBool") == 0) {
    Serial.println("we received an actuateBool message");
    for (int i = 3; i < (3 + AMOUNTOFACTUATORS); i++) {

      // we want to change the actuator's wanted value to the one in the message:
      actuator[i-3][1] = atoi(parsedMsg[i].c_str()); //atoi(): sting to int
      delay(0);
    }
    return 1;
    
  } else 
    // if we don't understand the message, return a 0.
  Serial.println("couldn't handle the message\n\r");
  return 0;
}



void authenticating(){
  // goes through the whole authentication procedure.
  Serial.println("Starting authentication procedure");

  
  // build the message, and send it to the server

  sendData(buildAuthenticationMsg().c_str());

  // wait for some sort of reply, if received do the assignment thing.
  Serial.println("Waiting for a response.");
  
  int receivedResponse = 0;
  const char* receivedMsg = "";
  do {receivedMsg = receiveData(&receivedResponse);
  delay(1);
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




std::string answerBuildStatusMsg(std::string function){
  // composes a string that has all the sensor data formatted in xml.
  
  std::string temp = Buildheader(); // message starts off with a header
  temp += "<function>"+ function + "</function><context><password>" +wachtwoord+ "</password><type>" + type + "</type>";
      
      temp +=  "<data1>" + intToString(sensor[0][0])+ " </data1>"+ "<data2>" +intToString(sensor[1][0])+ " </data2>";
     delay(0);
     temp +=   "</context></message>";
     return temp;
  }
