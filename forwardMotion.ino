// motor one
int enA = 10;
int in1 = 12;
int in2 = 8;

// motor two
int enB = 5;
int in3 = 7;
int in4 = 4;

// motor three
int enA_1 = 9;
//int in1_1 = 2;
//int in2_1 = 1;

//motor four
int enB_1 = 11;
//int in1_1 = 0;
//int in2_1 = 3;

//All Motor Controller pins must be set to outputs.
void setup()
{
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(enA_1, OUTPUT);
  pinMode(enB_1, OUTPUT);
  pinMode(in1, OUTPUT);
  //pinMode(in1_1, OUTPUT);
  //pinMode(in2_1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
 // pinMode(in3_1, OUTPUT);
  pinMode(in4, OUTPUT);
  //pinMode(in4_1, OUTPUT);
  
  
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
  
 // digitalWrite(in1_1, HIGH);
 // digitalWrite(in2_1, LOW);
 // digitalWrite(in3_1, HIGH);
  //digitalWrite(in4_1, LOW);
  
  //set speed to 255 out of possible range 0~255
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  analogWrite(enA_1, 255);
  analogWrite(enB_1, 255);
  
  delay(180000);

//STOP AFTER 3 MINUTES.
while(true){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
 // digitalWrite(in1_1, LOW);
  //digitalWrite(in2_1, LOW);
  //digitalWrite(in3_1, LOW);
  //digitalWrite(in4_1, LOW);
}

}
