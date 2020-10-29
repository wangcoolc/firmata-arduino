
#ifndef CANBUSFIRMATA_H
#define CANBUSFIRMATA_H

#include <Firmata.h>
#include "FirmataFeature.h"
#include <SPI.h>
#include "mcp_can.h"

#define INT_PIN 0
#define CANBUS_SPI_CS_PIN 9
#define CANBUS_SPI SPI
#define CANBUS_READ_TIMEOUT 5000 //mS

#define FIRMATA_CANBUS_FEATURE 

#define CANBUS_DATA                 0x64
#define PIN_MODE_CAN                0x0E

// CANBUS command bytes
#define CANBUS_CONFIG               0x10
#define CANBUS_WRITE                0x20
#define CANBUS_READ                 0x30
#define CANBUS_REPLY                0x40
#define CANBUS_CLOSE                0x50

// // CANBUS read modes
// #define CANBUS_READ_CONTINUOUSLY    0x00
// #define CANBUS_STOP_READING         0x01
#define CANBUS_MODE_MASK            0xF0
#define CANBUS_RXT_MASK             0x01
class CanbusFirmata: public FirmataFeature
{
  public:
    CanbusFirmata();
    boolean handlePinMode(byte pin, int mode){};
    void handleCapability(byte pin);
    boolean handleSysex(byte command, byte argc, byte* argv);
    void update();
    void reset();
  private:
    unsigned long startReadMillis;
    bool readEnable;
    SPIClass *_pSPI = NULL;
    int _intPin;
    MCP_CAN *_pCAN = (MCP_CAN *)malloc(sizeof(MCP_CAN));
    void reply(bool rxtEnable, uint8_t numBytes, long id ,uint8_t *buffer);
    void checkCANBUS();

};

#endif /* CanbusFirmata_h */
