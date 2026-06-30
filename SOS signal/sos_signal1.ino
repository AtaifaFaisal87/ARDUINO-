// C++ code
//SOS SIGNAL

int redLED=13;
int dit=800;
int ditdit=3000;
int dah=500;

void setup()
{
  pinMode(13, OUTPUT);
}

void loop()
{
  //S
  digitalWrite(redLED, HIGH);
  delay(dit); 
  digitalWrite(redLED, LOW);
  delay(dah);
  digitalWrite(redLED, HIGH);
  delay(dit);
  digitalWrite(redLED, LOW);
  delay(dah);
  digitalWrite(redLED, HIGH);
  delay(dit);
  digitalWrite(redLED, LOW);
  delay(dah);
  
  
  //O
  digitalWrite(redLED, HIGH);
  delay(ditdit);
    digitalWrite(redLED, LOW);
  delay(dah);
  digitalWrite(redLED, HIGH);
  delay(ditdit);
    digitalWrite(redLED, LOW);
  delay(dah);
  digitalWrite(redLED, HIGH);
  delay(ditdit);
    digitalWrite(redLED, LOW);
  delay(dah);
  
  
  //S
  digitalWrite(redLED, HIGH);
  delay(dit); 
  digitalWrite(redLED, LOW);
  delay(dah);
  digitalWrite(redLED, HIGH);
  delay(dit);
  digitalWrite(redLED, LOW);
  delay(dah);
  digitalWrite(redLED, HIGH);
  delay(dit);
  digitalWrite(redLED, LOW);
  delay(dah);
  
  delay(5000);
}