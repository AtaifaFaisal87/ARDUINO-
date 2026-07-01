// C++ code
//
void setup()
{
  pinMode(11, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop()
{
  digitalWrite(11, HIGH);
  digitalWrite(9, LOW);
  delay(3000); // Wait for 1000 millisecond(s)
  
  digitalWrite(11, LOW);
  digitalWrite(9, LOW);
  delay(3000);
  
  digitalWrite(9,HIGH);
  digitalWrite(11,LOW);
  delay(3000); // Wait for 1000 millisecond(s)
  
  digitalWrite(11, LOW);
  digitalWrite(9, LOW);
  delay(3000);
  
}