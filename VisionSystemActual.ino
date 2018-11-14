#include "Enes100Simulation.h"
#include "DFRTankSimulation.h"

#define abs(x) ((x)>0?(x):-(x))

Enes100Simulation enes;
DFRTankSimulation tank;

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
navigation();
}

void navigation(){
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
    enes.navigated();
    stopMotors(300);
  }
}
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
      tank.setLeftMotorPWM(-40);
      tank.setRightMotorPWM(40);
    }
      else{
      tank.setLeftMotorPWM(40);
      tank.setRightMotorPWM(-40);  
      }
    }
    else if(destinationTheta>0&&currentTheta>0){
      if (destinationTheta <= currentTheta) {
      tank.setLeftMotorPWM(40);
      tank.setRightMotorPWM(-40);
    }
      else{
      tank.setLeftMotorPWM(-40);
      tank.setRightMotorPWM(40);  
      }
    }
    else if(destinationTheta<=0&&currentTheta>=0){
      if(abs(destinationTheta-currentTheta)<=3.14){
        tank.setLeftMotorPWM(40);
        tank.setRightMotorPWM(-40);
      }
      else{
        tank.setLeftMotorPWM(-40);
        tank.setRightMotorPWM(40);  
      }
    }
    else if(destinationTheta>=0&&currentTheta<=0){
      if(abs(destinationTheta-currentTheta)<=3.14){
        tank.setLeftMotorPWM(-40);
        tank.setRightMotorPWM(40);
      }
      else{
        tank.setLeftMotorPWM(40);
        tank.setRightMotorPWM(-40);  
      }
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
    tank.setLeftMotorPWM(255);
    tank.setRightMotorPWM(255);
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
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(255);
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
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(255);
  }
 }
 
//Turns right a certain angle
void turnRight(float t){
  float turnAngle=.4;
  while(enes.location.theta>t-turnAngle){
  enes.updateLocation();
  enes.retrieveDestination();
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(-255);
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
    tank.setLeftMotorPWM(-255);
    tank.setRightMotorPWM(-255);
    }
  }
  else if(t<=0){
  backwardsY=y+(d*sin(abs(t)));
  while((enes.location.x>backwardsX)&&(enes.location.y<backwardsY)){
  enes.updateLocation();
  enes.retrieveDestination();
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(-255);  
  }
  }
}

void stopMotors(float t){
  tank.setLeftMotorPWM(0);
  tank.setRightMotorPWM(0);
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

