//#define LOW 1
//#define HIGH 0

void setup() {
  pinMode(D1_Green, OUTPUT);
  pinMode(D2_Green, OUTPUT);
  pinMode(D3_Green, OUTPUT);
  pinMode(D4_Green, OUTPUT);
  pinMode(D5_Green, OUTPUT);
}
 
 void loop() {
  digitalWrite(D1_Green, LOW);
  digitalWrite(D2_Green, HIGH);
  digitalWrite(D3_Green, HIGH);
  digitalWrite(D4_Green, HIGH);
  digitalWrite(D5_Green, HIGH);
  delay(50);
  digitalWrite(D1_Green, HIGH);
  digitalWrite(D2_Green, LOW);
  digitalWrite(D3_Green, HIGH);
  digitalWrite(D4_Green, HIGH);
  digitalWrite(D5_Green, HIGH);
  delay(50);
  digitalWrite(D1_Green, HIGH);
  digitalWrite(D2_Green, HIGH);
  digitalWrite(D3_Green, LOW);
  digitalWrite(D4_Green, HIGH);
  digitalWrite(D5_Green, HIGH);
  delay(50);
  digitalWrite(D1_Green, HIGH);
  digitalWrite(D2_Green, HIGH);
  digitalWrite(D3_Green, HIGH);
  digitalWrite(D4_Green, LOW);
  digitalWrite(D5_Green, HIGH);
  delay(50);
  digitalWrite(D1_Green, HIGH);
  digitalWrite(D2_Green, HIGH);
  digitalWrite(D3_Green, HIGH);
  digitalWrite(D4_Green, HIGH);
  digitalWrite(D5_Green, LOW);
  delay(50);
  digitalWrite(D1_Green, HIGH);
  digitalWrite(D2_Green, HIGH);
  digitalWrite(D3_Green, HIGH);
  digitalWrite(D4_Green, LOW);
  digitalWrite(D5_Green, HIGH);
  delay(50);
  digitalWrite(D1_Green, HIGH);
  digitalWrite(D2_Green, HIGH);
  digitalWrite(D3_Green, LOW);
  digitalWrite(D4_Green, HIGH);
  digitalWrite(D5_Green, HIGH);
  delay(50);
  digitalWrite(D1_Green, HIGH);
  digitalWrite(D2_Green, LOW);
  digitalWrite(D3_Green, HIGH);
  digitalWrite(D4_Green, HIGH);
  digitalWrite(D5_Green, HIGH);
  delay(50);
  digitalWrite(D1_Green, LOW);
  digitalWrite(D2_Green, HIGH);
  digitalWrite(D3_Green, HIGH);
  digitalWrite(D4_Green, HIGH);
  digitalWrite(D5_Green, HIGH);
  delay(50);
  
 }
