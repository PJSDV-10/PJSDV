

const char* receiveData() {
  // pretty rudementairy, if we can read a string we return it, if not we return 0.
  // TODO: add fancy error checking mechanism.
  //Serial.println("looking if we received a message");
  if(client.available()){
    delay(0);

    std::string plas(client.readString().c_str());
      


    Serial.println("We received the following message");
    Serial.println(plas.c_str());
    Serial.println("");
    
    if (plas.compare("") != 0) {
      return plas.c_str();
    } else {
      Serial.println("client is availible, but we received no message");
      Serial.println("");
      return "NULL";
    }
    
  } else{
    //Serial.println("client isn't availible");
    return "NULL";
  }
}

const char* receiveData(int* receivedResponse) {
  // pretty rudementairy, if we can read a string we return it, if not we return 0.
  // TODO: add fancy error checking mechanism.
    if(client.available()){
    

      std::string plas(client.readString().c_str());
      *receivedResponse = 1;


          Serial.println("We received the following message");
          Serial.println(plas.c_str());
          Serial.println("");

          return plas.c_str();
    } else
    return "NULL";
}


void sendData(const char * msg){
  // sends the enclosed message to the connected client
  Serial.println("starting a transmission of the following message:");
  Serial.println(msg);
  
    if(client.availableForWrite()&&client.connected()){
          client.write(msg);
          Serial.println("The message has been send succesfully.");
          Serial.println("");
    }
}

void setupWifi(){
  WiFi.begin(ssid, password); // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("...");
  
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i);
    Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Wifi connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer
  
  // connect to server
  if (client.connect(ip, 8080)) {
    Serial.println("Connected to server");
  }else{
    Serial.println("Not connected to server");
  }
  
}


void parser(std::string S1 ,std::string arr[]){
//    this parser result in the folowing
//    0 = status en errors
//    1=sender
//    2=function
//    3 and up = data

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
                    //HIER GAAT HET FOUT:
                        
                        SUB1 = S1.find("<data1>"); //find sensor
                        SUB2 = S1.find("</data1>"); // find end sensor
                        Serial.println("parsing the data now: ");
                        for(int i = 3; SUB1==-1 || !(SUB2==-1|| i == NUMBER_OF_STRING); i++){// look for context
                            delay(1);
                            
                            std::string roundd = intToString(i - 2);
                            
                            SUB1 = S1.find("<data"+roundd+">"); //zfind sensor
                            SUB2 = S1.find("</data"+roundd+">"); // find end sensor
                            SUB1 += 7 ;
                            std::string temp = S1.substr(SUB1, SUB2 - SUB1);
                            arr[i] = temp;
                            //Serial.print("data: ");
                            //Serial.println(arr[i].c_str());
                        }
                        Serial.println("Finished parsing the data.");
                
        }
    }
}
