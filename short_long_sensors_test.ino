int LED = 13; // Use the onboard Uno LED
int longSensor = A1;
int shortSensor = A2;
//int isObstaclePin = 1;  // This is our input pin
int isObstacle = HIGH;  // HIGH MEANS NO OBSTACLE

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(longSensor, INPUT);
  pinMode(shortSensor, INPUT);
  
  Serial.begin(9600);  
}

int obsatcleOrPoolDetection(int pinLongSensor, int pinShortSensor) {
  int readingLong = digitalRead(pinLongSensor);
  int readingShort = digitalRead(pinShortSensor);
  Serial.println(readingLong);
  Serial.println(readingShort);

  delay(1000);
  if (readingShort == LOW) {
    Serial.println("POOL FOUND.");
    digitalWrite(LED, HIGH);
    return 1;
  }

  else if (readingLong == LOW) {
    Serial.println("OBSTACLE!!, OBSTACLE!!");
    digitalWrite(LED, HIGH);
    return 2;
  }
  else
  {
    Serial.println("clear");
    digitalWrite(LED, LOW);
    return 0;
  }
  delay(500);
}

void loop() {
  int value = 0;
  do {
    value = obsatcleOrPoolDetection(longSensor, shortSensor);
  } while (value == 0);
  if (value != 0) {
    while(true) {
      Serial.println("End of function.");
      delay(200000);
    }
  }
}


