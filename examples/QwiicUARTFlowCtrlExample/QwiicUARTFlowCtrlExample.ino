#include <Wire.h>
#include <QwiicUART.h>

QwiicUART qSerial;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  qSerial.begin(9600);
  qSerial.setHwFlowCtrlMode(UART_HW_FLOWCTRL_CTS_RTS);
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

}