#include <EEPROM.h>

// Grava valor inicial do leitor na EEPROM
unsigned long val_meter_dec = 987685432;    //Valor do monitor de consumo
unsigned long val_meter_recons = 0;
char val_meter_str[9];                   //String
int i;


void setup() {
  
// initialize serial and wait for port to open:
delay(500);
Serial.begin(115200);
EEPROM.begin(512);

  Serial.print("Valor inicial: ");    
  Serial.print(val_meter_dec);
  Serial.println(" [DEC]");
  Serial.println();


//dtostrf(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, charBuf);
dtostrf(val_meter_dec, 8, 0, val_meter_str);  //Converte float para string
  Serial.print("Valor inicial: ");    
  Serial.print(val_meter_str);
  Serial.println(" [CHAR]");
  Serial.println();

//################################################
//GRAVAR OS DADOS NA EEPROM
//################################################
//EEPROM.write(addr, val);
//Grava cada caracter nos endereços da memória
Serial.println("Gravando EEPROM");
for(i=0;i<=9;i++){
  Serial.print("     Endereço: ");
  Serial.println(i);
  EEPROM.write(i, val_meter_str[i]);
}
   EEPROM.commit();
   Serial.println("DONE");
   Serial.println();
//################################################
//################################################   



//################################################
//LER OS DADOS NA EEPROM
//################################################
Serial.println("Lendo EEPROM");
for(i=0;i<=9;i++){
  val_meter_str[i]=EEPROM.read(i);
  Serial.print("     Endereço: ");
  Serial.print(i);
  Serial.print("  Valor ");
  Serial.println(val_meter_str[i]);
}
   
   Serial.println("DONE");
   Serial.println();
//################################################
//################################################


val_meter_recons=atol(val_meter_str);

  Serial.print("Valor reconstruido: ");    
  Serial.print(val_meter_recons);
  Serial.println(" [DEC]");
  Serial.println();
}



void loop() {

  delay(500);
}
