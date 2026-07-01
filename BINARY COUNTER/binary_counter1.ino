// C++ code
//

int pin2=2;
int pin4=4;
int pin8=8;
int pin12=12;

int dah=800;

void setup()
{
  pinMode(pin2, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin8, OUTPUT);
  pinMode(pin12, OUTPUT);
}

void loop()
{
  //0
  digitalWrite(pin2,LOW);
  digitalWrite(pin4,LOW);
  digitalWrite(pin8,LOW);
  digitalWrite(pin12,LOW);
  delay(dah);
        
  //1
  digitalWrite(pin2,HIGH);
  digitalWrite(pin4,LOW);
  digitalWrite(pin8,LOW);
  digitalWrite(pin12,LOW);
  delay(dah);
  
  //2
  digitalWrite(pin2,LOW);
  digitalWrite(pin4,HIGH);
  digitalWrite(pin8,LOW);
  digitalWrite(pin12,LOW);
  delay(dah);
  
  //3
  digitalWrite(pin2,HIGH);
  digitalWrite(pin4,HIGH);
  digitalWrite(pin8,LOW);
  digitalWrite(pin12,LOW);
  delay(dah);
  
  //4
  digitalWrite(pin2,LOW);
  digitalWrite(pin4,LOW);
  digitalWrite(pin8,HIGH);
  digitalWrite(pin12,LOW);
  delay(dah);
  
  //5
  digitalWrite(pin2,HIGH);
  digitalWrite(pin4,LOW);
  digitalWrite(pin8,HIGH);
  digitalWrite(pin12,LOW);
  delay(dah);
  
  //6
  digitalWrite(pin2,LOW);
  digitalWrite(pin4,HIGH);
  digitalWrite(pin8,HIGH);
  digitalWrite(pin12,LOW);
  delay(dah);
  
  //7
  digitalWrite(pin2,HIGH);
  digitalWrite(pin4,HIGH);
  digitalWrite(pin8,HIGH);
  digitalWrite(pin12,LOW);
  delay(dah);
  
  //8
  digitalWrite(pin2,LOW);
  digitalWrite(pin4,LOW);
  digitalWrite(pin8,LOW);
  digitalWrite(pin12,HIGH);
  delay(dah);
  
  //9
  digitalWrite(pin2,HIGH);
  digitalWrite(pin4,LOW);
  digitalWrite(pin8,LOW);
  digitalWrite(pin12,HIGH);
  delay(dah);
  
  //10
  digitalWrite(pin2,LOW);
  digitalWrite(pin4,HIGH);
  digitalWrite(pin8,LOW);
  digitalWrite(pin12,HIGH);
  delay(dah);
  
  //11
  digitalWrite(pin2,HIGH);
  digitalWrite(pin4,HIGH); 
  digitalWrite(pin8,LOW);  
  digitalWrite(pin12,HIGH);
  delay(dah);
  
  //12
  digitalWrite(pin2,LOW);  
  digitalWrite(pin4,LOW);  
  digitalWrite(pin8,HIGH);
  digitalWrite(pin12,HIGH);
  delay(dah);
  
  //13
  digitalWrite(pin2,HIGH); 
  digitalWrite(pin4,LOW);  
  digitalWrite(pin8,HIGH); 
  digitalWrite(pin12,HIGH);
  delay(dah); 
  
  //14
  digitalWrite(pin2,LOW);
  digitalWrite(pin4,HIGH); 
  digitalWrite(pin8,HIGH);  
  digitalWrite(pin12,HIGH);
  delay(dah);
  
  //15
  digitalWrite(pin2,HIGH);
  digitalWrite(pin8,HIGH); 
  digitalWrite(pin12,HIGH);
  delay(dah);
  
  delay(1000);
}