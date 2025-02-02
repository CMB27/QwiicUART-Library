#include <Wire.h>
#include <QwiicUART.h>

QwiicUART qSerial;

bool peekToggle = true;

void setup() {
  pinMode(13, OUTPUT);

  Wire.begin();
  Serial.begin(115200);
  qSerial.begin(9600);
  qSerial.setMode(UART_MODE_RS485_HALF_DUPLEX);
}

void loop() {
  if (qSerial.available()) {
    if (peekToggle) {
      for (uint8_t i = 0; i < 3; i++) {
        Serial.print("Peeked: ");
        Serial.write(qSerial.peek());
        Serial.println();
      }
    }
    Serial.print("Read: ");
    while (qSerial.available()) {
      Serial.write(qSerial.read());
    }
    peekToggle = !peekToggle;
  }

  if(Serial.available()) {
    digitalWrite(13, HIGH);
    while (Serial.available()) {
      qSerial.write(Serial.read());
    }
    qSerial.flush();
    digitalWrite(13, LOW);
  }

}