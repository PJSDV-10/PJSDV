// This is testcode for now, since we need to make 7 different wemos codes eventually
// authentication problably works, but didn't have time to test yet
// based on the "wall" wemos from the excel file on blackboard 
// connections:
// A0 - LDR     A1 - potmeter       DO4 - LCDPanel     D5 - RGBLED

// I also want to make very clear that i HATE the arduino IDE.


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <XMLWriter.h>

#define KEEPALIVETIME 5000
#define RESPTIME 300
#define MIN_TIME      5 // Skip arrivals sooner than this (minutes)
#define SENSOR_PIN 

// authentication macros
const char *wemosNaam = "wall"
const char *server = "testServer"
const char *clientName = "wallWemos"
const int amountOfPins = 4;

// Network SSID
const char *ssid = "WatEenRotTaart";
const char *password = "KankerKanker";
const char *ip = "40.68.29.170";

WiFiClient client;

void setup()
{
  //setupsensor() - needed when using TWI sensor.
  
	Serial.begin(115200);
	Serial.write("Test Message");

  XMLWriter xml;

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
	if (client.connect(ip, 8080)) {
    Serial.println("Connected to server");
	}
	
	// first we must authenticate with the server, if this can't happen we can't send any data.
	// authenticating() is not finished yet though
	while (authenticating()) {
    }
	
}




void loop()
{
   // for the time being, the old sending function will be used. 
   // a string formatted like an xml file wil be send via said function.

   
   // readSensors --- placeholder for now, depends on the test setup.
   int sensValue = analogread(2);
   
   // format msg
   // i don't have time to add in the real message now, will come on friday i hope.
   char *msg = "<message>\n\r\t<header>\n\r\t\t<sender>sendername</sender>\n\r\t</header>\n\r</message>";


  // send msg
  sendData(msg);
	
 
}



bool authenticating(){
  // goes through the whole authentication procedure.
  // function is pretty long because of the long message strings.
  // !!!not finished yet!!!
  
  // first format the initial message.
  char initialMsg[] = 
  "<message>
  <header>
  <sender>wemosNaam</sender>
  <receiver>server</receiver>
    </header>
    <function>authentication</function>
    <context>
        <password>password</password>
        <clientName>clientName</clientName>
        <AOP>amountOfPins</AOP>
        <capabilities>
            <func>
                <type>actuateBool</type>
                <funcName>lamp</funcName>
            </func>
            <func>
                <type>buttonPress</type>
                <funcName>lampKnop</funcName>
            </func>
        </capabilities>
    </context>
</message>";

  // now send this to the server
  sendData(initialMsg);

  // wait for some sort of reply, if received do the assignment thing.
  do {char *receivedMsg = receiveData();}
  while (receivedResponse != 1);

  // TODO: now extract the usefull bits
  // DEFINITELY NOT FINISHED YET!!!!
  
  char *deviceName = "clientname"; // hardcoded them in so it goes through.
  char *msgType = "OK";

  // if we receive the wrong clientName of msgtype something has probably gone wrong, so we try again.
  if (!(deviceName == clientName && msgType == "OK")) {  
    return 0;
  }

  // if nothing has gone wrong, send a confirmation
  char *confirmMsg = 
  "<header>
  <sender>wemosNaam</sender>
  <receiver>server</receiver>
    </header>
    <function>authentication</function>
    <context>
        <password>password</password>
        <clientName>clientName</clientName>
        <capabilities>
            <func>
                <type>OK</type>
                <funcName>OK</funcName>
            </func>
        </capabilities>
    </context>
</message>";
    // send the confirm message
  sendData(confirmMsg);

return 1;
}


char* receiveData() {
  // pretty rudementairy, if we can read a string we return it, if not we return 0.
  // TODO: add fancy error checking mechanism.
if (client.connect(ip, 8080)) {
    if(client.available() && receivedResponse == 0){
      return client.readString().c_str();
    }
    else return 0;
  }
  else return 0;
}


void sendData(char *msg){
  // don't know if this still works, problably does though

  
    if(client.available()){
      if(client.connected() && receivedResponse == 1){
        delay(KEEPALIVETIME);
        if(client.availableForWrite()){
          client.write(msg);
		  return 1;
        }
      } else return 0;
    }
	else return 0;
}
