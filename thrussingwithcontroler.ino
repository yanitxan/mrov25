#include <SPI.h>
#include <Ethernet.h>

// config
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC address
IPAddress ip(192, 168, 1, 50);                         // arduino's static IP address
EthernetServer server(8080);                           //  port 8080


#define PWM1 5   // Left motor speed control
#define DIR1 2   // Left motor direction control (reassigned)
#define PWM2 6   // Right motor speed control
#define DIR2 7   // Right motor direction control
#define PWM3 9   // Vertical up motor speed control
#define DIR3 8   // Vertical up motor direction control
#define PWM4 10  // Vertical down motor speed control
#define DIR4 11  // Vertical down motor direction control

void setup() {
  // disabble pin
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  
  // Initialize motor pins
  pinMode(PWM1, OUTPUT); pinMode(DIR1, OUTPUT);
  pinMode(PWM2, OUTPUT); pinMode(DIR2, OUTPUT);
  pinMode(PWM3, OUTPUT); pinMode(DIR3, OUTPUT);
  pinMode(PWM4, OUTPUT); pinMode(DIR4, OUTPUT);
  
  
  Serial.begin(9600);
  while (!Serial) { ; }
  
  // start server
  Serial.println("starting Ethernet...");
  Ethernet.begin(mac, ip);
  server.begin();
  delay(1000);
  
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  Serial.println("waiting for client connections...");
}

void loop() {
  // check if a client is available
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Client connected!");
    String command = "";
    unsigned long lastReceiveTime = millis();
    
    // keep the connection open as long as the client is connected
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // update the last received time whenever a character is received
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
              setMotor(PWM3, DIR3, upMotor);
              setMotor(PWM4, DIR4, downMotor);
              Serial.println("Motor commands workes.");
            } else {
              Serial.println("Failed to command!");
            }
          }
          command = ""; // clear the command
        } else {
          command += c;
        }
      }
      
      //disconnects after 10 seconds.
      if (millis() - lastReceiveTime > 10000) {
        Serial.println("No data received for 10 seconds. Disconnecting.");
        break;
      }
    }
    
    client.stop(); // close the connection
    Serial.println("Client disconnected.\n");
    delay(100); // pause before checking for new connections
  }
}

void setMotor(int pwmPin, int dirPin, int speed) {
  
  speed = constrain(speed, -255, 255);

  digitalWrite(dirPin, speed >= 0 ? HIGH : LOW);

  analogWrite(pwmPin, abs(speed));
}
