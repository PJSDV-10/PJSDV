//#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <XMLWriter.h>

#define KEEPALIVETIME 5000
#define RESPTIME 300
#define MIN_TIME      5 // Skip arrivals sooner than this (minutes)
#define SENSOR_PIN 
#define STREAM
#define BUFFERSIZE

// Network SSID
const char *ssid = "WatEenRotTaart";
const char *password = "KankerKanker";
const char *ip = "40.68.29.170";
int receivedResponse = 1;



void setup()
{
  //setupsensor() - deeded if TWI sensor.
  
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
}

WiFiClient client;

void loop()
{
   // as far as we know, we can't create .xml files without using an sd card module.
   // so for the time being, the old sending function will be used. 
   // a string formatted like an xml file wil be send via said function.

   
   // readSensors --- placeholder for now, depends on the test setup.
   int sensValue = analogread(2);
   
   // format msg
   char msg[] = // this is a placeholder message.
   "<message>\n\r\t<header>\n\r\t\t<sender>sendername</sender>\n\r\t</header>\n\r</message>";


  // send msg


  
	if (client.connect(ip, 8080))
	{
		Serial.println("Connected to server");
		while(client.available() || client.connected()){
			if(client.connected() && receivedResponse == 1){
				delay(KEEPALIVETIME);
				if(client.availableForWrite()){
					client.write(msg);
					receivedResponse = 0;
				}
				
			}
			if(client.available() && receivedResponse == 0){
				Serial.printf("Response:\n\r%s\r\n", client.readString().c_str());
				receivedResponse = 1;
			}
		}
	}
 
}
