// motor one
int enA = 10;
int in1 = 12;
int in2 = 8;

// motor two
int enB = 5;
int in3 = 7;
int in4 = 4;


void setup() {
  
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

}

void loop() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(6000);

  while(true){
    
  }

}
