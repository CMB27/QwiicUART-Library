#include <Wire.h>
#include <QwiicUART.h>

QwiicUART qSerial;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  qSerial.begin(38400);
}

void loop() {

  if (Serial.available()) {
    uint8_t buffer[256];
    size_t length = Serial.readBytes(buffer, 256);
    Serial.write(buffer, length);
    qSerial.write(buffer, length);
  }

}