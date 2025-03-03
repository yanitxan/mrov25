#include <SPI.h>
#include <Ethernet.h>

// configuration
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC address
IPAddress ip(192, 168, 1, 50);                         // Arduino's static IP address
EthernetServer server(8080);                           // Server listening on port 8080

// motor driver pins 
#define PWM1 5   // Left motor speed control
// Originally DIR1 was defined as pin 4, but pin 4 is used by the SD card on the shield.
// Reassign DIR1 to a different free digital pin (e.g., pin 2).
#define DIR1 2   // Left motor direction control (reassigned)
#define PWM2 6   // Right motor speed control
#define DIR2 7   // Right motor direction control
#define PWM3 9   // Vertical up motor speed control
#define DIR3 8   // Vertical up motor direction control
#define PWM4 10  // Vertical down motor speed control
#define DIR4 11  // Vertical down motor direction control

void setup() {
  // Disable the SD card by setting its chip-select pin (pin 4) as OUTPUT and driving it HIGH.
  // This prevents interference with the Ethernet controller.
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  
  // Initialize Ethernet
  Ethernet.begin(mac, ip);
  server.begin();  // Start the server

  Serial.begin(9600);
  while (!Serial) { ; }
  
  // Set motor pins as outputs
  pinMode(PWM1, OUTPUT); pinMode(DIR1, OUTPUT);
  pinMode(PWM2, OUTPUT); pinMode(DIR2, OUTPUT);
  pinMode(PWM3, OUTPUT); pinMode(DIR3, OUTPUT);
  pinMode(PWM4, OUTPUT); pinMode(DIR4, OUTPUT);
  
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // Check for an incoming connection from the Python client
  EthernetClient client = server.available();
  if (client) {
    String command = "";
    // Read incoming characters until a newline is encountered
    while (client.available()) {
      char c = client.read();
      if (c == '\n') break;
      command += c;
    }
    
    // Process the command if received
    if (command.length() > 0) {
      int leftMotor, rightMotor, upMotor, downMotor;
      // Expected command format: "L:150,R:-150,U:255,D:0"
      sscanf(command.c_str(), "L:%d,R:%d,U:%d,D:%d", &leftMotor, &rightMotor, &upMotor, &downMotor);
      
      // Debug prints to the serial monitor
      Serial.print("L: "); Serial.print(leftMotor);
      Serial.print(" R: "); Serial.print(rightMotor);
      Serial.print(" U: "); Serial.print(upMotor);
      Serial.print(" D: "); Serial.println(downMotor);
      
      // Set motor speeds based on the received values
      setMotor(PWM1, DIR1, leftMotor);
      setMotor(PWM2, DIR2, rightMotor);
      setMotor(PWM3, DIR3, upMotor);
      setMotor(PWM4, DIR4, downMotor);
    }
    client.stop();  // Close the connection
  }
}

// Function to set motor speed and direction
void setMotor(int pwmPin, int dirPin, int speed) {
  // Constrain speed to the valid range (-255 to 255)
  speed = constrain(speed, -255, 255);
  // Set motor direction based on the sign of speed
  digitalWrite(dirPin, speed >= 0 ? HIGH : LOW);
  // Write the PWM value (absolute speed) to the motor
  analogWrite(pwmPin, abs(speed));
  
}
