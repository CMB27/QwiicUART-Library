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


/*
// these registers are always accessible
#define SC16IS741A_LCR        0x18  // 0x03 << 3  R/W
#define SC16IS741A_TXLVL      0x40  // 0x08 << 3  R
#define SC16IS741A_RXLVL      0x48  // 0x09 << 3  R
#define SC16IS741A_UART_RESET 0x70  // 0x0E << 3  R/W
#define SC16IS741A_EFCR       0x78  // 0x0F << 3  R/W

// only accessible when
// LCR[7] != 1
#define SC16IS741A_RHR        0x00  // 0x00 << 3  R
#define SC16IS741A_THR        0x00  // 0x00 << 3  W
#define SC16IS741A_IER        0x08  // 0x01 << 3  R/W

// only accessible when
// LCR != 0xBF
#define SC16IS741A_FCR        0x10  // 0x02 << 3  W
#define SC16IS741A_IIR        0x10  // 0x02 << 3  R
#define SC16IS741A_MCR        0x20  // 0x04 << 3  R/W
#define SC16IS741A_LSR        0x28  // 0x05 << 3  R

// only accessible when
// LCR != 0xBF (0b10111111) and
// MCR[2] = 0
#define SC16IS741A_MSR        0x30  // 0x06 << 3
#define SC16IS741A_SPR        0x38  // 0x07 << 3

// only accessible when
// LCR != 0xBF (0b10111111) and
// MCR[2] = 1 and
// EFR[4] = 1
#define SC16IS741A_TCR        0x30  // 0x06 << 3
#define SC16IS741A_TLR        0x38  // 0x07 << 3

// only accessible when
// LCR[7] = 1 and
// LCR != 0xBF (0b10111111)
#define SC16IS741A_DLL        0x00  // 0x00 << 3
#define SC16IS741A_DLH        0x08  // 0x01 << 3

// only accessible when
// LCR = 0xBF (0b10111111)
#define SC16IS741A_EFR        0x10  // 0x02 << 3
#define SC16IS741A_XON1       0x20  // 0x04 << 3
#define SC16IS741A_XON2       0x28  // 0x05 << 3
#define SC16IS741A_XOFF1      0x30  // 0x06 << 3
#define SC16IS741A_XOFF2      0x38  // 0x07 << 3

// some bits can only be written to when EFR[4] = 1
*/


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
    void setHwFlowCtrlMode(QwiicUARTFlowCtrlMode mode = UART_HW_FLOWCTRL_CTS_RTS, uint8_t threshold = 32);
    //bool digitalReadCts();
    //void digitalWriteRts(bool value);

    virtual int available();
    virtual int read();
    virtual int peek();

    virtual int availableForWrite();
    virtual void flush();
    virtual size_t write(uint8_t value);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write;
    
  private:
  enum _Reg {
      _RHR,
      _THR,
      _IER,
      _FCR,
      _IIR,
      _LCR,
      _MCR,
      _LSR,
      _MSR,
      _SPR,
      _TCR,
      _TLR,
      _TXLVL,
      _RXLVL,
      _RESET,
      _EFCR,
      _DLL,
      _DLH,
      _EFR,
      _XON1,
      _XON2,
      _XOFF1,
      _XOFF2
    };

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