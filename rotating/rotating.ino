// Hasbro R2D2 Quadrature encoder based dome actuator

// PWR - Brown
// GND - Purple
// Internal pull-up resistors appear well tuned for this. Tried 1M Ohm resistor for pull-down, got minimal success.

// Arduino pin numbers
const byte QUAD1_PIN = A0; // Red Wire
const byte QUAD2_PIN = A2; // Grey Wire
const byte CENT_PIN = A4; // Blue Wire

// Variables
double quad1_val = -99999;
double quad2_val = -99999;
double cent_val = -99999;


void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(QUAD1_PIN, INPUT_PULLUP);
  pinMode(QUAD2_PIN, INPUT_PULLUP);
  pinMode(CENT_PIN, INPUT_PULLUP);
}

void loop() {
  // Check instructions from Pi (Mode, Drive data)
  quad1_val = digitalRead(QUAD1_PIN);
  quad2_val = digitalRead(QUAD2_PIN);
  cent_val = digitalRead(CENT_PIN);
  Serial.print("Quad 1  - ");
  Serial.println(quad1_val);
  Serial.print("Quad 2  - ");
  Serial.println(quad2_val);
  Serial.print("Center  - ");
  Serial.println(cent_val);
  Serial.println();
  delay(200);
}
