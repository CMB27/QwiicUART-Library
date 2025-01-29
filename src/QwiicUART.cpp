#include "QwiicUART.h"

QwiicUART::QwiicUART(TwoWire& wire, uint8_t address) : _wire(wire) {
  _address = address;
}

void QwiicUART::begin(unsigned long baud, uint32_t config) {
  _setupFlag = false;

  // calulate the best divisor for the requested baud rate
  if (baud > QWIIC_UART_MAX_BAUD) baud = QWIIC_UART_MAX_BAUD;
  unsigned long clockDivisor = (QWIIC_UART_MAX_BAUD + (baud / 2)) / baud;

  // select the correct lcr value for the requested configuration
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
  
  _writeRegister(SC16IS741A_UART_RESET, 0b00001000);                // reset the uart
  _writeRegister(SC16IS741A_LCR, 0b10011101);                       // enable setting the divisor
  _writeRegister(SC16IS741A_DLL, lowByte((uint16_t)clockDivisor));  // set the low byte of the divisor
  _writeRegister(SC16IS741A_DLH, highByte((uint16_t)clockDivisor)); // set the high byte of the divisor
  _writeRegister(SC16IS741A_LCR, 0xBF);                             // disable setting the divisor and enable the enhanced features register
  _writeRegister(SC16IS741A_EFR, 0b00010000);                       // enable enhanced functions
  _writeRegister(SC16IS741A_LCR, lcrValue);                         // disable the enhanced features register and set the configuration
  _writeRegister(SC16IS741A_IER, 0b00010001);                       // enable sleep mode and rx data available interrupt
  _writeRegister(SC16IS741A_FCR, 0b00000111);                       // clear and enable the fifo buffers
  delayMicroseconds(QWIIC_UART_2TCLK_MICROS);

  _setupFlag = true;
}

void QwiicUART::end() {
  _writeRegister(SC16IS741A_UART_RESET, 0b00001000);
  _setupFlag = false;
}

/*
bool QwiicUART::operator() {
  if (!_setupFlag) return false;
  uint8_t value = random(0, 255);
  if (_writeRegister(SC16IS741A_SPR, value) != 1) return false;
  int16_t sprValue = _readRegister(SC16IS741A_SPR);
  if (sprValue < 0) return false;
  if ((uint8_t)sprValue != value) return false;
  return true;
}
*/


/*
bool QwiicUART::setHwFlowCtrlMode(QwiicUARTFlowCtrlMode mode) {
  int16_t lcrValue = _readRegister(SC16IS741A_LCR);
  if (lcrValue < 0) return false;
  int16_t mcrValue = _readRegister(SC16IS741A_MCR);
  if (mcrValue < 0) return false;

  switch (mode) {
    //default:
    case UART_HW_FLOWCTRL_DISABLE:
      if (_writeRegister(SC16IS741A_LCR, 0xBF) != 1) return false;
      if (_writeRegister(SC16IS741A_EFR, 0b00010000) != 1) return false;
      if (_writeRegister(SC16IS741A_LCR, lcrValue) != 1) return false;
      break;
    case UART_HW_FLOWCTRL_RTS:
      if (_writeRegister(SC16IS741A_LCR, 0xBF) != 1) return false;
      if (_writeRegister(SC16IS741A_EFR, 0b00010000) != 1) return false;
      if (_writeRegister(SC16IS741A_LCR, lcrValue) != 1) return false;
      if (_writeRegister(SC16IS741A_MCR, mcrValue | 0b00000100) != 1) return false;
      if (_writeRegister(SC16IS741A_TCR, 0x79) != 1) return false;
      if (_writeRegister(SC16IS741A_MCR, mcrValue) != 1) return false;
      if (_writeRegister(SC16IS741A_LCR, 0xBF) != 1) return false;
      if (_writeRegister(SC16IS741A_EFR, 0b01010000) != 1) return false;
      if (_writeRegister(SC16IS741A_LCR, lcrValue) != 1) return false;
      break;
    case UART_HW_FLOWCTRL_CTS:
      if (_writeRegister(SC16IS741A_LCR, 0xBF) != 1) return false;
      if (_writeRegister(SC16IS741A_EFR, 0b10010000) != 1) return false;
      if (_writeRegister(SC16IS741A_LCR, lcrValue) != 1) return false;
      break;
    case UART_HW_FLOWCTRL_CTS_RTS:
      if (_writeRegister(SC16IS741A_LCR, 0xBF) != 1) return false;
      if (_writeRegister(SC16IS741A_EFR, 0b00010000) != 1) return false;
      if (_writeRegister(SC16IS741A_LCR, lcrValue) != 1) return false;
      if (_writeRegister(SC16IS741A_MCR, mcrValue | 0b00000100) != 1) return false;
      if (_writeRegister(SC16IS741A_TCR, 0x79) != 1) return false;
      if (_writeRegister(SC16IS741A_MCR, mcrValue) != 1) return false;
      if (_writeRegister(SC16IS741A_LCR, 0xBF) != 1) return false;
      if (_writeRegister(SC16IS741A_EFR, 0b11010000) != 1) return false;
      if (_writeRegister(SC16IS741A_LCR, lcrValue) != 1) return false;
      break;
  }
  return true;
}

bool QwiicUART::setMode(QwiicUARTMode mode) {
  int16_t mcrValue;
  switch (mode) {
    //default:
    case UART_MODE_UART:
      mcrValue = _readRegister(SC16IS741A_MCR);
      if (mcrValue < 0) return false;
      if (_writeRegister(SC16IS741A_MCR, mcrValue & ~(0b10000000)) != 1) return false;
      if (_writeRegister(SC16IS741A_EFCR, 0b00000000) != 1) return false;
      break;
    case UART_MODE_RS485_HALF_DUPLEX:
      if (setHwFlowCtrlMode(UART_HW_FLOWCTRL_DISABLE) == false) return false;
      if (_writeRegister(SC16IS741A_EFCR, 0b00110000) != 1) return false;
      break;
    case UART_MODE_IRDA:
      mcrValue = _readRegister(SC16IS741A_MCR);
      if (mcrValue < 0) return false;
      if (_writeRegister(SC16IS741A_MCR, mcrValue | 0b10000000) != 1) return false;
      break;
  }
  return true;
}

bool QwiicUART::digitalReadCts() {
  int16_t msrValue = _readRegister(SC16IS741A_MSR);
  if (msrValue < 0) return false;
  return !bitRead(msrValue, 4);
}

void QwiicUART::digitalWriteRts(bool value) {
  int16_t mcrValue = _readRegister(SC16IS741A_MCR);
  if (mcrValue < 0) return;
  bitWrite(mcrValue, 1, !value);
  _writeRegister(SC16IS741A_MCR, mcrValue);
}
*/




int QwiicUART::available() {
  int16_t rxlvlValue = _readRegister(SC16IS741A_RXLVL);
  if (rxlvlValue < 0) rxlvlValue = 0;
  if (_peekedFlag) return rxlvlValue + 1;
  return rxlvlValue;
}

int QwiicUART::read() {
  if (_peekedFlag) {
    _peekedFlag = false;
    return _peekedChar;
  }
  int16_t lsrValue = _readRegister(SC16IS741A_LSR);
  if (lsrValue < 0) return -1;
  if (bitRead(lsrValue, 0) == false) return -1;
  return _readRegister(SC16IS741A_RHR);
}

int QwiicUART::peek() {
  if (_peekedFlag) return _peekedChar;
  int16_t lsrValue = _readRegister(SC16IS741A_LSR);
  if (lsrValue < 0) return -1;
  if (bitRead(lsrValue, 0) == false) return -1;
  int16_t rhrValue = _readRegister(SC16IS741A_RHR);
  if (rhrValue < 0) return -1;
  _peekedChar = rhrValue;
  _peekedFlag = true;
  return _peekedChar;
}



int QwiicUART::availableForWrite() {
  int16_t txlvlValue = _readRegister(SC16IS741A_TXLVL);
  if (txlvlValue < 0) return 0;
  return txlvlValue;
}

void QwiicUART::flush() {
  while(true) {
    int16_t lsrValue = _readRegister(SC16IS741A_LSR);
    if (lsrValue < 0) return;
    if (bitRead((uint8_t)lsrValue, 5)) return;
  }
}

size_t QwiicUART::write(uint8_t value) {
  while (!availableForWrite());
  _writeRegister(SC16IS741A_THR, value);
  return 1;
}



int16_t QwiicUART::_readRegister(uint8_t reg) {
  _wire.beginTransmission(_address);
  _wire.write(reg);
  uint8_t error = _wire.endTransmission(false);
  if (error) return -1;
  if (_wire.requestFrom(_address, (uint8_t)1) != 1) return -1;
  if (!_wire.available()) return -1;
  return _wire.read();
}

void QwiicUART::_writeRegister(uint8_t reg, uint8_t value) {
  _wire.beginTransmission(_address);
  _wire.write(reg);
  _wire.write(value);
  _wire.endTransmission();
}

/*
uint8_t QwiicUART::_writeRegister(uint8_t reg, uint8_t value) {
  uint8_t attemptCount = 0;
  while (true) {
    attemptCount++;
    _wire.beginTransmission(_address);
    _wire.write(reg << 3);
    _wire.write(value);
    uint8_t error = _wire.endTransmission();
    if (error) {
      Serial.print("_writeRegister attemptCount: ");
      Serial.print(attemptCount);
      Serial.print(" error: ");
      Serial.println(error);
      if (attemptCount >= 10) return 0;
      //return 0;
    }
    else return 1;
  }
}
*/
