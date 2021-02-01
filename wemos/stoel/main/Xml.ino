//TiXmlDocument*//
std::string Buildheader() {  //build header with the gloals specified
    std::string temp ="<message><header><sender>"+ wemosNaam +"</sender><receiver>"+ server +"</receiver></header>"; 
    return temp;
}

std::string buildAuthenticationMsg() {
   //dammit mad this entire thing only to notice im using the wrong file.............................
   //std::string temp = Buildheader() +"<function>authentication</function><context><password>"+wachtwoord+"</password><clientName>"+wemosNaam+"</clientName><type>"+type+"</type>"+ buildcapabilities()+" </context> </message>";
   std::string temp = Buildheader() + "<function>authentication</function><context><password>" +wachtwoord+ "</password><clientName>" + wemosNaam + "</clientName><type>" + type + "</type></context></message>" ;
   return temp;
}

std::string buildStatusMsg(std::string function){
  std::string temp = Buildheader();
  temp += "<function>"+ function + "</function><context><password>" +wachtwoord+ "</password><type>" + type + "</type>";
  
    for (int i = 0;i<AMOUNTOFSENSORS;i++) { //voeg elke keer neeiwe sensot toe
      delay(0);
      
       std::string roundd = intToString(i + 1); // int too string
       std::string worth = intToString(sensor[i][0]);
       
       if (sensorNames[i][1].compare("forceSensor") == 0) {
        if (sensor[i][0] > 100) {
          worth = "1";
        } else
          worth = "0";
       }
       
       if (sensorNames[i][1].compare("pushButton") == 0 && sensor[1][0] == 1) {
        if (knopAan) {
          worth = "0";
          knopAan = 0;
        } else {
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
    //Serial.println("Received a awnserToStatusRequest msg");
    client.write(buildStatusMsg("answerToStatusRequest").c_str());
    //Serial.println("send a reply to the broadcast request\n\r");
    return 1;
    
  } else if (parsedMsg[2].compare("actuateBool") == 0) {
    //Serial.println("Received a actuateBool msg");
    //Serial.println(sizeof(parsedMsg)/sizeof(parsedMsg[0]));
    for (int i = 3; i < (3 + AMOUNTOFACTUATORS); i++) {
      //Serial.print("the string in the parsed msg has this point of data as: ");
      //Serial.print(parsedMsg[i].c_str());
      //Serial.print(" and after atoi: ");
      //Serial.println(atoi(parsedMsg[i].c_str()));
      actuator[i-3][1] = atoi(parsedMsg[i].c_str());
      delay(0);
    }
    return 1;
    
  } else //if (parsedMsg[1].compare("keepalive") == 0) {
    
  //}
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




/*std::string buildcapabilities() {
  std::string temp = "<capabilities>";

  for(int i = 0;i<AMOUNTOFSENSORS;i++){ //voeg elke keer neeiwe sensot toe
    temp += "<func><type>" + sensorNames[i][0] + "</type><funcName>" + sensorNames[i][1] + "</funcName></func>";
  }
  
   for(int i = 0;i<AMOUNTOFACTUATORS;i++){ //voeg elke keer neeiwe actuator toe
    temp += "<func><type>" + actuatorNames[i][0] + "</type><funcName>" + actuatorNames[i][1] + "</funcName></func>";
  }
  
  temp+= "</capabilities>";
  return temp;
} dam this isnt even necesary anymore*/ 
