/* Auhtor          : JAISON JOSE
 * Device          : ESP8266
 * Connected with  : SD card module 
 *           CS    : D8 - GPIO15
 *           MOSI  : D7 - GPIO13
 *           SCK   : D5 - GPIO14
 *           MISO  : D6 - GPIO12
 *           Vcc   : 3.3V
 * Serial Buad Rate: 9600          
 */           
#include<ESP8266WiFi.h>
#include<Arduino.h>
#include<SD.h>
#include<ESP8266HTTPClient.h>
#include<ArduinoJson.h>                                        // version : 5.12

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
File myFile;
int id, i;
String test;

void setup()                                                   //<-- To be executed once -->
{
Serial.begin(9600);                                            //Baud rate = 9600
WiFi.begin(ssid, password);                                    //Connecting to the WIFI 
delay(3000);                                                   // Waits for 3 seconds
if (WiFi.status() == WL_CONNECTED)                             //Only if WIFI is connected
 {
    HTTPClient http;                                           // Creating object og HTTPClient
    http.begin("http://demo4657392.mockable.io/list-tag-ids"); //API URL                               
    if (http.available())                                      //If http is available
    {
     myFile = SD.open("apInfo.txt", FILE_WRITE);               //Creates a new file if not available - "apInfo.txt"
     if (myFile)                                               //If myFile is created then.. 
     {
      SD.remove("apInfo.txt");                                 //Removing the older file so as to repack the new data
      myFile = SD.open("apInfo.txt", FILE_WRITE);
     }
     if (myFile) 
        {myFile.println(http.GET();}                           // Writing all the API file
      if (myFile)
        {myFile.close();}                                      //File close
     } 
    http.end();                                                //Close connection
  }
}


void loop()                                                    //<-- Executed continously after SETUP()--> 
{
while(Serial.available() == 0)                                // Continous only if the Serial gets STRING
{}
test = Serial.readString();   
myFile = SD.open("apInfo.txt");
if (myFile) 
   {id = 0;
    i = 1;
    DynamicJsonBuffer jb;                                      // Creating dynamic JSON buffer
    JsonObject& root = jb.parseObject(myFile);                 // Parsing the API- JSON file
    while (myFile.available()) 
         {if (root["student_id"][i] == test)                   // Checking on with the CODE and the STRING form serial com port
            {id = i;                                           // Storing the id
             i++; 
            }           
         }
   }
if (myFile)
  {
    myFile.close();
  }
Serial.println(id);                                               //Printing ID
Serial.println("Time for query:" + String(millis()) + "milliSec");// Printing the query time     
}
