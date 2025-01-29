#include <Wire.h>
#include <QwiicUART.h>

QwiicUART qSerial;

void setup() {
    Wire.begin();
    Serial.begin(9600);
    qSerial.begin(9600);
}

void loop() {
    if(qSerial.available()) Serial.write(qSerial.read());
    if(Serial.available()) qSerial.write(Serial.read());
}