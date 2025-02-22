#include <SPI.h>
#include <Ethernet.h>

// configuration
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //  MAC address dont forget to change this stuff
IPAddress ip(192, 168, 1, 50);                         // arduino's static IP address
EthernetServer server(8080);                           // server listening on port 8080

// motor driver pins 
#define PWM1 5   // left motor speed control
#define DIR1 4   // left ,otor direction control
#define PWM2 6   // right motor speed control
#define DIR2 7   // right =motor direction control
#define PWM3 9   // vertical up Motor speed control
#define DIR3 8   // vertical up Motor direction control
#define PWM4 10  // vertical down Motor speed control
#define DIR4 11  // vertical down Motor direction control

void setup() {
  Ethernet.begin(mac, ip);
  server.begin();  // start the server
  Serial.begin(9600);
  
  // set motor pins as outputs
  pinMode(PWM1, OUTPUT); pinMode(DIR1, OUTPUT);
  pinMode(PWM2, OUTPUT); pinMode(DIR2, OUTPUT);
  pinMode(PWM3, OUTPUT); pinMode(DIR3, OUTPUT);
  pinMode(PWM4, OUTPUT); pinMode(DIR4, OUTPUT);
}

void loop() {
  // check for an incoming connection
  EthernetClient client = server.available();
  if (client) {
    String command = "";
    // read incoming characters until newline is encountered
    while (client.available()) {
      char c = client.read();
      if (c == '\n') break;
      command += c;
    }
    
    // if a command was received process it
    if (command.length() > 0) {
      int leftMotor, rightMotor, upMotor, downMotor;
      // command format: "L:150,R:-150,U:255,D:0"
      sscanf(command.c_str(), "L:%d,R:%d,U:%d,D:%d", &leftMotor, &rightMotor, &upMotor, &downMotor);
      
      //  for debug prints values
      Serial.print("L: "); Serial.print(leftMotor);
      Serial.print(" R: "); Serial.print(rightMotor);
      Serial.print(" U: "); Serial.print(upMotor);
      Serial.print(" D: "); Serial.println(downMotor);
      
      // set motor speeds using the values
      setMotor(PWM1, DIR1, leftMotor);
      setMotor(PWM2, DIR2, rightMotor);
      setMotor(PWM3, DIR3, upMotor);
      setMotor(PWM4, DIR4, downMotor);
    }
    client.stop();  // close the connection
  }
}

// function to set motor speed and direction
void setMotor(int pwmPin, int dirPin, int speed) {
  // constrain speed to valid range (-255 to 255)
  speed = constrain(speed, -255, 255);
  // set motor direction
  digitalWrite(dirPin, speed >= 0 ? HIGH : LOW);
  // write the PWM value
  analogWrite(pwmPin, abs(speed));
}
