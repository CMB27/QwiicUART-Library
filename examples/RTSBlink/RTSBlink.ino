#include <Wire.h>
#include <QwiicUART.h>

QwiicUART qSerial;

void setup() {
  Wire.begin();
  qSerial.begin(9600);
}

void loop() {
  qSerial.digitalWriteRts(HIGH);
  delay(1000);
  qSerial.digitalWriteRts(LOW);
  delay(1000);
}
