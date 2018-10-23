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

}

void loop() {
  int j = false;
  enes.print( enes.destination.x);
  //turn to face forward


  missionSite();
  //move forward
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(255);
while(true){
if(enes.readDistanceSensor(1) < 0.5 || enes.readDistanceSensor(0) < 0.5 || enes.readDistanceSensor(2) < 0.5){
   enes.println("Found Obstacle");
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(-255);
  delay(200);
  tank.setLeftMotorPWM(0);
   tank.setRightMotorPWM(0);
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(255);
  delay(1000);
  j  = true;
}
while(j){
  enes.println("SHIT");
    tank.setLeftMotorPWM(255);
    tank.setRightMotorPWM(-255);
    delay(500);
    break;
  }
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(255);
  if(j){
    j = false;
  delay(3200);
  while (!enes.retrieveDestination());

  while (!enes.updateLocation());
  
  missionSite();
  }
}
}
void locUpdate(){
  while (abs(enes.location.theta) > 0.02) {
    tank.setLeftMotorPWM(255);
    tank.setRightMotorPWM(-255);
    while (!enes.updateLocation());
    
  }
}
void missionSite(){
  enes.updateLocation();
  enes.retrieveDestination();
  float currentX = enes.location.x;
  float currentY = enes.location.y;
  float currentTheta = enes.location.theta;
  float destinationX = enes.destination.x;
  float destinationY = enes.destination.y;

  float EPSILON = 0.111;
  float deltY = (destinationY - currentY);
  float deltX = (destinationX - currentX);

  float destinationTheta = atan(deltY / deltX);
 /* enes.println(currentX);
  enes.println(currentY);
  enes.println(destinationX);
  enes.println(destinationY);
  enes.println(destinationTheta);
  */
  while(destinationTheta != currentTheta || abs(destinationTheta - currentTheta) < EPSILON){
    tank.setLeftMotorPWM(255);
    tank.setRightMotorPWM(-255);
      //UPDATE
    
  }

  
  
}
