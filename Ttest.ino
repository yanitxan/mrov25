// Define Motor Driver Pins
#define PWM1 5   // Left Thruster speed control
#define DIR1 4   // Left Thruster direction control
#define PWM2 6   // Right Thruster speed control
#define DIR2 7   // Right Thruster direction control
#define PWM3 9   // Up Thruster speed control
#define DIR3 8   // Up Thruster direction control
#define PWM4 10  // Down Thruster speed control
#define DIR4 11  // Down Thruster direction control

// Function to set motor speed and direction
// Speed range: -255 to 255 (positive for forward, negative for reverse)
void setMotor(int pwmPin, int dirPin, int speed) {
  speed = constrain(speed, -255, 255);
  digitalWrite(dirPin, speed >= 0 ? HIGH : LOW);
  analogWrite(pwmPin, abs(speed));
}

void setup() {
  Serial.begin(9600);
  
  // Initialize motor control pins as outputs
  pinMode(PWM1, OUTPUT); pinMode(DIR1, OUTPUT);
  pinMode(PWM2, OUTPUT); pinMode(DIR2, OUTPUT);
  pinMode(PWM3, OUTPUT); pinMode(DIR3, OUTPUT);
  pinMode(PWM4, OUTPUT); pinMode(DIR4, OUTPUT);
  
  Serial.println("Thruster Test Started");
}

void loop() {
  // Test Left Thruster
  Serial.println("Testing Left Thruster Forward");
  setMotor(PWM1, DIR1, 255);  // Full forward
  delay(1000);
  Serial.println("Testing Left Thruster Reverse");
  setMotor(PWM1, DIR1, -255); // Full reverse
  delay(1000);
  setMotor(PWM1, DIR1, 0);    // Off
  delay(500);

  // Test Right Thruster
  Serial.println("Testing Right Thruster Forward");
  setMotor(PWM2, DIR2, 255);
  delay(1000);
  Serial.println("Testing Right Thruster Reverse");
  setMotor(PWM2, DIR2, -255);
  delay(1000);
  setMotor(PWM2, DIR2, 0);
  delay(500);

  // Test Up Thruster
  Serial.println("Testing Up Thruster Forward");
  setMotor(PWM3, DIR3, 255);
  delay(1000);
  Serial.println("Testing Up Thruster Reverse");
  setMotor(PWM3, DIR3, -255);
  delay(1000);
  setMotor(PWM3, DIR3, 0);
  delay(500);

  // Test Down Thruster
  Serial.println("Testing Down Thruster Forward");
  setMotor(PWM4, DIR4, 255);
  delay(1000);
  Serial.println("Testing Down Thruster Reverse");
  setMotor(PWM4, DIR4, -255);
  delay(1000);
  setMotor(PWM4, DIR4, 0);
  delay(500);

  // Cycle complete; repeat the test
}
