// motor one (L)
int enA = 10;
int in1 = 12;
int in2 = 8;

// motor two (R)
int enB = 5;
int in3 = 7;
int in4 = 4;

//All Motor Controller pins must be set to outputs.
void setup()
{
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
 
}


//Drives vehicle forward for 3 minutes.
void loop()
{

  //in2 MUST BE HIGH AND in1 MUST BE LOW TO DRIVE FORWARD. 
  //in3 HIGH in4 LOW.
  
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  
  //set speed to 255 out of possible range 0~255
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  
  delay(180000);

//STOP AFTER 3 MINUTES.
while(true){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
}

}
