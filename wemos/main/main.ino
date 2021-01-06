#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include<string>







#define KEEPALIVETIME 5000
#define RESPTIME 300
std::string uniekewemosnaam = "hallo";
std::string definedwachtwoord = "hallo";
#define NUMBER_OF_STRING 10

//vergeet niet NUMBER_OF_STRING te define
// vergeet niet uniekewemosnaam te define
// vergeet niet definedwachtwoord te definen

// Network SSID
const char *ssid = "Kanker Project";
const char *password = "KankerKanker";
void parser(std::string ,std::string arr[]);

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





void parser(std::string S1 ,std::string arr[]){
//    in deze parser word het als volgt neergezet
//    0 = status en errors
//    1=sender
//    2=function
//    3 en up = context

//vergeet niet NUMBER_OF_STRING te define
// vergeet niet uniekewemosnaam te define
// vergeet niet definedwachtwoord te definen

    int  SUB1 = S1.find("<message>");
    int  SUB2 = S1.find("</message>"); // zoek of dit we lecht een mesage is

    if(SUB1 == -1 || SUB2 == -1){
        arr[0]= "Verkeerde soort message";
    }
    else {

        SUB1 = S1.find("<receiver>");
        SUB2 = S1.find("</receiver>"); //zoek naa reciever
        SUB1 +=11 ; //want <receiver> is elf groot
        std::string naam = S1.substr(SUB1, SUB2 - SUB1);
        if ((naam == "allWemos" || uniekewemosnaam == naam)){
            arr[0]= "Niet voor deze wemos bedoelt"; //kijken ov hij voor deze wemos bedoel is
        }
        else {
            SUB1 = S1.find("<sender>"); // zoek sender
            SUB2 = S1.find("</sender>"); // zoek einde van sender
            SUB1 += 8 ; //want <sender> is 8 groot
            std::string sender = S1.substr(SUB1, SUB2 - SUB1); //defieneer sender SUB2 -  Sub1 want substr heeft een getal nodig met hoevel die verder gaat
            arr[1] = sender; // zet sender definitief


            S1.erase(0,SUB2+9); // delete alles van de string waar we al overheen gelopen zijn
            SUB1 = S1.find("<function>"); // zoek functie
            SUB2 = S1.find("</function>"); // zoekunctie einde
            SUB1 += 10 ; //want <function> is 10 groot
            std::string functie= S1.substr(SUB1, SUB2 - SUB1);
            arr[2] = functie;

            SUB1 = S1.find("<password>"); // zoek pasword
            SUB2 = S1.find("</password>"); // zoekunctie pasword einde
            SUB1 += 10; // want pasword is 10 groot
            std::string wachtwoord = S1.substr(SUB1, SUB2 - SUB1);
            if (!(wachtwoord==definedwachtwoord)){ // check of het wachtwoord goed is
                    arr[0]= "Verkeerde Wachtwoord";
                }
                else{
                    S1.erase(0,SUB1+11);
                    SUB1 = S1.find(uniekewemosnaam); //zoek naar wemosnaam in overgebleen bestand
                    if( SUB1 == -1){
                        arr[0]= "geen acties"; // geen actie uit te voeren want deze wemos komt niet voor in context
                    }
                    else{
                        SUB2 = S1.find("</wemos>");
                        S1.erase(SUB2,100000); // erase alles na de actie die deze wemos moet uitvoeren
                        SUB1 = S1.find("<sensor>"); //zoek sensor
                        SUB2 = S1.find("</sensor>"); // zoek einde sensor

                        for(int i = 3; SUB1==-1 || !(SUB2==-1|| i == NUMBER_OF_STRING); i++){
                            SUB1 = S1.find("<sensor>"); //zoek sensor
                            SUB2 = S1.find("</sensor>"); // zoek einde sensor
                            SUB1 += 8 ;
                            std::string temp = S1.substr(SUB1, SUB2 - SUB1);
                            arr[i] = temp;
                        }





                }
            }
        }
    }
}
