#include "QwiicUART.h"

QwiicUART::QwiicUART(TwoWire& wire, uint8_t address) : _wire(wire) {
  _address = address;
}

void QwiicUART::begin(unsigned long baud, uint32_t config) {
  if (baud > 115200) baud = 115200;
  unsigned long clockDivisor = (115200 + (baud / 2)) / baud;


  uint8_t lcrValue;
  switch (config) {
    case SERIAL_5N1: lcrValue = 0b00000000; break;
    case SERIAL_6N1: lcrValue = 0b00000001; break;
    case SERIAL_7N1: lcrValue = 0b00000010; break;
    default:
    case SERIAL_8N1: lcrValue = 0b00000011; break;
    case SERIAL_5N2: lcrValue = 0b00000100; break;
    case SERIAL_6N2: lcrValue = 0b00000101; break;
    case SERIAL_7N2: lcrValue = 0b00000110; break;
    case SERIAL_8N2: lcrValue = 0b00000111; break;
    case SERIAL_5E1: lcrValue = 0b00011000; break;
    case SERIAL_6E1: lcrValue = 0b00011001; break;
    case SERIAL_7E1: lcrValue = 0b00011010; break;
    case SERIAL_8E1: lcrValue = 0b00011011; break;
    case SERIAL_5E2: lcrValue = 0b00011100; break;
    case SERIAL_6E2: lcrValue = 0b00011101; break;
    case SERIAL_7E2: lcrValue = 0b00011110; break;
    case SERIAL_8E2: lcrValue = 0b00011111; break;
    case SERIAL_5O1: lcrValue = 0b00001000; break;
    case SERIAL_6O1: lcrValue = 0b00001001; break;
    case SERIAL_7O1: lcrValue = 0b00001010; break;
    case SERIAL_8O1: lcrValue = 0b00001011; break;
    case SERIAL_5O2: lcrValue = 0b00001100; break;
    case SERIAL_6O2: lcrValue = 0b00001101; break;
    case SERIAL_7O2: lcrValue = 0b00001110; break;
    case SERIAL_8O2: lcrValue = 0b00001111; break;
  }

  /*
  _wire.beginTransmission(_address);
  _wire.write(SC16IS741A_LCR << 3);
  _wire.write(0b10000000);
  _wire.endTransmission();

  _wire.beginTransmission(_address);
  _wire.write(SC16IS741A_DLL << 3);
  _wire.write(lowByte((uint16_t)clockDivisor));
  _wire.write(highByte((uint16_t)clockDivisor));
  _wire.write(0b00000111);
  _wire.write(lcrValue);
  _wire.endTransmission();
  */

  _writeRegister(SC16IS741A_LCR, 0b10000000);
  _writeRegister(SC16IS741A_DLL, lowByte((uint16_t)clockDivisor));
  _writeRegister(SC16IS741A_DLH, highByte((uint16_t)clockDivisor));
  _writeRegister(SC16IS741A_LCR, lcrValue);
  _writeRegister(SC16IS741A_FCR, 0b00000111);
  

  // wait 2 * Tclk (see note 4 on page 18)


}

void QwiicUART::end() {

}

operator bool() {

}



int QwiicUART::available() {

}

int QwiicUART::read() {

}

int QwiicUART::peek() {

}



int QwiicUART::availableForWrite() {

}

void QwiicUART::flush() {

}

size_t QwiicUART::write(uint8_t byte) {

}

/*
size_t QwiicUART::write(const uint8_t *buffer, size_t size) {

}
*/



void QwiicUART::_writeRegister(uint8_t reg, uint8_t value) {
  _wire.beginTransmission(_address);
  _wire.write(reg << 3);
  _wire.write(value);
  _wire.endTransmission();
}

uint8_t QwiicUART::_readRegister(uint8_t reg) {
  _wire.beginTransmission(_address);
  _wire.write(reg << 3);
  _wire.endTransmission(false);
  _wire.requestFrom(_address, 1);
  if (_wire.available()) return _wire.read();
  return 0;
}

