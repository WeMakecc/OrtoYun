/**********************************************************************************/

boolean getApiSmartCitizen(String APIKEY) {
  int index = 0;
  boolean startData = false;
  char results[textBuffSize];
  
  String url  = "http://api.smartcitizen.me/v0.0.1/";
         url += APIKEY;
         url  = "/lastpost.json";
  
  HttpClient client;
  client.get(url); // get SmartCitizen Data

  #ifdef DEBUG
      Serial.print("SmartCitizen: ");
      Serial.println("connected");
  #endif
  
  while (client.available()) {
       char c = client.read();
       if( c == '{'  ){ if( startData==false){ startData=true; } }
       if(startData==true){ results[index] = c; index++; }
  }
  results[index]=0;
  
  #ifdef DEBUG
    Serial.print("SmartCitizen: ");
    Serial.println("disconnected");
  #endif
    
  startData=false;
  index = 0;
  
  StaticJsonBuffer<550> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(results);
  if (!root.success()) {
    Serial.println("parseObject: failed");
    return false;
  }
    
  temp  = root["devices"][0]["posts"]["temp"];
  hum   = root["devices"][0]["posts"]["hum"];
  co    = root["devices"][0]["posts"]["co"];
  no2   = root["devices"][0]["posts"]["no2"];
  noise = root["devices"][0]["posts"]["noise"];
  light = root["devices"][0]["posts"]["light"];
  batt  = root["devices"][0]["posts"]["bat"];
  
  return true;
  
}

/**********************************************************************************/

String getTime() {
  String result;
  Process time;
  time.begin("date");
  time.addParameter("+%d/%m/%Y %T");
  time.run();
  while(time.available()>0) {
    char c = time.read();
    if(c != '\n') result += c;
  }
  
  return result;
}

/**********************************************************************************/

byte getInterval( char* string ) {
  String result;
  Process time;
  time.begin("date");
  time.addParameter(string);
  time.run();
  while(time.available()>0) {
    char c = time.read();
    if(c != '\n') result += c;
  }
  return result.toInt();
}

/**********************************************************************************/

void getPh() {
  static float voltage;
  int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
  byte pHArrayIndex=0;  
  for (byte i = 0; i < ArrayLenth; i++ ) {
      pHArray[pHArrayIndex++]=analogRead(PhMeterPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
  }
  
  #ifdef DEBUG
    Serial.print("Volt: "); Serial.println(voltage); 
    Serial.print("Ph:   "); Serial.println(pHValue); 
  #endif
}

/**********************************************************************************/

double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}

/**********************************************************************************/

float readEC( byte isens ) {
  
  float RS1 = 0.;
  float RS2 = 0.;
  float VCC = 5000.;
  float Vconv = VCC/1024.;
  float V1 = 0.;
  float V2 = 0.;
  float VSG = 0.;
  
  float runningAvg1 = 0.;
  float runningAvg2 = 0.;
  float runningAvg3 = 0.;
  
  for(byte i = 0; i < n; i++) {
    digitalWrite(pinEC1[isens],HIGH);
    digitalWrite(pinEC3[isens],LOW);
    runningAvg1 = runningAvg1 + Vconv*analogRead(pinEC2[isens]);         
    digitalWrite(pinEC1[isens],LOW);
    runningAvg2 = runningAvg2 + Vconv*analogRead(pinEC2[isens]);         
    digitalWrite(pinEC3[isens],HIGH);
    runningAvg3 = runningAvg3 + Vconv*analogRead(pinEC2[isens]);         
  }
  VSG = runningAvg2/n;
  V1  = runningAvg1/n;      
  V2  = runningAvg3/n;      
  //  compute RH and VS
  if(V1 < 2.){
    RS[isens] = 60000.;
    CS[isens] = 0.;
  } else {
    //  correggo con la galvanica
    V1 = V1-VSG;
    V2 = V2-VSG;
    //  primo calcolo di RS
    RS1 = R1*(VCC-V1)/V1; 
    //  secondo calcolo di RS e media tra i due (RS in kohm)
    RS2 = R1*V2/(VCC-V2);
    RS[isens] = (RS1 + RS2)/2000.;
    //  calcolo CS secondo i parametri di taratura
    if( RS[isens] < 0.5) { CS[isens] = 1300; } else{ CS[isens] = A[isens]*pow(RS[isens],B[isens]); }          
  }
  
  #ifdef DEBUG
    Serial.print( "EC: " ); Serial.println( CS[isens] ); 
  #endif
  
  return CS[isens];
}

/**********************************************************************************/

void apriGocciolatore() {
   Serial.println( "apri" );
   digitalWrite(pinFert1,HIGH);
}

/**********************************************************************************/

void chiudiGocciolatore() {
   Serial.println( "chiudi" );
   digitalWrite(pinFert1,LOW);
}

/**********************************************************************************/

void writeText( String text1, String text2 ) {
  /*
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(text1);
  lcd.setCursor(0, 1);
  lcd.print(text2);
  */
  Serial.print(text1);
  Serial.print(text2);
}

/**********************************************************************************/


