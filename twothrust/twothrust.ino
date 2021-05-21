// Low pass filter on motor recommended to reduce whine
// I ended up just raising pwm frequency by setting timer 0 divisor to 1
// Will this have unintended consequences? That is a problem for future Sean

// Arduino pin numbers

// Port thruster
const byte ENA = 5; // Enable A
const byte IN1 = 3; // Input 1
const byte IN2 = 4; // Input 2

// Starboard thruster
const byte ENB = 6; // Enable B
const byte IN3 = 7; // Input 3
const byte IN4 = 8; // Input 4

// Transmitter
const byte THR = A2;
const byte RUD = A1;


const int baudrate = 19200;

// Variables
float throttle = 0.0;
float rudder = 0.0;
float portOut = 0.0;
float starOut = 0.0;


void setup() {
  Serial.begin(19200); // set baud rate to 57600, slower causes issues
  // Set pinmodes
  //pinMode(QUAD1_PIN, INPUT_PULLUP);
  //pinMode(QUAD2_PIN, INPUT_PULLUP);
  //pinMode(CENT_PIN, INPUT_PULLUP);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  TCCR0B = TCCR0B & B11111000 | B00000001; // set timer 0 divisor to 1

  digitalWrite(IN1, HIGH); // LOW, HIGH => CCW
  digitalWrite(IN2, LOW); // HIGH, LOW => CW

  digitalWrite(IN3, LOW); // LOW, HIGH => CCW
  digitalWrite(IN4, HIGH); // HIGH, LOW => CW
  
  // No motor movement
  analogWrite(ENB, 0);
  analogWrite(ENA, 0);

}

void loop() {
  // Read receiver
  float throttleIn = pulseIn(THR, HIGH, 25000);
  float rudderIn = pulseIn(RUD, HIGH, 25000);

  if (rudderIn < 1) {
    freeze();
  } else {
    throttle = map(throttleIn, 870, 1090, -255, 255);
    rudder = map(rudderIn, 780, 1080, -255, 255);
    
    // Process thruster values
    if (throttle > 0) {
      throttle = min(throttle, 255);
    } else {
      throttle = max(throttle, -255);
    }
    if (rudder > 0) {
      rudder = min(rudder, 255);
    } else {
      rudder = max(rudder, -255);
    }
  
//    Serial.print(throttle);
//    Serial.print(" <- THR   RUD -> ");
//    Serial.println(rudder);

    portOut = (portOut + throttle - rudder) / 2.0;
    starOut = (starOut + throttle + rudder) / 2.0;

    Serial.print(portOut);
    Serial.print(" <- PRT   STR -> ");
    Serial.println(starOut);
  
    // Thrust
    if (portOut > 25) {
      digitalWrite(IN1, HIGH); // LOW, HIGH => CCW
      digitalWrite(IN2, LOW); // HIGH, LOW => CW
    } else if (portOut < -25) {
      digitalWrite(IN1, LOW); // LOW, HIGH => CCW
      digitalWrite(IN2, HIGH); // HIGH, LOW => CW
       
    } else {
      digitalWrite(IN1, LOW); // LOW, HIGH => CCW
      digitalWrite(IN2, LOW); // HIGH, LOW => CW 
    }
    
    if (starOut > 25) {
      digitalWrite(IN3, LOW); // LOW, HIGH => CCW
      digitalWrite(IN4, HIGH); // HIGH, LOW => CW
    } else if (starOut < -25) {
      digitalWrite(IN3, HIGH); // LOW, HIGH => CCW
      digitalWrite(IN4, LOW); // HIGH, LOW => CW    
    } else {
      digitalWrite(IN3, LOW); // LOW, HIGH => CCW
      digitalWrite(IN4, LOW); // HIGH, LOW => CW    
    }
    
    analogWrite(ENA, max(abs(portOut), 255));
    analogWrite(ENB, max(abs(starOut), 255));

  }


}


void freeze() {
  analogWrite(ENA, 0);
  //digitalWrite(IN1, LOW);
  //digitalWrite(IN2, LOW);
  analogWrite(ENB, 0);
  //digitalWrite(IN3, LOW);
  //digitalWrite(IN4, LOW);
}
