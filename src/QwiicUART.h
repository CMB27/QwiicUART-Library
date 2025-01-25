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
#define QWIIC_UART_2TCLK_MICROS ((2000000 + (QWIIC_UART_CLK - 1)) / QWIIC_UART_CLK)



// these registers are always accessible
#define SC16IS741A_LCR 0x03
#define SC16IS741A_TXLVL 0x08
#define SC16IS741A_RXLVL 0x09
#define SC16IS741A_UART_RESET 0x0E
#define SC16IS741A_EFCR 0x0F

// only accessible when LCR[7] != 1
#define SC16IS741A_RHR 0x00
#define SC16IS741A_THR 0x00
#define SC16IS741A_IER 0x01

// only accessible when LCR != 0xBF
#define SC16IS741A_FCR 0x02
#define SC16IS741A_IIR 0x02
#define SC16IS741A_MCR 0x04
#define SC16IS741A_LSR 0x05

// only accessible when LCR != 0xBF and MCR[2] = 0
#define SC16IS741A_MSR 0x06
#define SC16IS741A_SPR 0x07

// only accessible when LCR != 0xBF and MCR[2] = 1 and EFR[4] = 1
#define SC16IS741A_TCR 0x06
#define SC16IS741A_TLR 0x07

// only accessible when LCR[7] = 1 but LCR != 0xBF
#define SC16IS741A_DLL 0x00
#define SC16IS741A_DLH 0x01

// only accessible when LCR = 0xBF
#define SC16IS741A_EFR 0x02
#define SC16IS741A_XON1 0x04
#define SC16IS741A_XON2 0x05
#define SC16IS741A_XOFF1 0x06
#define SC16IS741A_XOFF2 0x07

// some bits can only be written to when EFR[4] = 1
// when exiting a function:
// EFR[4] = 1
// MCR[2] = 0
// LCR[7] = 0



enum QwiicUARTMode {
  UART_MODE_UART,
  UART_MODE_RS485_HALF_DUPLEX,
  UART_MODE_IRDA
};

enum QwiicUARTFlowCtrlMode {
  UART_HW_FLOWCTRL_DISABLE,
  UART_HW_FLOWCTRL_RTS,
  UART_HW_FLOWCTRL_CTS,
  UART_HW_FLOWCTRL_CTS_RTS
};

class QwiicUART : public Stream {
  public:
    QwiicUART(TwoWire& wire = Wire, uint8_t address = 0x90);
    void begin(unsigned long baud, uint32_t config = SERIAL_8N1);
    void end();
    operator bool();

    bool setHwFlowCtrlMode(QwiicUARTFlowCtrlMode mode = UART_HW_FLOWCTRL_CTS_RTS, uint8_t threshold = 32);
    bool setMode(QwiicUARTMode mode);
    bool digitalReadCts();
    void digitalWriteRts(bool value);

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
    TwoWire& _wire;
    uint8_t _address;
    bool _setupFlag = false;
    bool _peekedFlag = false;
    uint8_t _peekedChar;

    int16_t _readRegister(uint8_t reg);
    uint8_t _writeRegister(uint8_t reg, uint8_t value);

};


#endif