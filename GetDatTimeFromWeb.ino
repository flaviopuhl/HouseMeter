#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid     = "CasaDoTheodoro";
const char *password = "09012011";
int Wifi_statusRecover=0;

const long utcOffsetInSeconds = -10800;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
    Wifi_statusRecover++;
      if (Wifi_statusRecover>=20){
        Serial.println ( "#" );
        ESP.restart();}
  }

  Wifi_statusRecover=0;
  timeClient.begin();
}

void loop() {
  timeClient.update();

String formattedDate = timeClient.getFormattedDate();

// Extract date
int splitT = formattedDate.indexOf("T");
String dayStamp = formattedDate.substring(0, splitT);
Serial.print(dayStamp);
Serial.print(" ");
// Extract time
String timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
Serial.print(timeStamp);
Serial.print(" ");

 if( WiFi.status() != WL_CONNECTED ) {
    Serial.print ( "Lost WiFi Connection" );
    Wifi_statusRecover++;
      if (Wifi_statusRecover>=5){
        Serial.println ( " Reseting" );
        ESP.restart();}
 }
  
  Serial.println("");
  delay(1000);
}
