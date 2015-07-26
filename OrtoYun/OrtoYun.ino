/*
 * Progetto Orto Arduino Yùn ( Internet of Greens )
 *
 * Autore: Alfieri Mauro
 * Twitter: @mauroalfieri
 *
 */
#include <FileIO.h>
#include <HttpClient.h>
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
 
  for(byte isens = 0; isens < nsens ; isens++){
    pinMode(pinEC1[isens],OUTPUT);
    pinMode(pinEC2[isens],INPUT);
    pinMode(pinEC3[isens],OUTPUT);
  }
 
  digitalWrite(pinFert1,0);
}

void loop () {
  currMin  = getInterval("+%M");
  currHour = getInterval("+%H");
  
  /**********************************************************************************/
  
  #ifdef DEBUG  
    Serial.println("\n------------------");
    Serial.println( getTime() );
  #endif
  
  /**********************************************************************************/
  if ( (currMin == 0 || currMin == 30) && currMin != lastMin ) {
    lastMin = currMin;
    
    /**********************************************************************************/
    
    getApiSmartCitizen();
    
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
  
  if ( ((20 <= currHour && currHour <= 23)  || (5 <= currHour && currHour <= 7)) && currMin == 20 && currMin != lastMin ) {
    lastMin = currMin;
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
  if ( currHour == 22 && currMin == 31  && statusFert == true) { statusFert = false; }

  /**********************************************************************************/
  
  if ( 7 <= currHour && currHour <= 20 && statusLight1 == false ) { // Luci accese
     digitalWrite(pinLamp1,HIGH);
     statusLight1 = true;
  }
  
  if ( ((21 <= currHour && currHour <= 23)  || (0 <= currHour && currHour <= 6)) && statusLight1 == true ) { // Luci accese
    digitalWrite(pinLamp1,LOW);
    statusLight1 = false;
  }
  
  /**********************************************************************************/
  
  if ( 7 <= currHour && currHour <= 20 && statusLight2 == false ) { // Luci accese
     digitalWrite(pinLamp2,HIGH);
     statusLight2 = true;
  }
   
  if ( ((21 <= currHour && currHour <= 23)  || (0 <= currHour && currHour <= 6)) && statusLight2 == true ) { // Luci accese
    digitalWrite(pinLamp2,LOW);
    statusLight2 = false;
  }
  
  /**********************************************************************************/
  
  if (( 8 <= currHour && currHour <= 21 ) && (0 <= currMin && currMin <= 15)) {
      if ( !statusPump1 ) {
        digitalWrite(pinPump1,HIGH); delay( 100 );
        statusPump1 = true;
      }
  } else {
     if ( statusPump1 ) {
         digitalWrite(pinPump1,LOW); delay( 100 );
         statusPump1 = false;
     }
  } 

  if ( 6 <= currHour && currHour <= 23 ) {
      if ( !statusPump2 ) {
        digitalWrite(pinPump2,HIGH); delay( 100 );
        statusPump2 = true;
      }
  } else {
     if ( statusPump2 ) {
         digitalWrite(pinPump2,LOW); delay( 100 );
         statusPump2 = false;
     }
  }
   
  /**********************************************************************************/
  
  #ifdef DEBUG
    Serial.print  ( "Controllo curr: " );
    Serial.print  ( currMin );
    Serial.print  ( " last: " );
    Serial.print( lastMin );
    Serial.print  ( " write: " );
    Serial.println( writeMin );
  #endif
  
  /**********************************************************************************/
  
  if ( currMin != writeMin ) {
    writeMin = currMin;
    
    getApiSmartCitizen();
    
    String dataString;
    dataString += getTime();
/*  
    dataString += " "; dataString += String( temp );
    dataString += " "; dataString += String( hum );
    dataString += " "; dataString += String( co );
    dataString += " "; dataString += String( no2 );
    dataString += " "; dataString += String( noise );
    dataString += " "; dataString += String( light );
    dataString += " "; dataString += String( batt );
*/
    dataString += " "; dataString += String( results );
    dataString += " "; dataString += String( pHValue );
    dataString += " "; dataString += String( level );
    dataString += " "; dataString += String( CS1 );
    dataString += " "; dataString += String( fertil );
    dataString += " "; dataString += String( statusLight1 );
    dataString += " "; dataString += String( statusLight2 );
    dataString += " "; dataString += String( statusPump1 );
    dataString += " "; dataString += String( statusPump2 );
    
    File dataFile = FileSystem.open(logFile, FILE_APPEND);
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    }
  }
  
  /**********************************************************************************/
  delay(30000);
  
} // end loop
