/*
 * Progetto Orto Arduino Yùn ( Internet of Greens )
 *
 * Autore: Alfieri Mauro
 * Twitter: @mauroalfieri
 *
 */

#include <FileIO.h>
#include <HttpClient.h>
#include <ArduinoJson.h>
#include "ortoConfig.h"

void setup() {
  Serial.begin(9600);
  
  Bridge.begin();
  FileSystem.begin();
  
  pinMode(pinLamp1,OUTPUT);
  pinMode(pinLamp2,OUTPUT);
  pinMode(pinPump1,OUTPUT);
  pinMode(pinPump2,OUTPUT);
  pinMode(pinFert1,OUTPUT);
  
  pinMode(pinValF1,INPUT);
  
  for(byte isens = 0; isens < nsens ; isens++){
    pinMode(pinEC1[isens],OUTPUT);
    pinMode(pinEC2[isens],INPUT);
    pinMode(pinEC3[isens],OUTPUT);
  }
  analogWrite(pinFert1,0);
}

void loop () {
  currMin  = getInterval("+%M");
  currHour = getInterval("+%H");
  
  Serial.println("\n---------------");
  Serial.println( getTime() );

  if ( (currMin == 0 || currMin == 30) && currMin != old ) {
    old = currMin;
    
    /**********************************************************************************/

    if ( getApiSmartCitizen( SmartCitizenApiKey ) ) {
    
      #ifdef DEBUG  
        Serial.print( "Temp: " ); Serial.print(temp);  Serial.println(" ");
        Serial.print( "Hum:  " ); Serial.print(hum);   Serial.println(" %");
        Serial.print( "Co: "   ); Serial.print(co);    Serial.println(" Co");
        Serial.print( "No2: " );  Serial.print(no2);   Serial.println(" No");
        Serial.print( "Nois: " ); Serial.print(noise); Serial.println("");
        Serial.print( "Luce: " ); Serial.print(light); Serial.println("");
        Serial.print( "Batt: " ); Serial.print(batt);  Serial.println("v");
      #endif  
      
    }
    
    /**********************************************************************************/
     
    getPh();
     
    /**********************************************************************************/
  
     digitalWrite(pinEC1[1],HIGH);
     if ( analogRead(pinEC2[1]) > 512 ) { level = 0; }
     else                               { level = 1; } 
     digitalWrite(pinEC1[1],LOW);
     
     /**********************************************************************************/
    
  } // end if 0/30 min

  /**********************************************************************************/
  
  if ( ((20 <= currHour && currHour <= 23)  || (5 <= currHour && currHour <= 7)) && currMin == 20 && currMin != old ) {
    old = currMin;
    CS1 = readEC(0);
  }
  
  /**********************************************************************************/
  
  // FERTILIZZANTE
  if ( currHour == 22 && currMin == 30 && statusFert == false ) {
    apriGocciolatore();
    fertil = 1;
    delay( 1300 ); // con 1300 millisecondi fa 10ml
    chiudiGocciolatore();
    statusFert = true;
  } else { 
    fertil = 0;
  }
  if ( currHour == 22 && currMin == 35  && statusFert == true) { statusFert = false; }

  /**********************************************************************************/
  
  if ( 7 <= currHour && currHour <= 20 && statusLight == false ) { // Luci accese
     digitalWrite(pinLamp1,HIGH);
     digitalWrite(pinLamp2,HIGH);
     
     statusLight = true;
  }
   
  if ( ((21 <= currHour && currHour <= 23)  || (0 <= currHour && currHour <= 6)) && statusLight == true ) { // Luci accese
    digitalWrite(pinLamp1,LOW);
    digitalWrite(pinLamp2,LOW);
    
    statusLight = false;
  }
   
  /**********************************************************************************/
  
  if (( 8 <= currHour && currHour <= 21 ) && (0 <= currMin && currMin <= 15)) {
      if ( !statusPump ) {
        digitalWrite(pinPump1,HIGH); delay( 100 );
        digitalWrite(pinPump2,HIGH); delay( 100 );
        
        statusPump = true;
      }
  } else {
   if ( statusPump ) {
       digitalWrite(pinPump1,LOW); delay( 100 );
       digitalWrite(pinPump2,LOW); delay( 100 );
       
       statusPump = false;
     }
  }
   
  /**********************************************************************************/
  
  if ( currMin != old ) {
    String dataString;
    dataString += getTime();
  
    dataString += " "; dataString += String( temp );
    dataString += " "; dataString += String( hum );
    dataString += " "; dataString += String( co );
    dataString += " "; dataString += String( no2 );
    dataString += " "; dataString += String( noise );
    dataString += " "; dataString += String( light );
    dataString += " "; dataString += String( batt );
    
    dataString += " "; dataString += String( pHValue );
    dataString += " "; dataString += String( level );
    dataString += " "; dataString += String( CS1 );
    dataString += " "; dataString += String( fertil );
    dataString += " "; dataString += String( statusLight );
    dataString += " "; dataString += String( statusPump );
    
    File dataFile = FileSystem.open(logFile, FILE_APPEND);
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    }
  }
  
  /**********************************************************************************/

  delay(60000);
  
} // end loop

