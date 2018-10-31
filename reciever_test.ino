#include "Enes100.h"

int markerID = 3;
int aSerialRecieve = 9;
int aSerialTransmit = 8 ;

Enes100 enes("Heisenburg", CHEMICAL, markerID, aSerialRecieve, aSerialTransmit);


void setup() {
}

void loop() {
  enes.retrieveDestination();
  float destinationX = enes.destination.x;
  float destinationY = enes.destination.y;
  Coordinate destinationLocation(destinationX, destinationY);
  
  enes.updateLocation();
  float currentX = enes.location.x;
  float currentY = enes.location.y;
  float currentTheta = enes.location.theta;
  Coordinate currentLocation(currentX, currentY, currentTheta);
//  String currentLocationString = "Current Location: " + currentX + ", " + currentY;

  enes.print("Destination coordinate: (");
  enes.print(destinationX);
  enes.print(", ");
  enes.print(destinationY);
  enes.println(")");

  enes.print("Current location coordinate: ");
  enes.print(currentX);
  enes.print(", ");
  enes.print(currentY);
  enes.print(", ");
  enes.print(currentTheta);
  enes.println(")");
  

  delay(5000);
}
