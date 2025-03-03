#include <SPI.h>
#include <Ethernet.h>

// config
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC address
IPAddress ip(192, 168, 1, 50);                         // arduino's static IP address
EthernetServer server(8080);                           // server listening on port 8080

// motor driver pins 
#define PWM1 5   // left motor speed control

#define DIR1 2   // left motor direction control
#define PWM2 6   // right motor speed control
#define DIR2 7   // right motor direction control
#define PWM3 9   //  up motor speed control
#define DIR3 8   //  up motor direction control
#define PWM4 10  //  down motor speed control
#define DIR4 11  //  down motor direction control

void setup() {
  // disable sd card
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  
  // Init ethernet
  Ethernet.begin(mac, ip);
  server.begin();  // start the server

  Serial.begin(9600);
  while (!Serial) { ; }
  
  // set motor pins as outputs
  pinMode(PWM1, OUTPUT); pinMode(DIR1, OUTPUT);
  pinMode(PWM2, OUTPUT); pinMode(DIR2, OUTPUT);
  pinMode(PWM3, OUTPUT); pinMode(DIR3, OUTPUT);
  pinMode(PWM4, OUTPUT); pinMode(DIR4, OUTPUT);
  
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {

// * don't forget to add ethenet checks 
  // check for an incoming connection from python
  EthernetClient client = server.available();
  if (client) {
    String command = "";
    // read incoming characters until a newline is encountered
    while (client.available()) {
      char c = client.read();
      if (c == '\n') break;
      command += c;
    }
    
    // process the command 
    if (command.length() > 0) {
      int leftMotor, rightMotor, upMotor, downMotor;
      //: "L:150,R:-150,U:255,D:0"
      sscanf(command.c_str(), "L:%d,R:%d,U:%d,D:%d", &leftMotor, &rightMotor, &upMotor, &downMotor);
      
      // degug stuff 
      Serial.print("L: "); Serial.print(leftMotor);
      Serial.print(" R: "); Serial.print(rightMotor);
      Serial.print(" U: "); Serial.print(upMotor);
      Serial.print(" D: "); Serial.println(downMotor);
      
      // set motor speeds
      setMotor(PWM1, DIR1, leftMotor);
      setMotor(PWM2, DIR2, rightMotor);
      setMotor(PWM3, DIR3, upMotor);
      setMotor(PWM4, DIR4, downMotor);
    }
    client.stop();  // close the connection
  }
}

// set motor speed and direction
void setMotor(int pwmPin, int dirPin, int speed) {
  // cconstrain speed to either -222 or 255
  speed = constrain(speed, -255, 255);
  // set motor direction based on the sign of speed
  digitalWrite(dirPin, speed >= 0 ? HIGH : LOW);
  // write the pw, value
  analogWrite(pwmPin, abs(speed));
  
}
