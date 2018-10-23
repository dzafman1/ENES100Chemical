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
  
  //turn to face forward


  locUpdate();
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
  while (abs(enes.location.theta) > 0.02) {
    tank.setLeftMotorPWM(255);
    tank.setRightMotorPWM(-255);
    while (!enes.retrieveDestination());
    
  }
}
