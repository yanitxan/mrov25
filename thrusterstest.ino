#define PWM1 5   // Left motor speed control
#define DIR1 4   // Left motor direction control
#define PWM2 6   // Right motor speed control
#define DIR2 7   // Right motor direction control
#define PWM3 9   // Vertical up motor speed control
#define DIR3 8   // Vertical up motor direction control
#define PWM4 10  // Vertical down motor speed control
#define DIR4 11  // Vertical down motor direction control

void setup() {
  Serial.begin(9600);
  
  // Set all motor pins as outputs
  int motor_pins[] = {PWM1, DIR1, PWM2, DIR2, PWM3, DIR3, PWM4, DIR4};
  for (int i = 0; i < 8; i++) {
    pinMode(motor_pins[i], OUTPUT);
  }

  Serial.println("Motor test starting...");
}

void loop() {
  // Test each motor forward and reverse
  Serial.println("Testing left motor forward...");
  set_motor(1, 255);
  delay(1000);
  Serial.println("Testing left motor reverse...");
  set_motor(1, -255);
  delay(1000);

  Serial.println("Testing right motor forward...");
  set_motor(2, 255);
  delay(1000);
  Serial.println("Testing right motor reverse...");
  set_motor(2, -255);
  delay(1000);

  Serial.println("Testing vertical up motor...");
  set_motor(3, 255);
  delay(1000);
  Serial.println("Testing vertical up motor reverse...");
  set_motor(3, -255);
  delay(1000);

  Serial.println("Testing vertical down motor...");
  set_motor(4, 255);
  delay(1000);
  Serial.println("Testing vertical down motor reverse...");
  set_motor(4, -255);
  delay(1000);

  Serial.println("All motors off...");
  set_motor(1, 0);
  set_motor(2, 0);
  set_motor(3, 0);
  set_motor(4, 0);
  delay(2000);

  while (1); // Stop loop after test
}

void set_motor(int motor, int speed) {
  speed = constrain(speed, -255, 255);
  
  int pwm_pin, dir_pin;
  
  switch (motor) {
    case 1: pwm_pin = PWM1; dir_pin = DIR1; break;  // Left motor
    case 2: pwm_pin = PWM2; dir_pin = DIR2; break;  // Right motor
    case 3: pwm_pin = PWM3; dir_pin = DIR3; break;  // Vertical up motor
    case 4: pwm_pin = PWM4; dir_pin = DIR4; break;  // Vertical down motor
    default: return; // Invalid motor number
  }
  
  // **Invert direction logic for vertical motors (motor 3 & 4)**
  if (motor == 3 || motor == 4) {
    digitalWrite(dir_pin, speed >= 0 ? LOW : HIGH);  // Reverse logic for up/down
  } else {
    digitalWrite(dir_pin, speed >= 0 ? HIGH : LOW);  // Normal logic for left/right
  }
  
  analogWrite(pwm_pin, abs(speed));
}
