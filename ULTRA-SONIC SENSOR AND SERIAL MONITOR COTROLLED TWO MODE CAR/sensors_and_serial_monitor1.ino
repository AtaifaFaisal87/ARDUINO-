// ================= MOTOR PINS =================
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
const int modeLED = A0;  // NEW: Orange LED connected to Analog Pin A0

// ================= TUNING VARIABLES =================
const int maxSpeed = 200;      
const int accelStep = 15;      
const int decelStep = 25;      
const int updateInterval = 20; 

int currentSpeedLeft = 0;      
int currentSpeedRight = 0;     
unsigned long lastUpdateTime = 0;

int targetLeft = 0;
int targetRight = 0;
int distance = 0;

// ================= MODE & SYSTEM VARIABLES =================
bool autonomousMode = false; // false = Manual Mode | true = Autonomous Mode

char manualDirection = 'S'; 
int targetSpeedSetting = 0; 
int lastTurnDirection = 0;  // 0 = Left, 1 = Right

// ================= SETUP =================
void setup() {
  // Motor controller pins
  pinMode(motor1_enable, OUTPUT);
  pinMode(motor1_in1, OUTPUT);
  pinMode(motor1_in2, OUTPUT);
  pinMode(motor2_enable, OUTPUT);
  pinMode(motor2_in3, OUTPUT);
  pinMode(motor2_in4, OUTPUT);
  
  // Peripherals
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(modeLED, OUTPUT); // Initialize the new Mode LED pin

  Serial.begin(9600);
  Serial.println("==================================================");
  Serial.println("--- DUAL-MODE CONTROLLER WITH MODE INDICATOR ---");
  Serial.println("Orange LED OFF = Manual Mode | Orange LED ON = Auto Mode");
  Serial.println("==================================================");
}

// ================= ULTRASONIC SENSOR FUNCTION =================
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); 

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

  // --- 1. PARSE INCOMING SERIAL COMMANDS ---
  if (Serial.available() > 0) {
    char inputChar = Serial.read();
    
    if (inputChar >= 'a' && inputChar <= 'z') {
      inputChar = inputChar - 32; 
    }

    // Toggle Modes and handle the Indicator LED state
    if (inputChar == 'A') {
      autonomousMode = true;
      digitalWrite(modeLED, HIGH); // Turn ON Orange LED for Auto Mode
      emergencyStop();
      Serial.println("SYSTEM STATE -> Switched to [AUTONOMOUS MODE]");
    } 
    else if (inputChar == 'M') {
      autonomousMode = false;
      digitalWrite(modeLED, LOW);  // Turn OFF Orange LED for Manual Mode
      manualDirection = 'S';
      emergencyStop();
      Serial.println("SYSTEM STATE -> Switched to [MANUAL MODE] (Stopped)");
    }
    // Process Manual Driving Commands
    else if (!autonomousMode && (inputChar == 'F' || inputChar == 'B' || inputChar == 'L' || inputChar == 'R' || inputChar == 'S')) {
      manualDirection = inputChar;
      if (manualDirection == 'S') {
        targetSpeedSetting = 0;
      } else {
        int parsedSpeed = Serial.parseInt();
        targetSpeedSetting = (parsedSpeed == 0) ? maxSpeed : constrain(parsedSpeed, 0, maxSpeed);
      }
    }
  }

  // --- 2. EXECUTE LOGIC BASED ON ACTIVE MODE ---
  if (autonomousMode) {
    // ==========================================
    //            AUTONOMOUS MODE ACTIVE
    // ==========================================
    if (distance > 15) {
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
      emergencyStop();
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      tone(buzzer, 1000);
      delay(300);
      noTone(buzzer);

      setTargetTargets(-maxSpeed, -maxSpeed); 
      executeRamping();
      delay(400); 
      emergencyStop();
      delay(100);

      if (lastTurnDirection == 0) {
        setTargetTargets(maxSpeed, -maxSpeed); 
        executeRamping();
        delay(600);
        lastTurnDirection = 1;
      } else {
        setTargetTargets(-maxSpeed, maxSpeed); 
        executeRamping();
        delay(600);
        lastTurnDirection = 0;
      }
      emergencyStop();
      delay(150);
    }
  } 
  else {
    // ==========================================
    //              MANUAL MODE ACTIVE
    // ==========================================
    if (distance <= 15 && manualDirection == 'F') {
      manualDirection = 'S'; 
      targetSpeedSetting = 0;
      emergencyStop();
      
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      tone(buzzer, 1000, 200);
      Serial.println("⚠️ MANUAL OVERRIDE: Proximity Brake Activated!");
    } 
    else if (distance <= 15) {
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
    } 
    else {
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, HIGH);
    }

    switch (manualDirection) {
      case 'F': setTargetTargets(targetSpeedSetting, targetSpeedSetting); break;
      case 'B': setTargetTargets(-targetSpeedSetting, -targetSpeedSetting); break;
      case 'L': setTargetTargets(-targetSpeedSetting, targetSpeedSetting); break;
      case 'R': setTargetTargets(targetSpeedSetting, -targetSpeedSetting); break;
      case 'S':
      default:  setTargetTargets(0, 0); break;
    }

    if (millis() - lastUpdateTime >= updateInterval) {
      lastUpdateTime = millis();
      
      if (currentSpeedLeft < targetLeft) currentSpeedLeft += accelStep;
      else if (currentSpeedLeft > targetLeft) currentSpeedLeft -= decelStep;

      if (currentSpeedRight < targetRight) currentSpeedRight += accelStep;
      else if (currentSpeedRight > targetRight) currentSpeedRight -= decelStep;

      drive(currentSpeedLeft, currentSpeedRight);
    }
  }
  delay(10);
}

// ================= HARDWARE MOTOR DRIVER LAYER =================
void drive(int speed1, int speed2) {
  if (speed1 > 0) {
    digitalWrite(motor1_in1, HIGH);   digitalWrite(motor1_in2, LOW);
    analogWrite(motor1_enable, speed1);
  } else if (speed1 < 0) {
    digitalWrite(motor1_in1, LOW);    digitalWrite(motor1_in2, HIGH);
    analogWrite(motor1_enable, abs(speed1)); 
  } else {
    digitalWrite(motor1_in1, LOW);    digitalWrite(motor1_in2, LOW);
    analogWrite(motor1_enable, 0);
  }

  if (speed2 > 0) {
    digitalWrite(motor2_in3, HIGH);   digitalWrite(motor2_in4, LOW);
    analogWrite(motor2_enable, speed2);
  } else if (speed2 < 0) {
    digitalWrite(motor2_in3, LOW);    digitalWrite(motor2_in4, HIGH);
    analogWrite(motor2_enable, abs(speed2));
  } else {
    digitalWrite(motor2_in3, LOW);    digitalWrite(motor2_in4, LOW);
    analogWrite(motor2_enable, 0);
  }
}