#include "Enes100Simulation.h"
#include "DFRTankSimulation.h"

#define abs(x) ((x)>0?(x):-(x))

Enes100Simulation enes;
DFRTankSimulation tank;



  
void setup() {
  tank.init();
  enes.println("Starting Navigation");
  while (!enes.retrieveDestination());
  while (!enes.updateLocation());
    missionSite();
}

void loop() {


  //move forward
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(255);


  /************************************************************************************************
   *If the distance sensor detects an obstacle, first the OSV will move backwards for 0.3 seconds.*                                          
   *Next, the OSV will decide whether to turn right or left based on its current location.        *
   *If the OSV is above 1.5 Meters, it will turn to the right. If the OSV is 1.5 meters or below, *
   *it will turn to the left. The OSV will then move forward in that direction for 1.5 seconds.   *
   *The method missionSite() will perform to determine the OSV location in comparison to mission  *
   ***********************************************************************************************/
while(enes.readDistanceSensor(1) < 0.5 || enes.readDistanceSensor(0) < 0.5 || enes.readDistanceSensor(2) < 0.5){
  enes.println("Found Obstacle");
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(-255);
  delay(600);
  tank.setLeftMotorPWM(0);
  tank.setRightMotorPWM(0);
  enes.updateLocation();
   if(enes.location.y <= 1.2){
     tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(255);
  delay(700);
   } 
   if(enes.location.y > 1.2){
    tank.setLeftMotorPWM(255);
    tank.setRightMotorPWM(-255);
    delay(700);
   }
  

  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(255);
  delay(2000);
  missionSite();
}
  enes.updateLocation();
  float EPSILON = 0.01;
  enes.println(enes.destination.x);
  enes.println(enes.destination.y);
   if(enes.destination.x<=enes.location.x+EPSILON && enes.destination.x>=enes.location.x-EPSILON && enes.destination.y<=enes.location.y+EPSILON && enes.destination.y>=enes.location.y-EPSILON){
       tank.setLeftMotorPWM(0);
       tank.setRightMotorPWM(0);
    }
  
}

void missionSite(){
  enes.updateLocation();
  enes.retrieveDestination();
  float currentX = enes.location.x;
  float currentY = enes.location.y;
  float currentTheta = enes.location.theta*100/100;
  float destinationX = enes.destination.x;
  float destinationY = enes.destination.y;
  float EPSILON = 0.01;
  float deltY = (destinationY - currentY);
  float deltX = (destinationX - currentX);
  float destinationTheta = atan(deltY/ deltX) * 100 / 100;
  while(destinationTheta != currentTheta){
    if(destinationTheta>=0){
    tank.setLeftMotorPWM(40);
    tank.setRightMotorPWM(-40);
    }
    if(destinationTheta<0){
    tank.setLeftMotorPWM(-40);
    tank.setRightMotorPWM(40);  
    }
    enes.updateLocation();
    enes.retrieveDestination();
    destinationX = enes.destination.x;
    destinationY = enes.destination.y;
    currentX = enes.location.x;
    currentY = enes.location.y;
    deltY = (destinationY - currentY);
    deltX = (destinationX - currentX);
    currentTheta = enes.location.theta*100/100;
    destinationTheta = atan(deltY/ deltX) * 100 / 100;
    enes.println("");
    enes.print(destinationTheta);
    enes.print("     ");
    enes.print(currentTheta);
    if(currentTheta<=destinationTheta+EPSILON && currentTheta>=destinationTheta-EPSILON){
      break;
    }
  }

  
  
}
