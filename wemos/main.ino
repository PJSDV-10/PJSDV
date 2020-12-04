#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>

// Network SSID
const char *ssid = "Kanker Project";
const char *password = "KankerKanker";

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

void loop()
{
	//Add your project's loop code here
}
