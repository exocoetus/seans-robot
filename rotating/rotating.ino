// Rotating- Implementation Goal: Switch between R/C and autonomous and drive motor and servo
// TODO - Get hardware... Currently Pi3 and Arduino Nano. Read commands via Serial read
// VERY MUCH NOT READY TO GO


// Arduino pin numbers
const byte BLUE_PIN = A0;
const byte YELLOW_PIN = A2;
const byte GREEN_PIN = A3;

// Variables
double blue_val = -99999;
double yellow_val = -99999;
double green_val = -99999;


void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}

void loop() {
  // Check instructions from Pi (Mode, Drive data)
  blue_val = analogRead(BLUE_PIN);
  yellow_val = analogRead(YELLOW_PIN);
  green_val = analogRead(GREEN_PIN);
  Serial.print("Blue   - ");
  Serial.println(blue_val);
  Serial.print("Yellow - ");
  Serial.println(yellow_val);
  Serial.print("Green  - ");
  Serial.println(green_val);
  Serial.println();
  delay(500);
}
