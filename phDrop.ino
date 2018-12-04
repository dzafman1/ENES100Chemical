#include <Adafruit_MCP23008.h>
#include "Wire.h"
#include "Enes100.h"
#define abs(x) ((x)>0?(x):-(x))

//pH sensor
#define SensorPin A0 //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.12 //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth 40 //times of collection
int pHArray[ArrayLenth]; //Store the average value of the sensor feedback
int pHArrayIndex=0;
// RF Communication
int markerID = 10 ; // change every time we get a marker
int aSerialRecieve = 6;
int aSerialTransmit = 2 ;

Enes100 enes("Heisenburg", CHEMICAL, markerID, aSerialRecieve, aSerialTransmit);

// cupMotor (L)
int enA = 11;
int in1 = 3;
int in2 = 0;

// phMotor (R)
int enB = 9;
int in3 = 1;
int in4 = 2;

Adafruit_MCP23008 mcp;


void setup()
{
  mcp.begin();
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  mcp.pinMode(in1, OUTPUT);
  mcp.pinMode(in2, OUTPUT);
  mcp.pinMode(in3, OUTPUT);
  mcp.pinMode(in4, OUTPUT);
 
}

void loop()
{
  analogWrite(enA, 100);
  analogWrite(enB, 100);
  mcp.digitalWrite(in1, LOW);
  mcp.digitalWrite(in2, HIGH);
  delay(3000);

  mcp.digitalWrite(in1, HIGH);
  mcp.digitalWrite(in2, LOW);
  delay(3000);
  
  mcp.digitalWrite(in1, HIGH);
  mcp.digitalWrite(in2, LOW);
  mcp.digitalWrite(in3, HIGH);
  mcp.digitalWrite(in4, LOW);

  delay(3000);
  
  mcp.digitalWrite(in1, LOW);
  mcp.digitalWrite(in2, LOW);
  mcp.digitalWrite(in3, LOW);
  mcp.digitalWrite(in4, LOW);

  while(true){
    initialpH();
  }
 
 /*
  delay(10000);
  mcp.digitalWrite(in3, LOW);
  mcp.digitalWrite(in4, HIGH);

  delay(3000);
  mcp.digitalWrite(in3, LOW);
  mcp.digitalWrite(in4, LOW);
  */

}
 boolean initialpH(){
static unsigned long samplingTime = millis();
static unsigned long printTime = millis();
static float pHValue,voltage;
int counter;
if(millis()-samplingTime > samplingInterval)
{
pHArray[pHArrayIndex++]=analogRead(SensorPin);
if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
pHValue = 3.5*voltage+Offset;
samplingTime=millis();
}
if(millis() - printTime > printInterval) //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
{
enes.print(" pH value: ");
enes.println(pHValue);
counter++;
digitalWrite(LED,digitalRead(LED)^1);
printTime=millis();
if(counter>=10){
  enes.baseObjective(pHValue);
  return true;
}
}
return false;
}

double avergearray(int* arr, int number){
int i;
int max,min;
double avg;
long amount=0;
if(number<=0){
enes.println("Error number for the array to avraging!/n");
return 0;
}
if(number<5){ //less than 5, calculated directly statistics
for(i=0;i<number;i++){
amount+=arr[i];
}
avg = amount/number;
return avg;
}
else{
if(arr[0]<arr[1]){
min = arr[0];max=arr[1];
}
else{
min=arr[1];max=arr[0];
}
for(i=2;i<number;i++){
if(arr[i]<min){
amount+=min; //arr<min
min=arr[i];
}else {
if(arr[i]>max){
amount+=max; //arr>max
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
 
