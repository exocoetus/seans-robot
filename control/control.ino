// Control - Implementation Goal: Switch between R/C and autonomous and drive motor and servo
// TODO - Get hardware... Currently Pi3 and Arduino Nano. Read commands via Serial read
// VERY MUCH NOT READY TO GO


// Arduino pin numbers
const byte DIAGNOSTIC_LED_PIN = 12; // digital pin on diagnostic LED
const byte STEERING_PIN = 3; // digital pin for steering control - verification needed
const byte THROTTLE_PIN = 4; // digital pin for throttle control - verification needed

// Variables
int diagnosticLEDState = LOW;
int autonomous = LOW;
int incomingByte = 0; // for incoming serial data

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}

void loop() {
  // Check instructions from Pi (Mode, Drive data)
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 0) {
      autonomous = LOW;
    }
    else if (incomingByte == 1) {
      autonomous = HIGH;
    }
    else {
      autonomous = LOW;
    }
  }
  // Output instructions to actuators
}
