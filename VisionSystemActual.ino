#include "Enes100.h"

#define abs(x) ((x)>0?(x):-(x))

Enes100Simulation enes;
DFRTankSimulation tank;

// RF Communication
int markerID = 9;
int aSerialRecieve = 6;
int aSerialTransmit = 2 ;

// motor one (L)
int enA = 10;
int in1 = 12;
int in2 = 8;

// motor two (R)
int enB = 5;
int in3 = 7;
int in4 = 4;

// IR sensor
int irSensorPin = A1;
boolean obstacle = false;

float locError=.30;
Enes100 enes("Heisenburg", CHEMICAL, markerID, aSerialRecieve, aSerialTransmit);

void setup() {
  tank.init();
  pinMode(irSensorPin, INPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  while (!enes.retrieveDestination());
  while (!enes.updateLocation());
  missionSite();
}

 /************************************************************************************************
    If the distance sensor detects an obstacle, first the OSV will move backwards for 0.2 meters.
    Next, the OSV will decide whether to turn right or left based on its current location.
    If the OSV is above 1.2 Meters, it will turn to the right. If the OSV is 1.2 meters or below,
    it will turn to the left. The OSV will then move forward in that direction for 0.4 meters.
    The method missionSite() will perform to determine the OSV location in comparison to mission
   ***********************************************************************************************/

void loop() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  if(navigation()==false){
  navigation();
  }
  
}

boolean navigation(){
  boolean navigated=false;
 //calculate distance to destination
  distanceToMission();
  //move forward
  driveForward(.3, enes.location.x, enes.location.y, enes.location.theta);
  //turn to face destination
  missionSite();
  //obstacle is detected
  while(foundObstacle()==true) {
    enes.updateLocation();
    enes.println("Found Obstacle");
    driveBackward(.2, enes.location.x, enes.location.y, enes.location.theta);
    stopMotors(0);
    enes.updateLocation();
    if (enes.location.y <= 1.2) {
      turnLeft(enes.location.theta);
    }
    if (enes.location.y > 1.2) {
      turnRight(enes.location.theta);
    }
    driveForward(.4, enes.location.x, enes.location.y, enes.location.theta);
    missionSite();
  }

  if(distanceToMission()<=locError){
    missionSite();
    navigated=true;
    enes.navigated();
    
  }
}
  return navigated;
}

void missionSite() {
  enes.updateLocation();
  enes.retrieveDestination();
  float currentX = enes.location.x;
  float currentY = enes.location.y;
  float currentTheta = enes.location.theta * 100 / 100;
  float destinationX = enes.destination.x;
  float destinationY = enes.destination.y;
  float EPSILON = 0.01;
  float deltY = (destinationY - currentY);
  float deltX = (destinationX - currentX);
  float destinationTheta = atan(deltY / deltX) * 100 / 100;
  while (destinationTheta != currentTheta) {
    enes.updateLocation();
    enes.retrieveDestination();
    destinationX = enes.destination.x;
    destinationY = enes.destination.y;
    currentX = enes.location.x;
    currentY = enes.location.y;
    deltY = (destinationY - currentY);
    deltX = (destinationX - currentX);
    currentTheta = enes.location.theta * 100 / 100;
    destinationTheta = atan(deltY / deltX) * 100 / 100;
    if(destinationTheta<=0&&currentTheta<=0){
      if (abs(destinationTheta) <= abs(currentTheta)) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }
      else{
        //turn right
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH); 
      }
    }
   
    else if(destinationTheta>0&&currentTheta>0){
      if (destinationTheta <= currentTheta) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
    }
      else{
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW); 
      }
    }
    else if(destinationTheta<=0&&currentTheta>=0){
      if(abs(destinationTheta-currentTheta)<=3.14){
        //turn right
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, HIGH);
      }
      else{
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW); 
      }
    }
    else if(destinationTheta>=0&&currentTheta<=0){
      if(abs(destinationTheta-currentTheta)<=3.14){
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
      }
      else{
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, HIGH); 
      }
      delay(100);
    }
    if (currentTheta <= destinationTheta + EPSILON && currentTheta >= destinationTheta - EPSILON) {
      break; 
    }
  }
}

//drive forward a certain distance
boolean driveForward(float d, float x, float y, float t){
  float destX = x+(d*cos(abs(t)));
  float destY;
  if(t>0){
  destY=y+(d*sin(abs(t)));
  enes.updateLocation();
  while((enes.location.x<destX)&&(enes.location.y<destY)&&(enes.readDistanceSensor(1) > 0.45 && enes.readDistanceSensor(0) > 0.45 && enes.readDistanceSensor(2) > 0.45)&&distanceToMission()>=locError){
    enes.updateLocation();
    enes.retrieveDestination();
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    if((enes.readDistanceSensor(1) <= 0.45 && enes.readDistanceSensor(0) <= 0.45 && enes.readDistanceSensor(2) <= 0.45)){
      return false;
    }
    }
  }
  if(t<=0){
  destY=y-(d*sin(abs(t)));
  while((enes.location.x<destX)&&(enes.location.y>destY)&&(enes.readDistanceSensor(1) > 0.45 && enes.readDistanceSensor(0) > 0.45 && enes.readDistanceSensor(2) > 0.45)&&distanceToMission()>=locError){
  enes.updateLocation();
  enes.retrieveDestination();
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  if((enes.readDistanceSensor(1) <= 0.45 && enes.readDistanceSensor(0) <= 0.45 && enes.readDistanceSensor(2) <= 0.45)){
      return false;
    }
  }
  }
  return true;
}

//Turns left a certain angle
void turnLeft(float t){
  float turnAngle=.4;
  while(enes.location.theta<t+turnAngle){
  enes.updateLocation();
  enes.retrieveDestination();
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  }
 }
 
//Turns right a certain angle
void turnRight(float t){
  float turnAngle=.4;
  while(enes.location.theta>t-turnAngle){
  enes.updateLocation();
  enes.retrieveDestination();
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  }
  }
 
//drive backwards a certain distance
void driveBackward(float d, float x, float y, float t){
  float backwardsX = x-(d*cos(abs(t)));
  float backwardsY;
  if(t>0){
  backwardsY=y-(d*sin(abs(t)));
  enes.updateLocation();
  while((enes.location.x>backwardsX)&&(enes.location.y>backwardsY)){
    enes.updateLocation();
    enes.retrieveDestination();
    analogWrite(enA, -255);
    analogWrite(enB, -255);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    }
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  }
  else if(t<=0){
  backwardsY=y+(d*sin(abs(t)));
  while((enes.location.x>backwardsX)&&(enes.location.y<backwardsY)){
  enes.updateLocation();
  enes.retrieveDestination();
    analogWrite(enA, -255);
    analogWrite(enB, -255);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  }
}

void stopMotors(float t){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(t*1000);
}

float distanceToMission(){
  enes.updateLocation();
  enes.retrieveDestination();
  float currentX = enes.location.x;
  float currentY = enes.location.y;
  float destinationX = enes.destination.x;
  float destinationY = enes.destination.y;
  float deltY = (destinationY - currentY);
  float deltX = (destinationX - currentX);
  float distance=sqrt(pow(deltY, 2)+pow(deltX, 2));
  return distance;
}

boolean foundObstacle() {
  int obstacleReading = digitalRead(irSensorPin);
  if (obstacleReading == LOW) {
    return true;
  }
  else {
    return false;
  }
}


