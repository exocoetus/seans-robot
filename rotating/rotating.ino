// Hasbro R2D2 Quadrature encoder based dome actuator
// https://cdn.sparkfun.com/datasheets/Robotics/How%20to%20use%20a%20quadrature%20encoder.pdf
// https://www.dynapar.com/knowledge/how-to-calculate-encoder-resolution
// https://forum.arduino.cc/index.php?topic=606542.0

// PWR - Brown
// GND - Purple
// Internal pull-up resistors appear well tuned for this. 
// Tried 1M Ohm resistor for pull-down, got minimal success.
// Sensors from left to right 1, 2, center

// Low pass filter on motor recommended to reduce whine
// I ended up just raising pwm frequency by setting timer 0 divisor to 1
// Will this have unintended consequences? That is a problem for future Sean

// Arduino pin numbers
const byte QUAD1_PIN = A0; // Red Wire
const byte QUAD2_PIN = A1; // Grey Wire
const byte CENT_PIN = A2; // Blue Wire

const byte EN1 = 5; // Enable 1
const byte IN1 = 3; // Input 1
const byte IN2 = 4; // Input 2

const int QEM[16] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};

const int baudrate = 57600;

// Variables
byte quad1_val = 0;
byte quad2_val = 0;
byte cent_val = 0;
int last_state = 0b00;
int curr_state = 0b00;
int dir = 2;
int pos = 0;
bool calibrated = false;


void setup() {
  Serial.begin(57600); // set baud rate to 57600, slower causes issues
  // Set pinmodes
  pinMode(QUAD1_PIN, INPUT_PULLUP);
  pinMode(QUAD2_PIN, INPUT_PULLUP);
  pinMode(CENT_PIN, INPUT_PULLUP);
  pinMode(EN1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  TCCR0B = TCCR0B & B11111000 | B00000001; // set timer 0 divisor to 1

  // No motor movement
  digitalWrite(IN1, LOW); // LOW, HIGH => CCW
  digitalWrite(IN2, LOW); // HIGH, LOW => CW
  analogWrite(EN1, 0);

  pos = -9999;
  calibrate();
}

void loop() {
  updatePosition();
  Serial.println(pos);
  rotate(0);
  for (int i = 0; i < 8000; i++) {
    updatePosition();
  }
}

void updatePosition() {
  quad1_val = digitalRead(QUAD1_PIN);
  quad2_val = digitalRead(QUAD2_PIN);
  if (digitalRead(CENT_PIN)) {
    pos = 0;
    calibrated = true;
  }
  
  last_state = curr_state;
  curr_state = (quad1_val << 1) + quad2_val;
  pos = pos + QEM[(last_state * 4) + curr_state];
}

// Find the center position
void calibrate() {
  calibrated = false;
  bool ccw = false;
  while (!calibrated) {
    // Check if rotation occured
    if (!rotateCheck(ccw)) {
      Serial.println("OOF");
      ccw = !ccw;
    }
  }
  freeze();
}

void freeze() {
  analogWrite(EN1, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

// Rotate between -255 (CCW) and 255 (CW) speed
void rotate(int intensity) {
  freeze();
  if (intensity < 0) {
    digitalWrite(IN2, HIGH);
  } else if (intensity > 0) {
    digitalWrite(IN1, HIGH);
  } else {
    freeze();
  }
  analogWrite(EN1, abs(intensity));
}

// Check CW rotation otherwise CCW
bool rotateCheck(bool ccw) {
  updatePosition();
  int last_pos = pos;
  if (ccw) {
    rotate(-70);
  } else {
    rotate(70);
  }
  for (int i = 0; i < 6000; i++) {
    updatePosition();
  }
  if (abs(pos - last_pos) < 2) {
    return false;
  }
  return true;
}
