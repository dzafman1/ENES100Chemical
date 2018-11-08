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

void setup() {
  
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

void loop() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  analogWrite(enA_1, 255);
  analogWrite(enB_1, 255);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(3500);
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(2000);
}
