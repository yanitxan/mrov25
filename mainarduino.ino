#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

// ETHERNET CONFIG
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 50);
EthernetServer server(8080);

// MOTOR PINS
#define PWM1 5   // Left motor speed control
#define DIR1 2   // Left motor direction control
#define PWM2 6   // Right motor speed control
#define DIR2 7   // Right motor direction control
#define PWM3 9   // Vertical up motor speed control
#define DIR3 8   // Vertical up motor direction control
#define PWM4 3   // Vertical down motor speed control
#define DIR4 11  // Vertical down motor direction control

// SERVO
#define CLAW_PIN 5 
Servo clawServo;
volatile int targetClawPos = 90; // default clawpos

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }  // waits for serial connection
  
  Serial.println("Starting Ethernet...");
  Ethernet.begin(mac, ip);
  server.begin();
  delay(1000);
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());

  
  clawServo.attach(CLAW_PIN);
  clawServo.write(targetClawPos);
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Client connected!");
    String command = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (command.length() > 0) {
            Serial.print("Received: ");
            Serial.println(command);
            int leftMotor, rightMotor, upMotor, downMotor, clawPos;
            int parsed = sscanf(command.c_str(), "L:%d,R:%d,U:%d,D:%d,Claw:%d",
                                &leftMotor, &rightMotor, &upMotor, &downMotor, &clawPos);
            if (parsed == 5) {
              // process motor commands
              setMotor(PWM1, DIR1, leftMotor);
              setMotor(PWM2, DIR2, rightMotor);
              setVerticalMotors(upMotor, downMotor);
              
              // moves claw
              moveClaw(clawPos);
              Serial.println("Motor and claw commands executed.");
            } else {
              Serial.println("Failed to parse command!");
              turnOffMotors();
            }
          }
          command = "";
        } else {
          command += c;
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected.");
    turnOffMotors();
    delay(100);
  }
}


void moveClaw(int pos) {
  clawServo.attach(CLAW_PIN);  // reattach before moving
  clawServo.write(pos);
  delay(20);  // allows time for movement
  clawServo.detach();  
}

// motor control functions
void setMotor(int pwmPin, int dirPin, int speed) {
  speed = constrain(speed, -255, 255);
  digitalWrite(dirPin, speed >= 0 ? HIGH : LOW);
  analogWrite(pwmPin, abs(speed));
}

void setVerticalMotors(int upSpeed, int downSpeed) {
  upSpeed = constrain(upSpeed, 0, 255);
  downSpeed = constrain(downSpeed, 0, 255);
  if (upSpeed > 0 && downSpeed == 0) {
    digitalWrite(DIR3, HIGH);
    digitalWrite(DIR4, HIGH);
    analogWrite(PWM3, upSpeed);
    analogWrite(PWM4, upSpeed);
  } else if (downSpeed > 0 && upSpeed == 0) {
    digitalWrite(DIR3, LOW);
    digitalWrite(DIR4, LOW);
    analogWrite(PWM3, downSpeed);
    analogWrite(PWM4, downSpeed);
  } else {
    analogWrite(PWM3, 0);
    analogWrite(PWM4, 0);
  }
}

void turnOffMotors() {
  setMotor(PWM1, DIR1, 0);
  setMotor(PWM2, DIR2, 0);
  setVerticalMotors(0, 0);
}
