#include <Wire.h>
#include <QwiicUART.h>

QwiicUART qSerial;

void setup() {
  pinMode(13, OUTPUT);

  Wire.begin();
  Serial.begin(115200);
  qSerial.begin(9600);
}

void loop() {
  if(qSerial.available()) Serial.write(qSerial.read());
  //if(Serial.available()) qSerial.write(Serial.read());

  if(Serial.available()) {
    digitalWrite(13, HIGH);
    while(Serial.available()) {
      qSerial.write(Serial.read());
    }
    qSerial.flush();
    digitalWrite(13, LOW);
  }

}