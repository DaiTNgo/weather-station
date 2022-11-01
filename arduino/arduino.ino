#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println("Goodnight moon!");
  mySerial.begin(9600);
}

void loop() {
  mySerial.println("Hello, world?");
}
