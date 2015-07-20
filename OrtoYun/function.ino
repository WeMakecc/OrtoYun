/**********************************************************************************/

boolean getApiSmartCitizen() {
  
  HttpClient client;
  int lenght = client.get("http://localhost/sd/smartCitizen.php");
 
  while (client.available()) {
       char c = client.read();
       results[index] = c; index++;
  }
  results[index]=0;
  index = 0;

  #ifdef DEBUG
    Serial.println("--- BEGIN getApiSmartCitizen ---");
    Serial.print( "Result: " ); Serial.println(String( results ));
    Serial.println("---- END getApiSmartCitizen ----");
  #endif
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
    Serial.println("--- BEGIN getPh ---");
    Serial.print("Volt: "); Serial.println(voltage); 
    Serial.print("Ph:   "); Serial.println(pHValue);
    Serial.println("---- END getPh ----");
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
    Serial.println("--- BEGIN readEC ---");
    Serial.print( "EC: " ); Serial.println( CS[isens] ); 
    Serial.println("---- END readEC ----");
  #endif
  
  return CS[isens];
}

/**********************************************************************************/

void apriGocciolatore() {
   Serial.println( "apri" );
   digitalWrite(pinFert1,HIGH);
   
   #ifdef DEBUG
    Serial.println("--- BEGIN apriGocciolatore ---");
    Serial.print( "Gocciolatore: HIGH" ); 
    Serial.println("---- END apriGocciolatore ----");
   #endif
}

/**********************************************************************************/

void chiudiGocciolatore() {
   Serial.println( "chiudi" );
   digitalWrite(pinFert1,LOW);
   
   #ifdef DEBUG
    Serial.println("--- BEGIN chiudiGocciolatore ---");
    Serial.print( "Gocciolatore: LOW" ); 
    Serial.println("---- END chiudiGocciolatore ----");
   #endif
}

/**********************************************************************************/
