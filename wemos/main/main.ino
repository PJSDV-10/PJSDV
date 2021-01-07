// This is testcode for now, since we need to make 7 different wemos codes eventually
// authentication problably works, but didn't have time to test yet
// based on the "wall" wemos from the excel file on blackboard 
// connections:
// A0 - LDR     A1 - potmeter       DO4 - LCDPanel     D5 - RGBLED

// I also want to make very clear that i HATE the arduino IDE.


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <TinyXML.h>


#define KEEPALIVETIME 5000
#define RESPTIME 300
#define MIN_TIME      5 // Skip arrivals sooner than this (minutes)
#define SENSOR_PIN 

// authentication macros
const char *wemosNaam = "wall"
const char *server = "testServer"

// Network SSID
const char *ssid = "WatEenRotTaart";
const char *password = "KankerKanker";
const char *ip = "40.68.29.170";

// sensor globals
int previousSensorValue = 0;

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
   int sensorValue1 = analogread(2);
   
   // format msg
   // i don't have time to add in the real message now, will come on friday i hope.
   TiXmlDocument AnswerMsg = buildAnwserMsg;


  // send msg
  if (sensorValue1 != previousSensorValue) {
    sendData(AnswerMsg);
  }
  
	
 
}



bool authenticating(){
  // goes through the whole authentication procedure.
  // function is pretty long because of the long message strings.
  // !!!not finished yet!!!
  
  // first format the initial message.
  TiXmlDocument initialMsg = buildInitialMsg();

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
  TiXmlDocument confirmMsg = buildConfirmMsg();
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

void buildAnwserMsg(int sensorValue;)
{ // builds an awnser message that's to be send to the server.
  TiXmlDocument anwserMsg;
  
  TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * message = new TiXmlElement( "message" );
    //header
      TiXmlElement * header = new TiXmlElement( "header" );
        TiXmlElement * senderElement = new TiXmlElement( "sender" );
          TiXmlText * senderText = new TiXmlText( wemosNaam );
        senderElement->LinkEndChild(senderText);
        TiXmlElement * receiverElement = new TiXmlElement( "receiver" );
          TiXmlText * receiverText = new TiXmlText( server );
        receiverElement->LinkEndChild(receiverText);
    message->LinkEndChild(header);
    //function
      TiXmlElement * functionElement = new TiXmlElement( "function" );
        TiXmlText * functionText = new TiXmlText( "answerToStatusRequest" );
      functionElement->LinkEndChild(functionText);
    message->LinkEndChild(functionElement);
    // context
      TiXmlElement * context = new TiXmlElement( "context" );
      TiXmlElement * sensor1Element = new TiXmlElement( "sensor" );
        TiXmlElement * name1Element = new TiXmlElement( "name" );
          TiXmlText * name1Text = new TiXmlText( lamp );
        name1Element->LinkEndChild(name1Text);
      sensor1Element->LinkEndChild(name1Element);
        TiXmlElement * status1Element = new TiXmlElement( "status" );
          TiXmlText * status1Text = new TiXmlText( sensorValue ); //TODO: tostring convert
        status1Element->LinkEndChild(status1Text);
      sensor1Element->LinkEndChild(status1Element);
          
      message->LinkEndChild(context);
  
  anwserMsg.LinkEndChild( message );
  anwserMsg.SaveFile( "anwserMsg.xml" );
}
