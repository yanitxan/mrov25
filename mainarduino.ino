#include <SPI.h>
#include <Ethernet.h>

// Config
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
IPAddress ip(192, 168, 1, 50);                         
EthernetServer server(8080);                         

#define PWM1 5   // Left motor speed control
#define DIR1 2   // Left motor direction control
#define PWM2 6   // Right motor speed control
#define DIR2 7   // Right motor direction control
#define PWM3 9   // Vertical up motor speed control
#define DIR3 8   // Vertical up motor direction control
#define PWM4 3   // Vertical down motor speed control
#define DIR4 11  // Vertical down motor direction control

void setup() {
  
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  
 
  pinMode(PWM1, OUTPUT); pinMode(DIR1, OUTPUT);
  pinMode(PWM2, OUTPUT); pinMode(DIR2, OUTPUT);
  pinMode(PWM3, OUTPUT); pinMode(DIR3, OUTPUT);
  pinMode(PWM4, OUTPUT); pinMode(DIR4, OUTPUT);
  
  Serial.begin(9600);
  while (!Serial) { ; }

  turnOffMotors();

  
  Serial.println("Starting Ethernet...");
  Ethernet.begin(mac, ip);
  server.begin();
  delay(1000);
  
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
  Serial.println("Waiting for client connections...");
}

void loop() {
 
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Client connected!");
    String command = "";
    unsigned long lastReceiveTime = millis();
    
   
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        lastReceiveTime = millis();

        if (c == '\n') {
          if (command.length() > 0) {
            Serial.print("Received: ");
            Serial.println(command);

            int leftMotor, rightMotor, upMotor, downMotor;
            int parsed = sscanf(command.c_str(), "L:%d,R:%d,U:%d,D:%d", 
                                &leftMotor, &rightMotor, &upMotor, &downMotor);
            if (parsed == 4) {
              setMotor(PWM1, DIR1, leftMotor);
              setMotor(PWM2, DIR2, rightMotor);
              setVerticalMotors(upMotor, downMotor); 
              Serial.println("Motor commands executed.");
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
      

      if (millis() - lastReceiveTime > 10000) {
        Serial.println("No data received for 10 seconds. Disconnecting.");
        turnOffMotors();
        break;
      }
    }
    
    client.stop();
    Serial.println("Client disconnected.\n");
    turnOffMotors();
    delay(100);
  }
}


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
  } 
  else if (downSpeed > 0 && upSpeed == 0) { 
 
    digitalWrite(DIR3, LOW);
    digitalWrite(DIR4, LOW);
    analogWrite(PWM3, downSpeed);
    analogWrite(PWM4, downSpeed);
  } 
  else { 

    analogWrite(PWM3, 0);
    analogWrite(PWM4, 0);
  }
}


void turnOffMotors() {
  setMotor(PWM1, DIR1, 0);
  setMotor(PWM2, DIR2, 0);
  setVerticalMotors(0, 0);
}
