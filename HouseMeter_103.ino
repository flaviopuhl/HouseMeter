/************************************************************************
* PROGRAM :        HouseMeter_103.c             
* 
* DESCRIPTION :
*       Monitor de consumo de energia
*
* NOTES :
*       Suporte EEPROM.............[101]
*       Suporte Interrupção........[102]
*       Suporte WiFi/OTA...........[103]
*       Suporte NTP................[103]
*       Suporte Sleep mode.........[TBD]
*       Suporte Battery meas.......[TBD]
*       Suporte Thinger............[TBD]
*        
* HARDWARE:       
*       reed - D2
*       
* AUTHOR :    Flávio Puhl        START DATE :    SET19
*/


/************************************************************************
* Libraries
************************************************************************/
#include <EEPROM.h>
#include <ESP8266WiFi.h>                    //  WiFi
#include <ThingerESP8266.h>                 //  THINGER.IO 
#include <ESP8266mDNS.h>                    //  OTA
#include <WiFiUdp.h>                        //  OTA
#include <ArduinoOTA.h>                     //  OTA
#include <NTPClient.h>                      //  NPT

/************************************************************************
* Define pinout and constants
************************************************************************/ 
unsigned long val_meter_recons_old = 0;     // Valor reconstruido da eeprom
unsigned long val_meter_recons_new = 0;
char val_meter_str[9];                      // String auxiliar
int i=0;                                    // contador auxiliar
const int reed = 4;                         // Sensor reed
bool debug=false;                            // flag de debug
int redeRSSI = 0;                           // Variável de armazenamento de RSSI
void ICACHE_RAM_ATTR ISRoutine ();          // interrupção
String msgbox;
const long utcOffsetInSeconds = -10800;
String dayStamp;
String timeStamp;
String formattedDate;
int splitT;

/************************************************************************
* Thinger.io connection parameters 
************************************************************************/ 
#define user "fpuhlx"
#define device_Id "Nodex"
#define device_credentials "Hu$tZIOuj56rx"
ThingerESP8266 thing(user, device_Id, device_credentials);


/************************************************************************
* WiFi connection parameters and constants
************************************************************************/ 
const char WiFi_ssid[]="CasaDoTheodoro";      //WiFi SSID
const char WiFi_password[]="09012011";        //WiFi password

/************************************************************************
* Define NTP Client to get time
************************************************************************/  
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


void setup() {

/************************************************************************
* Setup
************************************************************************/
Serial.begin(115200);
EEPROM.begin(512);
timeClient.begin();

/************************************************************************
* Setup WiFi
************************************************************************/ 
    WiFi.mode(WIFI_STA);
    thing.add_wifi(WiFi_ssid, WiFi_password);
      while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      delay(3000);
      ESP.restart();}

/************************************************************************
* OTA Functions - do not remove
************************************************************************/ 
    ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

/************************************************************************
* Setup interrupção
************************************************************************/
pinMode(reed,INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(reed),ISRoutine,FALLING);

} //END void setup()



void loop() {
  
/************************************************************************
* OTA & Thinger handles - do not remove
************************************************************************/ 
  ArduinoOTA.handle();
  //thing.handle();

/************************************************************************
* Serial debug
************************************************************************/ 
  if(debug==true){    

    /************************************************************************
    * NTP
    ************************************************************************/
    timeClient.update();
    
    formattedDate = timeClient.getFormattedDate();
    
    // Extract date
    splitT = formattedDate.indexOf("T");
    dayStamp = formattedDate.substring(0, splitT);
    // Extract time
    timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);

      msgbox = "\n---------------------------------";
      msgbox += "\nValor inicial: [ ";
      msgbox += val_meter_recons_old;
      msgbox += " ]";
      msgbox += "\nValor final:   [ ";
      msgbox += val_meter_recons_new;
      msgbox += " ]";
      msgbox += "\nRSSI:          [ ";
      msgbox += WiFi.RSSI();
      msgbox += " ]";
      msgbox += "\nLast Update:   [ ";
      msgbox += dayStamp;
      msgbox += " ";
      msgbox += timeStamp;
      msgbox += " ]";
      msgbox += "\nVersao:        [ ";
      msgbox += "103";
      msgbox += " ]";
      msgbox += "\n---------------------------------";
      
      Serial.println(msgbox);
      
      debug=false;
  }

  
  }//END void loop()


/************************************************************************
* Interrupção
************************************************************************/

void ISRoutine (){

/************************************************************************
* Subrotina de leitura da EEPROM e conversão para decimal
************************************************************************/ 
    for(i=0;i<=9;i++)
      {
        val_meter_str[i]=EEPROM.read(i);
      }
    
  val_meter_recons_old=atol(val_meter_str);
  
  val_meter_recons_new = val_meter_recons_old + 1;      //Incrementa contador

/************************************************************************
* Subrotina de escrita da EEPROM e conversão para string
************************************************************************/
dtostrf(val_meter_recons_new, 8, 0, val_meter_str);  //Converte float para string

    for(i=0;i<=9;i++)
      {
       EEPROM.write(i, val_meter_str[i]);
      }
       EEPROM.commit();

  debug=true; 
  
}//END void ISRoutine()
