#include "CanbusFirmata.h"


CanbusFirmata::CanbusFirmata():_pSPI(&CANBUS_SPI),_intPin(INT_PIN)
{
  pinMode(_intPin,INPUT);
  _pCAN->init_CS(CANBUS_SPI_CS_PIN);
  _pCAN->setSPI(_pSPI);
}

void CanbusFirmata::handleCapability(byte pin)
{
  if (IS_PIN_SPI(pin)) {
    Firmata.write(PIN_MODE_CAN);
    // would actually use a value that corresponds to a specific pin (MOSI, MISO, SCK)
    // for now, just set to 1
    Firmata.write(1);
  }
}

boolean CanbusFirmata::handleSysex(byte command, byte argc, byte *argv)
{
  if (command == CANBUS_DATA) {
    byte mode = argv[0] & CANBUS_MODE_MASK;
    bool rxtEnable = argv[0] & CANBUS_RXT_MASK;
    switch (mode) {
      case CANBUS_CONFIG:
        {
          _pCAN->wake();
          byte baud = argv[1];
          if (baud < CAN_5KBPS)
            baud = CAN_5KBPS;
          if (baud > CAN_1000KBPS)
            baud = CAN_1000KBPS;
          _pCAN->begin(baud);
          break; // CANBUS_CONFIG
        }
      case CANBUS_WRITE:
        {
          byte txBuff[8] = {0};
          byte len = argv[1];
          byte argvIndex = 0;
          byte txBuffIndex = 0;
          if (!len){
            break;
          }
          long id = (long)argv[2] | ((long)argv[3] << 7) | ((long)argv[4] << 14);
          argvIndex = 5;
          if (rxtEnable == true)
          {
            id = id | ((long)argv[5] << 21) | ((long)argv[6] << 28);
            argvIndex = 7;
          }
          for (; argvIndex < argc; argvIndex += 2) {
            txBuff[txBuffIndex++] = argv[argvIndex] + (argv[argvIndex + 1] << 7);
          }
          if (txBuffIndex == len){
            _pCAN->sendMsgBuf(id,rxtEnable,len,txBuff);
          }
          break; // CANBUS_WRITE
        }
      case CANBUS_READ:
        startReadMillis = millis();
        readEnable = true;
        break; // CANBUS_READ
      case CANBUS_CLOSE:
        readEnable = false;
        _pCAN->sleep();
        break; // CANBUS_CLOSE
    } // end switch
    return true;
  }
  return false;
}

void CanbusFirmata::update()
{
  checkCANBUS();
}

void CanbusFirmata::reset()
{
  readEnable = false;
  startReadMillis = 0;
}

void CanbusFirmata::checkCANBUS()
{
  byte len;
  byte rxBuff[8];
  if (!readEnable)
    return ;
  unsigned long currentMillis = millis();  
  if (currentMillis - startReadMillis > CANBUS_READ_TIMEOUT){
    readEnable = false;
    return ;
  }  
  if (CAN_MSGAVAIL != _pCAN->checkReceive())
    return ;
  _pCAN->readMsgBuf(&len, rxBuff);
  long id =_pCAN->getCanId();
  bool rxtEnable = _pCAN->isExtendedFrame();
  reply(rxtEnable,len,id,rxBuff);
  readEnable = false;
}
void CanbusFirmata::reply(bool rxtEnable, uint8_t numBytes, long id ,uint8_t *buffer)
{
  Firmata.write(START_SYSEX);
  Firmata.write(CANBUS_DATA);
  Firmata.write(CANBUS_REPLY | rxtEnable);
  Firmata.write(numBytes);
  byte idLen = 3;
  if (rxtEnable)
    idLen = 5;
  for (uint8_t i = 0; i < idLen; i++){
    Firmata.write((id >> (i * 7))&0x7f);
  }
  for (uint8_t i = 0; i < numBytes; i++) {
    Firmata.write(buffer[i] & 0x7F);
    Firmata.write(buffer[i] >> 7 & 0x7F);
  }
  Firmata.write(END_SYSEX);
}
