#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,50);

void setup() {
  Serial.begin(9600);

  // disable SD card if one in the slot
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);

  Serial.println("Starting w5100");
  Ethernet.begin(mac,ip);

  Serial.println(Ethernet.localIP());
}

void loop() {}