#include "Enes100Simulation.h"
#include "DFRTankSimulation.h"

#define abs(x) ((x)>0?(x):-(x))

Enes100Simulation enes;
DFRTankSimulation tank;

float locError=.30;


void setup() {
  tank.init();
  enes.println("Starting Navigation");
  while (!enes.retrieveDestination());
  while (!enes.updateLocation());
  missionSite();
}

void loop() {

  distance();
  //move forward
  driveForward(0);


  /************************************************************************************************
    If the distance sensor detects an obstacle, first the OSV will move backwards for 0.3 seconds.
    Next, the OSV will decide whether to turn right or left based on its current location.
    If the OSV is above 1.5 Meters, it will turn to the right. If the OSV is 1.5 meters or below,
    it will turn to the left. The OSV will then move forward in that direction for 1.5 seconds.
    The method missionSite() will perform to determine the OSV location in comparison to mission
   ***********************************************************************************************/
  while (enes.readDistanceSensor(1) < 0.5 || enes.readDistanceSensor(0) < 0.5 || enes.readDistanceSensor(2) < 0.5) {
    enes.println("Found Obstacle");
    driveBackward(.4);
    stopMotors(0);
    enes.updateLocation();
    if (enes.location.y <= 1.2) {
      turnLeft(.7);
    }
    if (enes.location.y > 1.2) {
      turnRight(.7);
    }
    driveForward(2);
    missionSite();
  }

  /*if (enes.location.x >= enes.destination.x-locError  && (enes.location.y <= enes.destination.y+locError||enes.location.y >= enes.destination.y-locError)) {
    tank.setLeftMotorPWM(0);
    tank.setRightMotorPWM(0);
    enes.print("done");
  }*/
  if(distance()<=locError){
    missionSite();
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
    enes.println("");
    enes.print(destinationTheta);
    enes.print("     ");
    enes.print(currentTheta);
    if (currentTheta <= destinationTheta + EPSILON && currentTheta >= destinationTheta - EPSILON) {
      break;
    }
  }
  enes.print("done");
}

void driveForward(float t){
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(255);
  delay(t*1000);
}

void turnLeft(float t){
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(255);
  delay(t*1000);
}

void turnRight(float t){
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(-255);
  delay(t*1000);
}

void driveBackward(float t){
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(-255);
  delay(t*1000);
}

void stopMotors(float t){
  tank.setLeftMotorPWM(0);
  tank.setRightMotorPWM(0);
  delay(t*1000);
}

float distance(){
  enes.updateLocation();
  enes.retrieveDestination();
  float currentX = enes.location.x;
  float currentY = enes.location.y;
  float destinationX = enes.destination.x;
  float destinationY = enes.destination.y;
  float deltY = (destinationY - currentY);
  float deltX = (destinationX - currentX);
  float distance=sqrt(pow(deltY, 2)+pow(deltX, 2));
  enes.println(distance);
  return distance;
}


