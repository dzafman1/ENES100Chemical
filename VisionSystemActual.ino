#include "Enes100Simulation.h"
#include "DFRTankSimulation.h"

#define abs(x) ((x)>0?(x):-(x))

Enes100Simulation enes;
DFRTankSimulation tank;

float locError=.30;
boolean stuck;
int stuckInLoop = 0; 

void setup() {
  tank.init();
  enes.println("Starting Navigation");
  while (!enes.retrieveDestination());
  while (!enes.updateLocation());
  missionSite();
}

void loop() {

  distanceToMission();
  //move forward
  driveForward(.3, enes.location.x, enes.location.y, enes.location.theta);
  missionSite();


  /************************************************************************************************
    If the distance sensor detects an obstacle, first the OSV will move backwards for 0.3 seconds.
    Next, the OSV will decide whether to turn right or left based on its current location.
    If the OSV is above 1.5 Meters, it will turn to the right. If the OSV is 1.5 meters or below,
    it will turn to the left. The OSV will then move forward in that direction for 1.5 seconds.
    The method missionSite() will perform to determine the OSV location in comparison to mission
   ***********************************************************************************************/
  while(enes.readDistanceSensor(1) < 0.45 || enes.readDistanceSensor(0) < 0.45 || enes.readDistanceSensor(2) < 0.45) {
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
    stuck=driveForward(.4, enes.location.x, enes.location.y, enes.location.theta);
    if(stuck==false){
      stuckInLoop++;
    }
    if(stuckInLoop==3){
      delay(10000);
    }
    missionSite();
  }

  if(distanceToMission()<=locError){
    enes.navigated();
    stopMotors(300);
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
