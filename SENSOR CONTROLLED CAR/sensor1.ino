// ================= MOTOR PINS (Your Configurations) =================
// Motor 1 (Left Wheel)
const int motor1_enable = 11; 
const int motor1_in1    = 12; 
const int motor1_in2    = 13; 

// Motor 2 (Right Wheel)
const int motor2_enable = 9;  
const int motor2_in3    = 7;  
const int motor2_in4    = 8;  

// ================= ULTRASONIC SENSOR =================
const int trigPin = 5;
const int echoPin = 6;

// ================= BUZZER =================
const int buzzer = 4;

// ================= LEDs =================
const int greenLED = 3;
const int redLED = 10;

// ================= TUNING VARIABLES =================
const int maxSpeed = 200;      // Set slightly lower than 255 for safer lab testing
const int accelStep = 15;      // Snappy acceleration
const int decelStep = 25;      // Fast braking on obstacles
const int updateInterval = 20; // Math loop refresh rate

int currentSpeedLeft = 0;      
int currentSpeedRight = 0;     
unsigned long lastUpdateTime = 0;

int targetLeft = 0;
int targetRight = 0;
int distance = 0;

// Memory variable: 0 means last turn was Left, 1 means last turn was Right
int lastTurnDirection = 0; 

// ================= SETUP =================
void setup() {
  // Initialize motor controller pins
  pinMode(motor1_enable, OUTPUT);
  pinMode(motor1_in1, OUTPUT);
  pinMode(motor1_in2, OUTPUT);
  
  pinMode(motor2_enable, OUTPUT);
  pinMode(motor2_in3, OUTPUT);
  pinMode(motor2_in4, OUTPUT);
  
  // Sensor & Accessory Pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  Serial.begin(9600);
  Serial.println("--- Autonomous Robot with Alternate Direction Memory ---");
}

// ================= ULTRASONIC SENSOR FUNCTION =================
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); 

  // Fix: Return 400 if out of bounds or zero to prevent accidental buzzer triggers
  if (duration == 0) return 400; 
  
  int calculatedDistance = duration * 0.0343 / 2;
  if (calculatedDistance <= 0) return 400;
  
  return calculatedDistance;  
}

// ================= ACTION UTILITIES =================
void setTargetTargets(int left, int right) {
  targetLeft = left;
  targetRight = right;
}

void emergencyStop() {
  currentSpeedLeft = 0;
  currentSpeedRight = 0;
  targetLeft = 0;
  targetRight = 0;
  drive(0, 0);
}

void executeRamping() {
  unsigned long startWait = millis();
  while (millis() - startWait < 200) { 
    if (millis() - lastUpdateTime >= updateInterval) {
      lastUpdateTime = millis();

      if (currentSpeedLeft < targetLeft) currentSpeedLeft += accelStep;
      else if (currentSpeedLeft > targetLeft) currentSpeedLeft -= decelStep;

      if (currentSpeedRight < targetRight) currentSpeedRight += accelStep;
      else if (currentSpeedRight > targetRight) currentSpeedRight -= decelStep;

      drive(currentSpeedLeft, currentSpeedRight);
    }
  }
}

// ================= MAIN PROGRAM LOOP =================
void loop() {
  distance = getDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 15) {
    // Path is clear -> Move Forward smoothly
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
    noTone(buzzer);
    
    setTargetTargets(maxSpeed, maxSpeed);
    
    if (millis() - lastUpdateTime >= updateInterval) {
      lastUpdateTime = millis();
      if (currentSpeedLeft < targetLeft) currentSpeedLeft += accelStep;
      if (currentSpeedRight < targetRight) currentSpeedRight += accelStep;
      drive(currentSpeedLeft, currentSpeedRight);
    }
  } 
  else {
    // --- OBSTACLE EVASION ROUTINE ---
    emergencyStop(); // Instant Stop
    
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    
    // Buzz clearly to signal obstacle detection
    tone(buzzer, 1000);
    delay(300);
    noTone(buzzer);

    // Back up slightly to give turning clearance
    setTargetTargets(-maxSpeed, -maxSpeed);
    executeRamping();
    delay(400); 
    emergencyStop();
    delay(100);

    // Decide direction based on alternative memory tracking
    if (lastTurnDirection == 0) {
      // Last turn was Left (or first start), so this time turn RIGHT
      Serial.println("Action: Alternating to turn RIGHT");
      setTargetTargets(maxSpeed, -maxSpeed); // Pivot Right
      executeRamping();
      delay(600);
      
      lastTurnDirection = 1; // Update memory to reflect we just turned Right
    } 
    else {
      // Last turn was Right, so this time turn LEFT
      Serial.println("Action: Alternating to turn LEFT");
      setTargetTargets(-maxSpeed, maxSpeed); // Pivot Left
      executeRamping();
      delay(600);
      
      lastTurnDirection = 0; // Update memory to reflect we just turned Left
    }

    emergencyStop();
    delay(150);
  }
  delay(10);
}

// ================= HARDWARE MOTOR DRIVER LAYER =================
void drive(int speed1, int speed2) {
  // Left Wheel (Motor 1)
  if (speed1 > 0) {
    digitalWrite(motor1_in1, HIGH);
    digitalWrite(motor1_in2, LOW);
    analogWrite(motor1_enable, speed1);
  } else if (speed1 < 0) {
    digitalWrite(motor1_in1, LOW);
    digitalWrite(motor1_in2, HIGH);
    analogWrite(motor1_enable, abs(speed1)); 
  } else {
    digitalWrite(motor1_in1, LOW);
    digitalWrite(motor1_in2, LOW);
    analogWrite(motor1_enable, 0);
  }

  // Right Wheel (Motor 2)
  if (speed2 > 0) {
    digitalWrite(motor2_in3, HIGH);
    digitalWrite(motor2_in4, LOW);
    analogWrite(motor2_enable, speed2);
  } else if (speed2 < 0) {
    digitalWrite(motor2_in3, LOW);
    digitalWrite(motor2_in4, HIGH);
    analogWrite(motor2_enable, abs(speed2));
  } else {
    digitalWrite(motor2_in3, LOW);
    digitalWrite(motor2_in4, LOW);
    analogWrite(motor2_enable, 0);
  }
}