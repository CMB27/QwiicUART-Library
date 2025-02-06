#include "QwiicUART.h"

QwiicUART::QwiicUART(TwoWire& wire, uint8_t address) : _wire(wire) {
  _address = address;
}

void QwiicUART::begin(unsigned long baud, uint32_t config) {
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
  
  _reset();                                                               // software reset
  _writeRegister(_LCR, 0b10011101);                                       // enable setting the divisor
  _writeRegister(_getRegAddress(_DLL), lowByte((uint16_t)clockDivisor));  // set the low byte of the divisor
  _writeRegister(_getRegAddress(_DLH), highByte((uint16_t)clockDivisor)); // set the high byte of the divisor
  _writeRegister(_LCR, lcrValue);                                         // disable setting the divisor and set the configuration
  _writeRegister(_FCR, 0b00000001);                                       // enable the fifo buffers
  _writeRegister(_IER, 0b00000001);                                       // enable rx data available interrupt
}

void QwiicUART::end() {
  flush();
  _reset();
}



void QwiicUART::setMode(QwiicUARTMode mode) {
  switch (mode) {
    case UART_MODE_UART:
      _writeRegister(_EFCR, 0b00000000);
      break;
    case UART_MODE_RS485_HALF_DUPLEX:
      setHwFlowCtrlMode(UART_HW_FLOWCTRL_DISABLE);
      _writeRegister(_EFCR, 0b00110000);
      break;
  }
}

void QwiicUART::setHwFlowCtrlMode(QwiicUARTFlowCtrlMode mode, uint8_t threshold) {
  uint8_t efrValue = _readRegister(_EFR);
  switch (mode) {
    case UART_HW_FLOWCTRL_DISABLE:
      _writeRegister(_EFR, efrValue & 0b00111111);
      break;
    case UART_HW_FLOWCTRL_RTS:
      _writeRegister(_TCR, constrain((threshold + 2) / 4, 0x01, 0x0F));      
      _writeRegister(_EFR, (efrValue & 0b01111111) | 0b01000000);
      break;
    case UART_HW_FLOWCTRL_CTS:
      _writeRegister(_EFR, (efrValue & 0b10111111) | 0b10000000);
      break;
    case UART_HW_FLOWCTRL_CTS_RTS:
      _writeRegister(_TCR, constrain((threshold + 2) / 4, 0x01, 0x0F));      
      _writeRegister(_EFR, efrValue | 0b11000000);
      break;
  }
}

bool QwiicUART::digitalReadCts() {
  int16_t msrValue = _readRegister(_MSR);
  if (msrValue < 0) return false;
  return !bitRead(msrValue, 4);
}

void QwiicUART::digitalWriteRts(bool value) {
  int16_t mcrValue = _readRegister(_MCR);
  if (mcrValue < 0) return;
  bitWrite(mcrValue, 1, !value);
  _writeRegister(_MCR, mcrValue);
}



int QwiicUART::available() {
  int16_t rxlvlValue = _readRegister(_RXLVL);
  if (rxlvlValue < 0) rxlvlValue = 0;
  if (_peekedFlag) return rxlvlValue + 1;
  return rxlvlValue;
}

int QwiicUART::read() {
  if (_peekedFlag) {
    _peekedFlag = false;
    return _peekedChar;
  }
  return _readChar();
}

int QwiicUART::peek() {
  if (_peekedFlag) return _peekedChar;
  int16_t rhrValue = _readChar();
  if (rhrValue < 0) return -1;
  _peekedChar = rhrValue;
  _peekedFlag = true;
  return _peekedChar;
}



int QwiicUART::availableForWrite() {
  int16_t txlvlValue = _readRegister(_TXLVL);
  if (txlvlValue < 0) return 0;
  return txlvlValue;
}

void QwiicUART::flush() {
  while(true) {
    int16_t lsrValue = _readRegister(_LSR);
    if (lsrValue >= 0 && bitRead((uint8_t)lsrValue, 6)) return;
  }
}

size_t QwiicUART::write(uint8_t value) {
  while (!availableForWrite());
  _writeRegister(_THR, value);
  return 1;
}



int16_t QwiicUART::_readChar() {
  int16_t lsrValue = _readRegister(_LSR);
  if (lsrValue < 0) return -1;
  if (!bitRead(lsrValue, 0)) return -1;
  return _readRegister(_RHR);
}

void QwiicUART::_reset() {
  _writeRegister(_RESET, 0b00001000);
}



int16_t QwiicUART::_readRegister(_Reg reg) {
  int16_t value = -1;
  uint8_t regAddress = _getRegAddress(reg);
  switch (reg) {
    case _RHR:
    case _IER:
    case _IIR:
    case _LCR:
    case _MCR:
    case _LSR:
    case _MSR:
    case _TXLVL:
    case _RXLVL:
    case _RESET:
    case _EFCR:
      value = _readRegister(regAddress);
      break;
    case _TCR:
    case _TLR:
      {
        uint8_t efrValue = _readRegister(_EFR);
        uint8_t mcrValue = _readRegister(_MCR);
        _writeRegister(_EFR, efrValue | (1 << 4));
        _writeRegister(_MCR, mcrValue | (1 << 2));
        value = _readRegister(regAddress);
        _writeRegister(_MCR, mcrValue & ~(1 << 4));
        _writeRegister(_EFR, efrValue & ~(1 << 2));
      }
      break;
    case _EFR:
    case _XON1:
    case _XON2:
    case _XOFF1:
    case _XOFF2:
      {
        uint8_t lcrValue = _readRegister(_LCR);
        _writeRegister(_LCR, 0xBF);
        value = _readRegister(regAddress);
        _writeRegister(_LCR, lcrValue & ~(1 << 7));
      }
      break;
    default:
      break;
  }
  return value;
}

void QwiicUART::_writeRegister(_Reg reg, uint8_t value) {
  uint8_t regAddress = _getRegAddress(reg);
  switch (reg) {
    case _THR:
    case _IER:
    case _FCR:
    case _LCR:
    case _MCR:
    case _SPR:
    case _RESET:
    case _EFCR:
      _writeRegister(regAddress, value);
      break;
    case _TCR:
    case _TLR:
      {
        uint8_t efrValue = _readRegister(_EFR);
        uint8_t mcrValue = _readRegister(_MCR);
        _writeRegister(_EFR, efrValue | (1 << 4));
        _writeRegister(_MCR, mcrValue | (1 << 2));
        _writeRegister(regAddress, value);
        _writeRegister(_MCR, mcrValue & ~(1 << 4));
        _writeRegister(_EFR, efrValue & ~(1 << 2));
      }
      break;
    case _EFR:
    case _XON1:
    case _XON2:
    case _XOFF1:
    case _XOFF2:
      {
        uint8_t lcrValue = _readRegister(_LCR);
        _writeRegister(_LCR, 0xBF);
        _writeRegister(regAddress, value);
        _writeRegister(_LCR, lcrValue & ~(1 << 7));
      }
      break;
    default:
      break;
  }
}



uint8_t QwiicUART::_getRegAddress(_Reg reg) {
  switch (reg) {
    default:
    case _RHR:
    case _THR:
    case _DLL: return 0x00; // 0x00 << 3
    case _IER:
    case _DLH: return 0x08; // 0x01 << 3
    case _FCR:
    case _IIR:
    case _EFR: return 0x10; // 0x02 << 3
    case _LCR: return 0x18; // 0x03 << 3
    case _MCR:
    case _XON1: return 0x20; // 0x04 << 3
    case _LSR:
    case _XON2: return 0x28; // 0x05 << 3
    case _MSR:
    case _TCR:
    case _XOFF1: return 0x30; // 0x06 << 3
    case _SPR:
    case _TLR:
    case _XOFF2: return 0x38; // 0x07 << 3
    case _TXLVL: return 0x40; // 0x08 << 3
    case _RXLVL: return 0x48; // 0x09 << 3
    case _RESET: return 0x70; // 0x0E << 3
    case _EFCR: return 0x78; // 0x0F << 3
  }
}



int16_t QwiicUART::_readRegister(uint8_t regAddress) {
  _wire.beginTransmission(_address);
  _wire.write(regAddress);
  _wire.endTransmission(false);
  _wire.requestFrom(_address, (uint8_t)1);
  return _wire.read();
}

void QwiicUART::_writeRegister(uint8_t regAddress, uint8_t value) {
  _wire.beginTransmission(_address);
  _wire.write(regAddress);
  _wire.write(value);
  _wire.endTransmission();
}