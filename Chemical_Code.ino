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
  while (abs(enes.location.theta) > 0.05) {

    tank.setLeftMotorPWM(255);
    tank.setRightMotorPWM(-255);

    while (!enes.updateLocation());
    
  }

  //move forward
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(255);

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
    tank.setLeftMotorPWM(255);
    tank.setRightMotorPWM(-255);
    delay(500);
    break;
  }
  

}
