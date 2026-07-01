// --- Motor Pin Definitions ---
// Motor 1 (Left Wheel)
const int motor1_enable = 11; 
const int motor1_in1    = 12; 
const int motor1_in2    = 13; 

// Motor 2 (Right Wheel)
const int motor2_enable = 9;  
const int motor2_in3    = 7;  
const int motor2_in4    = 8;  

// --- Tuning Variables ---
const int maxSpeed = 255;      // Default top speed limit if no speed number is typed
const int accelStep = 10;      // Speed increase rate per 20ms step
const int decelStep = 15;      // Braking speed rate when slowing down or stopping
const int updateInterval = 20; // Time interval (in ms) to update speed calculation

int currentSpeedLeft = 0;      // Dynamically tracks left wheel speed
int currentSpeedRight = 0;     // Dynamically tracks right wheel speed
unsigned long lastUpdateTime = 0;

// --- Serial Command Registers ---
char targetDirection = 'S';    // Tracks desired movement direction ('F', 'B', 'L', 'R', 'S')
int targetSpeedSetting = 0;    // Tracks the requested target speed (0 to 255)

void setup() {
  // Initialize motor controller interface pins
  pinMode(motor1_enable, OUTPUT);
  pinMode(motor1_in1, OUTPUT);
  pinMode(motor1_in2, OUTPUT);
  
  pinMode(motor2_enable, OUTPUT);
  pinMode(motor2_in3, OUTPUT);
  pinMode(motor2_in4, OUTPUT);
  
  // Start serial communications at 9600 baud rate
  Serial.begin(9600);
  Serial.println("--- Serial Motor Control Active ---");
  Serial.println("Format: [Direction Code][Speed 0-255]");
  Serial.println("Examples: F255 (Forward Max), B150 (Back Slow), L (Left Max), S (Stop)");
  Serial.println("---------------------------------");
}

void loop() {
  // Check if a brand new text command has been typed into the monitor
  if (Serial.available() > 0) {
    
    // Read the first character as the core direction code
    char inputChar = Serial.read();
    
    // Convert lower-case letters to upper-case automatically
    if (inputChar >= 'a' && inputChar <= 'z') {
      inputChar = inputChar - 32; 
    }

    // Process only valid command letters
    if (inputChar == 'F' || inputChar == 'B' || inputChar == 'L' || inputChar == 'R' || inputChar == 'S') {
      targetDirection = inputChar;
      
      if (targetDirection == 'S') {
        targetSpeedSetting = 0; // Force target setting straight to 0 if stopping
      } else {
        // Parse any numbers immediately trailing the letter code
        int parsedSpeed = Serial.parseInt();
        
        // If you only type the letter (e.g., just 'F'), default to full max power
        if (parsedSpeed == 0) {
          targetSpeedSetting = maxSpeed; 
        } else {
          targetSpeedSetting = constrain(parsedSpeed, 0, maxSpeed);
        }
      }
      
      // Send quick debug logs back to the terminal screen
      Serial.print("Command Received -> Direction: ");
      Serial.print(targetDirection);
      Serial.print(" | Target Speed: ");
      Serial.println(targetSpeedSetting);
    }
  }

  // --- Map Direction and Speed Inputs to Immediate Mathematical Target States ---
  int targetLeft = 0;
  int targetRight = 0;

  switch (targetDirection) {
    case 'F': // Drive Straight Forward
      targetLeft = targetSpeedSetting;
      targetRight = targetSpeedSetting;
      break;
      
    case 'B': // Drive Straight Backward
      targetLeft = -targetSpeedSetting;
      targetRight = -targetSpeedSetting;
      break;
      
    case 'L': // Pivot Left on its center axis
      targetLeft = -targetSpeedSetting;
      targetRight = targetSpeedSetting;
      break;
      
    case 'R': // Pivot Right on its center axis
      targetLeft = targetSpeedSetting;
      targetRight = -targetSpeedSetting;
      break;
      
    case 'S': // Bring targets to complete stop
    default:
      targetLeft = 0;
      targetRight = 0;
      break;
  }

  // --- Smooth Ramping Math Loop (Runs every 20ms) ---
  if (millis() - lastUpdateTime >= updateInterval) {
    lastUpdateTime = millis();

    // Adjust Left Wheel Speed towards its dynamic target setting
    if (currentSpeedLeft < targetLeft) {
      currentSpeedLeft += accelStep;
      if (currentSpeedLeft > targetLeft) currentSpeedLeft = targetLeft;
    } else if (currentSpeedLeft > targetLeft) {
      currentSpeedLeft -= decelStep;
      if (currentSpeedLeft < targetLeft) currentSpeedLeft = targetLeft;
    }

    // Adjust Right Wheel Speed towards its dynamic target setting
    if (currentSpeedRight < targetRight) {
      currentSpeedRight += accelStep;
      if (currentSpeedRight > targetRight) currentSpeedRight = targetRight;
    } else if (currentSpeedRight > targetRight) {
      currentSpeedRight -= decelStep;
      if (currentSpeedRight < targetRight) currentSpeedRight = targetRight;
    }

    // Push the ramped speed variables down to the physical L293D registers
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