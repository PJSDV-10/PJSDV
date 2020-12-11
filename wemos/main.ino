#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define KEEPALIVETIME 5000
#define RESPTIME 300

// Network SSID
const char *ssid = "WatEenRotTaart";
const char *password = "KankerKanker";
const char *ip = "40.68.29.170";
int receivedResponse = 1;

void setup()
{
	Serial.begin(115200);
	Serial.write("Test Message");

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
	if (client.connect(ip, 8080))
	{
		Serial.println("Connected to server");
		while(client.available() || client.connected()){
			if(client.connected() && receivedResponse == 1){
				delay(KEEPALIVETIME);
				if(client.availableForWrite()){
					client.write("Function:KA\r\n");
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
