// Hasbro R2D2 Quadrature encoder based dome actuator
// https://cdn.sparkfun.com/datasheets/Robotics/How%20to%20use%20a%20quadrature%20encoder.pdf
// https://www.dynapar.com/knowledge/how-to-calculate-encoder-resolution

// PWR - Brown
// GND - Purple
// Internal pull-up resistors appear well tuned for this. 
// Tried 1M Ohm resistor for pull-down, got minimal success.
// Sensors from left to right 1, 2, center

// Arduino pin numbers
const byte QUAD1_PIN = A0; // Red Wire
const byte QUAD2_PIN = A1; // Grey Wire
const byte CENT_PIN = A2; // Blue Wire

const byte EN1 = 5; // Enable 1
const byte IN1 = 3; // Input 1
const byte IN2 = 4; // Input 2

const int QEM[16] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};


// Variables
byte quad1_val = 0;
byte quad2_val = 0;
byte cent_val = 0;
int last_state = 0b00;
int curr_state = 0b00;
int dir = 2;
int pos = 0;


void setup() {
  Serial.begin(57600); // set baud rate to 57600, slower causes issues
  // Set pinmodes
  pinMode(QUAD1_PIN, INPUT_PULLUP);
  pinMode(QUAD2_PIN, INPUT_PULLUP);
  pinMode(CENT_PIN, INPUT_PULLUP);
  pinMode(EN1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // No motor movement
  digitalWrite(IN1, LOW); // LOW, HIGH => CCW
  digitalWrite(IN2, LOW); // HIGH, LOW => CW
  analogWrite(EN1, 0);

  pos = -9999;
  calibrate();
  Serial.println("Calibrated");
}

void loop() {
  updatePosition();
  Serial.println(pos);
}

void updatePosition() {
  quad1_val = digitalRead(QUAD1_PIN);
  quad2_val = digitalRead(QUAD2_PIN);
  if (digitalRead(CENT_PIN)) {
    pos = 0;
  }
  
  last_state = curr_state;
  curr_state = (quad1_val << 1) + quad2_val;
  pos = pos + QEM[(last_state * 4) + curr_state];
}

// Find the center position, uninformed
void calibrate() {
  int num = 180;
  int factor = 1;
  while (pos != 0) {
    for (int i = 0; i < 5000; i++) {
      updatePosition();
      rotate(num);
    }
    num * -1;
  }
}

void stop() {
  analogWrite(EN1, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

// Rotate between -255 (CCW) and 255 (CW) speed
void rotate(int intensity) {
  stop();
  if (intensity < 0) {
    digitalWrite(IN2, HIGH);
  } else if (intensity > 0) {
    digitalWrite(IN1, HIGH);
  } else {
    stop();
  }
  analogWrite(EN1, intensity);
}
