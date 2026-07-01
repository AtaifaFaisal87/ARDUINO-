// --- Motor Pin Definitions ---
// Motor 1 (Left Wheel)
const int motor1_enable = 11; 
const int motor1_in1    = 12; 
const int motor1_in2    = 13; 

// Motor 2 (Right Wheel)
const int motor2_enable = 9;  
const int motor2_in3    = 7;  
const int motor2_in4    = 8;  

// --- Button Pin Definitions ---
const int btnForwardPin  = 5; // Pin 5
const int btnBackwardPin = 6; // Pin 6
const int btnLeftPin     = 3; // Pin 2 (NEW)
const int btnRightPin    = 4; // Pin 4 (NEW)

// --- Tuning Variables ---
const int maxSpeed = 255;      // Top digital power limit (your motor's physical max is ~149 RPM)
const int accelStep = 10;      // Speed increase rate per 20ms step (increased for snappy takeoff)
const int decelStep = 15;      // Braking speed rate when a button is released
const int updateInterval = 20; // Time interval (in ms) to update speed calculation

int currentSpeedLeft = 0;      // Dynamically tracks left wheel speed
int currentSpeedRight = 0;     // Dynamically tracks right wheel speed
unsigned long lastUpdateTime = 0;

void setup() {
  // Initialize motor controller interface pins
  pinMode(motor1_enable, OUTPUT);
  pinMode(motor1_in1, OUTPUT);
  pinMode(motor1_in2, OUTPUT);
  
  pinMode(motor2_enable, OUTPUT);
  pinMode(motor2_in3, OUTPUT);
  pinMode(motor2_in4, OUTPUT);
  
  // Initialize all 4 buttons using internal pull-up resistors
  pinMode(btnForwardPin, INPUT_PULLUP);
  pinMode(btnBackwardPin, INPUT_PULLUP);
  pinMode(btnLeftPin, INPUT_PULLUP);
  pinMode(btnRightPin, INPUT_PULLUP);
}

void loop() {
  // Read all inputs (LOW means the button is physically pressed to GND)
  bool forward  = (digitalRead(btnForwardPin) == LOW);
  bool backward = (digitalRead(btnBackwardPin) == LOW);
  bool left     = (digitalRead(btnLeftPin) == LOW);
  bool right    = (digitalRead(btnRightPin) == LOW);

  int targetLeft = 0;
  int targetRight = 0;

  // --- Direction Input Priority Matrix ---
  if (forward) {
    targetLeft = maxSpeed;
    targetRight = maxSpeed;
  } 
  else if (backward) {
    targetLeft = -maxSpeed;
    targetRight = -maxSpeed;
  } 
  else if (left) {
    targetLeft = -maxSpeed;  // Pivot Left: Left wheel rolls backward
    targetRight = maxSpeed;  // Right wheel rolls forward
  } 
  else if (right) {
    targetLeft = maxSpeed;   // Pivot Right: Left wheel rolls forward
    targetRight = -maxSpeed; // Right wheel rolls backward
  } 
  else {
    targetLeft = 0;          // No buttons pressed: coast to absolute stop
    targetRight = 0;
  }

  // --- Smooth Ramping Math Loop ---
  if (millis() - lastUpdateTime >= updateInterval) {
    lastUpdateTime = millis();

    // Adjust Left Wheel Speed towards its target setting
    if (currentSpeedLeft < targetLeft) {
      currentSpeedLeft += accelStep;
      if (currentSpeedLeft > targetLeft) currentSpeedLeft = targetLeft;
    } else if (currentSpeedLeft > targetLeft) {
      currentSpeedLeft -= decelStep;
      if (currentSpeedLeft < targetLeft) currentSpeedLeft = targetLeft;
    }

    // Adjust Right Wheel Speed towards its target setting
    if (currentSpeedRight < targetRight) {
      currentSpeedRight += accelStep;
      if (currentSpeedRight > targetRight) currentSpeedRight = targetRight;
    } else if (currentSpeedRight > targetRight) {
      currentSpeedRight -= decelStep;
      if (currentSpeedRight < targetRight) currentSpeedRight = targetRight;
    }

    // Push the ramped speed variables down to the low-level physical registers
    drive(currentSpeedLeft, currentSpeedRight);
  }
}

/**
 * Underlying Hardware Driver Block:
 * Direct translation layer between numerical integers and H-Bridge pin changes.
 */
void drive(int speed1, int speed2) {
  // --- Control Left Wheel (Motor 1) ---
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

  // --- Control Right Wheel (Motor 2) ---
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