// Hasbro R2D2 Quadrature encoder based dome actuator
// https://cdn.sparkfun.com/datasheets/Robotics/How%20to%20use%20a%20quadrature%20encoder.pdf
// https://www.dynapar.com/knowledge/how-to-calculate-encoder-resolution

// PWR - Brown
// GND - Purple
// Internal pull-up resistors appear well tuned for this. Tried 1M Ohm resistor for pull-down, got minimal success.
// Sensors from left to right 1, 2, center

// Arduino pin numbers
const byte QUAD1_PIN = A0; // Red Wire
const byte QUAD2_PIN = A1; // Grey Wire
const byte CENT_PIN = A2; // Blue Wire
const int QEM[16] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};

// Variables
byte quad1_val = 0;
byte quad2_val = 0;
byte cent_val = 0;
int last_state = 0b00;
int curr_state = 0b00;
int dir = 2;
bool centered = false;
int count = 0;


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

  if (cent_val) {
    count = 0;
  }
  //Serial.print("Quad 1  - ");
  //Serial.println(quad1_val);
  //Serial.print("Quad 2  - ");
  //Serial.println(quad2_val);
  //Serial.print("Center  - ");
  //Serial.println(cent_val);
  //Serial.println();
  last_state = curr_state;
  curr_state = (quad1_val << 1) + quad2_val;
  count = count + QEM[(last_state * 4) + curr_state];
  //Serial.println(QEM[(last_state * 4) + curr_state]);
  Serial.println(count);
}
