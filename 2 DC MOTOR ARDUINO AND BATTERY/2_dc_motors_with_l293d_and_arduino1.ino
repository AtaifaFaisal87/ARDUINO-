/**
 * Use of 2 DC motor and a L293D
 */

// Motor 1
const int PIN_ACTIVATE_3_4 = 2;
const int PIN_INPUT_3 = 3;
const int PIN_INPUT_4 = 4;

// Motor 2
const int PIN_ACTIVATE_1_2 = 8;
const int PIN_INPUT_1 = 9;
const int PIN_INPUT_2 = 10;

/**
 * Initialize the pins of a motor
 */
void initMotor(int activationPin, int pin1, int pin2)
{
    pinMode(activationPin, OUTPUT);
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    // Enable the motor driver
    digitalWrite(activationPin, HIGH); 
}

/**
 * Run a motor either clockwise or counter-clockwise
 */
void runMotor(int pin1, int pin2, bool clockwise = true)
{
    if (clockwise) {
        // Run the motor clockwise
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
    } else {
        // Run the motor counter-clockwise
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
    }
}

// Main functions

void setup()
{ 
    initMotor(PIN_ACTIVATE_1_2, PIN_INPUT_1, PIN_INPUT_2);
    initMotor(PIN_ACTIVATE_3_4, PIN_INPUT_3, PIN_INPUT_4);
}

void loop()
{
    runMotor(PIN_INPUT_1, PIN_INPUT_2, true);
    runMotor(PIN_INPUT_3, PIN_INPUT_4, true);
    delay(2000);
    
    runMotor(PIN_INPUT_1, PIN_INPUT_2, false);
    runMotor(PIN_INPUT_3, PIN_INPUT_4, false);
    delay(2000);
}