#ifndef QwiicUART_h
#define QwiicUART_h

#include "Arduino.h"
#include <Stream.h>
#include <Wire.h>

#define SC16IS741A_RHR 0x00
#define SC16IS741A_THR 0x00
#define SC16IS741A_IER 0x01
#define SC16IS741A_FCR 0x02
#define SC16IS741A_IIR 0x02
#define SC16IS741A_LCR 0x03
#define SC16IS741A_MCR 0x04
#define SC16IS741A_LSR 0x05
#define SC16IS741A_MSR 0x06
#define SC16IS741A_SPR 0x07
#define SC16IS741A_TCR 0x06
#define SC16IS741A_TLR 0x07
#define SC16IS741A_TXLVL 0x08
#define SC16IS741A_RXLVL 0x09
#define SC16IS741A_UART_RESET 0x0E
#define SC16IS741A_EFCR 0x0F

#define SC16IS741A_DLL 0x00
#define SC16IS741A_DLH 0x01
#define SC16IS741A_EFR 0x02
#define SC16IS741A_XON1 0x04
#define SC16IS741A_XON2 0x05
#define SC16IS741A_XOFF1 0x06
#define SC16IS741A_XOFF2 0x07

class QwiicUART : public Stream {
  public:
    QwiicUART(TwoWire& wire = Wire, uint8_t address = 0x90);
    void begin(unsigned long baud, uint32_t config = SERIAL_8N1);
    void end();
    operator bool();

    virtual int available();
    virtual int read();
    virtual int peek();

    virtual int availableForWrite();
    virtual void flush();
    virtual size_t write(uint8_t byte);
    //virtual size_t write(const uint8_t *buffer, size_t size);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write;

  private:
    TwoWire& _wire;
    uint8_t _address;
    void _writeRegister(uint8_t reg, uint8_t value);
    uint8_t _readRegister(uint8_t reg);
}


#endif