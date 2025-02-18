/*
  NXP SC16IS741A Webpage: https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/bridges/single-uart-with-ic-bus-spi-interface-64-b-of-transmit-and-receive-fifos-irda-sir-built-in-support:SC16IS741
  NXP SC16IS741A Datasheet: https://www.nxp.com/docs/en/data-sheet/SC16IS741A.pdf
*/


#ifndef QwiicUART_h
#define QwiicUART_h

#include "Arduino.h"
#include <Stream.h>
#include <Wire.h>

#define QWIIC_UART_CLK 1843200
#define QWIIC_UART_MAX_BAUD (QWIIC_UART_CLK / 16)

enum QwiicUARTMode {
  UART_MODE_UART,
  UART_MODE_RS485_HALF_DUPLEX
};
// I would like to add UART_MODE_IRDA in future.
// I don't currently have the equipment to test this mode.

enum QwiicUARTFlowCtrlMode {
  UART_HW_FLOWCTRL_DISABLE,
  UART_HW_FLOWCTRL_RTS,
  UART_HW_FLOWCTRL_CTS,
  UART_HW_FLOWCTRL_CTS_RTS
};

class QwiicUART : public Stream {
  public:
    QwiicUART(TwoWire& wire = Wire, uint8_t address = 0x48);
    void begin(unsigned long baud, uint32_t config = SERIAL_8N1);
    void end();

    void setMode(QwiicUARTMode mode);
    void setHwFlowCtrlMode(QwiicUARTFlowCtrlMode mode, uint8_t threshold = 32);
    bool digitalReadCts();
    void digitalWriteRts(bool value);

    virtual int available();
    virtual int read();
    virtual int peek();

    virtual int availableForWrite();
    virtual void flush();
    virtual size_t write(uint8_t value);
    virtual size_t write(const uint8_t *buffer, size_t size);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write;
    
  private:
    enum _Reg {_RHR, _THR, _IER, _FCR, _IIR, _LCR, _MCR, _LSR, _MSR, _SPR, _TCR, _TLR, _TXLVL, _RXLVL, _RESET, _EFCR, _DLL, _DLH, _EFR, _XON1, _XON2, _XOFF1, _XOFF2};

    TwoWire& _wire;
    uint8_t _address;
    bool _peekedFlag = false;
    uint8_t _peekedChar;

    int16_t _readChar();
    void _reset();

    int16_t _readRegister(_Reg reg);
    void _writeRegister(_Reg reg, uint8_t value);

    uint8_t _getRegAddress(_Reg reg);

    int16_t _readRegister(uint8_t regAddress);
    void _writeRegister(uint8_t regAddress, uint8_t value);

};

#endif