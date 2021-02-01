// Hasbro R2D2 Quadrature encoder based dome actuator

// PWR - Brown
// GND - Purple
// Internal pull-up resistors appear well tuned for this. Tried 1M Ohm resistor for pull-down, got minimal success.
// Sensors from left to right 1, 2, center

// Arduino pin numbers
const byte QUAD1_PIN = A0; // Red Wire
const byte QUAD2_PIN = A2; // Grey Wire
const byte CENT_PIN = A4; // Blue Wire

// Variables
byte quad1_val = 0;
byte quad2_val = 0;
byte cent_val = 0;
byte state = 0b00;
bool centered = false;


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
  state = (quad1_val << 1) + quad2_val;
  Serial.println(state);
  delay(200);
}
