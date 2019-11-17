//int redLed = 12;
//int greenLed = 11;
//int buzzer = 13;
int flame = A0;   
int state;      




void setup() {
  // put your setup code here, to run once:
//  pinMode(redLed, OUTPUT);
//  pinMode(greenLed, OUTPUT);
//  pinMode(buzzer, OUTPUT);
  pinMode(flame, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  state = analogRead(flame);
//  int LED = digitalRead(redLed);
  if(state == LOW) 
  {
//    digitalWrite(redLed, HIGH);
//    digitalWrite(greenLed, LOW);
//    tone(buzzer,800,800);
//    delay(200);
//    tone(buzzer,600,800);
 
    Serial.print("OFF ");
    Serial.println(state);
  }else if(state == HIGH) 
  {
//     digitalWrite(redLed, LOW);
//     digitalWrite(greenLed, HIGH);
//     noTone(buzzer);
      Serial.print("ON ");
    Serial.println(state);
  }
  delay(500);
}
