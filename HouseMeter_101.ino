#include <EEPROM.h>

// Grava valor inicial do leitor na EEPROM
unsigned long val_meter_recons_old = 0;     // Valor reconstruido da eeprom
unsigned long val_meter_recons_new = 0;
char val_meter_str[9];
int i=0;
const int reed = 14;          // Sensor reed
bool debug=false;

void setup() {
  
//Inicializa funções
delay(500);
Serial.begin(115200);
EEPROM.begin(512);
}



void loop() {

  delay(3000);

//#################################################
// Subrotina de leitura da EEPROM
// e conversão para decimal
//#################################################
   
    for(i=0;i<=9;i++){
      val_meter_str[i]=EEPROM.read(i);
      }
    
    val_meter_recons_old=atol(val_meter_str);
  
      if(debug){
        Serial.println("[Lendo EEPROM]"); 
        for(i=0;i<=9;i++){
            Serial.print("     Endereço: ");
            Serial.print(i);
            Serial.print("  Valor: ");
            Serial.println(val_meter_str[i]);
            } 
          Serial.print("Valor reconstruido: ");    
          Serial.print(val_meter_recons_old);
          Serial.println("d");
          Serial.println();
      
          Serial.println("[DONE]");
          Serial.println();
          }

//#################################################
// FIM subrotina leitura EEPROM
//#################################################
  
  val_meter_recons_new = val_meter_recons_old + 1;

//#################################################
// Subrotina de escrita da EEPROM
// e conversão para string
//#################################################

   dtostrf(val_meter_recons_new, 8, 0, val_meter_str);  //Converte float para string

    for(i=0;i<=9;i++){
      EEPROM.write(i, val_meter_str[i]);
      }
       EEPROM.commit();
       
    if(debug){
      Serial.println("[Escrevendo EEPROM]");
      Serial.print("Valor: ");    
      Serial.print(val_meter_recons_new);
      Serial.println("d");
    
      Serial.print("Valor: ");    
      Serial.print(val_meter_str);
      Serial.println("c");
      Serial.println();
    
      for(i=0;i<=9;i++){
        Serial.print("     Endereço: ");
        Serial.println(i);
        }
         Serial.println("[DONE]");
         Serial.println();
        }
    
//#################################################
// FIM subrotina escrita EEPROM
//#################################################
  

  Serial.println("---------------------------------");
    Serial.print("Valor inicial:");
      Serial.println(val_meter_recons_old);
    Serial.print("Valor final:  ");
      Serial.println(val_meter_recons_new);
  Serial.println("---------------------------------");
}
