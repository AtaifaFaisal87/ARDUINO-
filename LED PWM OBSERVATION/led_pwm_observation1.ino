// C++ code
//
int redPIN=9;
int bright=127.5;
int dim=146;
void setup()
{
  pinMode(redPIN, OUTPUT);
}

void loop()
{
  analogWrite(redPIN, bright);
  delay(1000); // Wait for 1000 millisecond(s)
  //analogWrite(redPIN, 0);
  //delay(1000); // Wait for 1000 millisecond(s)
}