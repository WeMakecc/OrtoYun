/**********************************************************************************/

#define DEBUG 1

/**********************************************************************************/

#define textBuffSize 500
#define PhMeterPin   A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset       0.24          //deviation compensate
#define ArrayLenth   40            //times of collection
#define pinLamp1     7            // Relay right Light
#define pinLamp2     8            // Relay left Light
#define pinPump1     12            // Relay Pump 1
#define pinPump2     13            // Relay Pump 2 - SmartCitizen
#define pinFert1     3             // elettrovalvola Fertilizzante

int index = 0;
boolean startData = false;
char results[textBuffSize];

double temp, hum, co, no2, light, noise, batt;
  
/**********************************************************************************/

static float pHValue = 0;
static byte  level   = 0;
static float CS1     = 0;
static byte  fertil  = 0;

/**********************************************************************************/

boolean statusLight1 = false;
boolean statusLight2 = false;
boolean statusPump1  = false;
boolean statusPump2  = false;
boolean statusFert  = false;
byte lastTimeSet = 0;
char lcdBuffer1[16] = "";
char lcdBuffer2[16] = "";

/**********************************************************************************/

byte nsens = 2;
byte pinEC1[] = {3,5};
byte pinEC2[] = {A1,A2};
byte pinEC3[] = {4,6};
// parametri di taratura in uS/cm
float A[] = {870.43,822.71};
float B[] = {-1.194,-1.190};
float R1 = 470.;  //  resistenza fissa tra P1 e P3 in ohm
float RS[] = {0,0}; //  inizializzo resistenza tra i due elettrodi 
float CS[] = {0,0}; //  inizializzo conducibilità specifica

byte isens = 0;
byte n = 50;

/**********************************************************************************/

byte currHour = 0;
byte currMin  = 0;
byte lastMin  = -1;
byte writeMin = -1;

/**********************************************************************************/

char* logFile = "/mnt/sd/arduino/www/orto.log";

