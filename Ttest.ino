
// pwm on pin 5, direction on pin 4

#define pwm_pin 5
#define dir_pin 4

void setup() {
  Serial.begin(9600);
  pinMode(pwm_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  Serial.println("thruster test starting...");
}

void loop() {
  // test thruster forward
  Serial.println("testing thruster forward...");
  set_thruster(255);
  delay(1000);

  // test thruster reverse
  Serial.println("testing thruster reverse...");
  set_thruster(-255);
  delay(1000);

  // turn thruster off
  Serial.println("thruster off...");
  set_thruster(0);
  delay(2000);

  while (1); // stop loop
}

void set_thruster(int speed) {
  speed = constrain(speed, -255, 255);
  digitalWrite(dir_pin, speed >= 0 ? HIGH : LOW);
  analogWrite(pwm_pin, abs(speed));
}
