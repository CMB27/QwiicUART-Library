#include <Wire.h>
#include <QwiicUART.h>

QwiicUART qSerial;

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  Wire.begin();
  Serial.begin(115200);
  qSerial.begin(9600);
}

void loop() {
  if (qSerial.available()) {
    while (qSerial.available()) {
      Serial.write(qSerial.read());
    }
  }

  if (Serial.available()) {
    while (Serial.available()) {
      qSerial.write(Serial.read());
    }
  }

  qSerial.digitalWriteRts(digitalRead(2));
  digitalWrite(13, qSerial.digitalReadCts());

}