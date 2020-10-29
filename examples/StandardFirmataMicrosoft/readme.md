# MicroSoft IoT expansion accessory software development manual

[Firmata](https://github.com/Hansen0314/firmata-arduino) is a protocol for communicating with microcontrollers from software on a host computer.This software-based on Standard Firmata protocol and extend some feature for Microsoft.and the software will be called FirmataMicrosoft.there exist extend feature as below:

**add SPI feature**

**add Serial feature**

**add 16-bit ADC feature**

**add CANBUS(MCP2515) feature**

**add Bootloader On/Off feature**

**add RS485(THVD1419DR) feature** 

Now we will describe the usage of FirmataMicrosoft.

## INFO

the info include protocol version,firmata version etc.

### firmata version

The firmware name to be reported should be exactly the same as the name of the Firmata client file, minus the file extension. So for StandardFirmata.ino, the firmware name is: StandardFirmata.

if we want to get the firmware version. we can type as below HEX to Microsoft IoT expansion accessory via USB.

**Query firmware Name and Version**

```
0  START_SYSEX       (0xF0)
1  queryFirmware     (0x79)
2  END_SYSEX         (0xF7)
```

**Receive Firmware Name and Version (after query)**

```
0  START_SYSEX       (0xF0)
1  queryFirmware     (0x79)
2  major version     (0-127)
3  minor version     (0-127)
4  first char of firmware name (LSB)
5  first char of firmware name (MSB)
6  second char of firmware name (LSB)
7  second char of firmware name (MSB)
... for as many bytes as it needs
N  END_SYSEX         (0xF7)
```

### protocol version

if we want to get the protocol version. we can type as below HEX to Microsoft IoT expansion accessory via USB.

**Request version report**
```
0  request version report (0xF9) (MIDI Undefined)
```

**the Version report format as below.**

```
0  version report header (0xF9)
1  major version (0-127)
2  minor version (0-127)
```

### Capability Query

The capability query provides a list of all modes supported by each pin. Each mode is described by 2 bytes where the first byte is the pin mode (such as digital input, digital output, PWM) and the second byte is the resolution (or sometimes the type of pin such as RX or TX for a UART pin). A value of 0x7F is used as a separator to mark the end each pin's list of modes. The number of pins supported is inferred by the message length.

**Capabilities query**

```
0  START_SYSEX              (0xF0)
1  CAPABILITY_QUERY         (0x6B)
2  END_SYSEX                (0xF7)
```

**Capabilities response**

```
0  START_SYSEX              (0xF0)
1  CAPABILITY_RESPONSE      (0x6C)
2  1st supported mode of pin 0
3  1st mode's resolution of pin 0
4  2nd supported mode of pin 0
5  2nd mode's resolution of pin 0
... additional modes/resolutions, followed by `0x7F`,
    to mark the end of the pin's modes. Subsequently, each pin
    follows with its modes/resolutions and `0x7F`,
    until all pins are defined. 
N  END_SYSEX                (0xF7)
```

#### Supported Modes

The modes in the following list are the modes of operation that can be returned during the capability response:

```
DIGITAL_INPUT      (0x00)
DIGITAL_OUTPUT     (0x01)
ANALOG_INPUT       (0x02)
PWM                (0x03)
SERVO              (0x04)
SHIFT              (0x05)
I2C                (0x06)
ONEWIRE            (0x07)
STEPPER            (0x08)
ENCODER            (0x09)
SERIAL             (0x0A)
INPUT_PULLUP       (0x0B)
SPI                (0x0C)
CAN                (0x0E)
```  

//TODO

*If no modes are defined for a pin, no values are returned (other than the separator value `0x7F`) and it should be assumed that pin is unsupported by Firmata.*

#### Mode Resolution

The resolution byte serves a couple of different purpose:

1. The original purpose was to define the resolution for analog input, pwm, servo and other modes that define a specific resolution (such as 10-bit for analog).
2. The resolution byte has been adapted for another purpose for Serial/UART pins, it defines if the pin is RX or TX and which UART it belongs to. RX0 is the RX pin of UART0 (Serial on an Arduino for example), TX1 if the TX pin of UART1 (Serial1 on an Arduino).

Modes utilizing the resolution byte as resolution data:

```
DIGITAL_INPUT      (0x00) // resolution is 1 (binary)
DIGITAL_OUTPUT     (0x01) // resolution is 1 (binary)
ANALOG_INPUT       (0x02) // analog input resolution in number of bits
PWM                (0x03) // pwm resolution in number of bits
SERVO              (0x04) // servo resolution in number of bits
STEPPER            (0x08) // resolution is number number of bits in max number of steps
INPUT_PULLUP       (0x0B) // resolution is 1 (binary)
```

Modes utilizing the resolution byte to define type of pin:

```
SERIAL             (0x0A) // RES_RX0 = 0x00 RES_TX0 = 0x01 
I2C                (0x06) // SCL = 0x00 SDA = 0x01
SPI                (0x0C) // SCK = 0x00 SDO = 0x01 SDI = 0x02
CAN                (0x0E) // CAN_L = 0x00 CAN_H = 0X01
```  

//TODO

*For other features (including I2C until updated) the resolution information is less important so a value of 1 is used.*

### Analog Mapping Query

Analog messages are numbered 0 to 15, which traditionally refer to the Arduino
pins labeled A0, A1, A2, etc. However, these pins are actually configured using
"normal" pin numbers in the pin mode message, and when those pins are used for
non-analog functions. The analog mapping query provides the information about
which pins (as used with Firmata's pin mode message) correspond to the analog
channels.

Analog mapping query
```
0  START_SYSEX              (0xF0)
1  analog mapping query     (0x69)
2  END_SYSEX                (0xF7)
```

Analog mapping response
```
0  START_SYSEX              (0xF0)
1  analog mapping response  (0x6A)
2  analog channel corresponding to pin 0, or 127 if pin 0 does not support analog
3  analog channel corresponding to pin 1, or 127 if pin 1 does not support analog
4  analog channel corresponding to pin 2, or 127 if pin 2 does not support analog
... etc, one byte for each pin
N  END_SYSEX                (0xF7)
```

*The above 2 queries provide static data (should never change for a particular
board). Because this information is fixed and should only need to be read once,
these messages are designed for a simple implementation in StandardFirmata,
rather that bandwidth savings (eg, using packed bit fields).*


### Pin State Query

The pin **state** is any data written to the pin (*it is important to note that
pin state != pin value*). For output modes (digital output,
PWM, and Servo), the state is any value that has been previously written to the
pin. For input modes, typically the state is zero. However, for digital inputs,
the state is the status of the pull-up resistor which is 1 if enabled, 0 if disabled.

The pin state query can also be used as a verification after sending pin modes
or data messages.

Pin state query
```
0  START_SYSEX              (0xF0)
1  pin state query          (0x6D)
2  pin                      (0-127)
3  END_SYSEX                (0xF7)
```

Pin state response
```
0  START_SYSEX              (0xF0)
1  pin state response       (0x6E)
2  pin                      (0-127)
3  pin mode (the currently configured mode)
4  pin state, bits 0-6
5  (optional) pin state, bits 7-13
6  (optional) pin state, bits 14-20
... additional optional bytes, as many as needed
N  END_SYSEX                (0xF7)
```

### String

Send short string messages between the board and the client application. String length is limited
to half the buffer size - 3 (for Arduino this limits strings to 30 chars). Commonly used to report
error messages to the client.

```
0  START_SYSEX        (0xF0)
1  STRING_DATA        (0x71)
2  first char LSB
3  first char MSB
4  second char LSB
5  second char MSB
... additional bytes up to half the buffer size - 3 (START_SYSEX, STRING_DATA, END_SYSEX)
N  END_SYSEX          (0xF7)
```

## SYSTEM

SYSTEM provides a couple of HEX to control the system and get the status of the system.

### RESTART SYSTEM

the all pin status will recover to default mode when the restart system is triggered.重启成功后的现象//TODO

**trigger restart system**

```
0  START_SYSEX       (0xF0)
1  RESTART_SYSTEM    (0x50)
2  END_SYSEX         (0xF7)
```

### BOOTLOADER MODE

this mode will be used to update the firmware. the system will restart when the firmware update finished.什么方式更新固件//TODO

**enter bootloader mode**

```
0  START_SYSEX       (0xF0)
1  BOOTLOADER_ON     (0x51)
2  END_SYSEX         (0xF7)
```

### SYSTEM STATUS

the system status response will be auto response when system IO enter over current or over voltage status.Of course, the system status response also will be triggered when the manual query the system status.

**System state query**

```
0  START_SYSEX                 (0xF0)
1  SYSTEM_STATUS_QUERY         (0x52)
2  END_SYSEX                   (0xF7)
```

**System state response**

```
0  START_SYSEX                 (0xF0)
1  SYSTEM_STATUS_RESPONSE      (0x53)
2  system_status(7-bit)        (0-127) //TODO
        0x00                   //  regular work
        0x01                   //  over current
        0x02                   //  over voltage
3  END_SYSEX                   (0xF7)
```


## GPIO

General-Purpose Input Output (GPIO) is a digital pin of an IC. It can be used as input or output for interfacing devices.If we want to read switch’s state, sensor data, etc then we need to configure it as input. And if we want to control the LED brightness, motor rotation, show text on display, etc then we need to configure it as output.There exist mraa-based [example](https://github.com/Hansen0314/mraa/blob/master/examples/c/firmata_gpio.c) code on the host side

### GPIO MODE

the GPIO has three modes for now.INPUT,OUTPUT,INPUT_PULLUP.

**Set pin mode**
```
0  set digital pin mode (0xF4) (MIDI Undefined)
1  set pin number (0-127)
2  mode (INPUT/OUTPUT/INPUT_PULLUP,0/1/11)
```

if we want to sets the digital pin 1 to input-mode we can type as below HEX to Microsoft IoT expansion accessory via USB.

```
0xF4 0x01 0x00
```

### GPIO OUT

there exist two control mode to produce output in terms of HIGH (3.3 V) or LOW (0 V).

the first one can be called port GPIO OUT. 


**Set digital port value**

Two byte digital data format, second nibble of byte 0 gives the port number (eg 0x92 is the third port, port 2)

```
0  digital data, 0x90-0x9F, (MIDI NoteOn, bud different data format)
1  digital pins 0-6 bitmask
2  digital pin 7 bitmask
```

if we want to sets the digital pin 1 on we can type as below HEX to Microsoft IoT expansion accessory via USB. and the 1 port include 8 pins.

```
0x90 0x02 0x00
```

the other one can be called bit GPIO OUT. 

**Set digital pin value**
```
0  set digital pin value (0xF5) (MIDI Undefined)
1  set pin number (0-127)
2  value (LOW/HIGH, 0/1)
```

if we want to sets the digital pin 1 on we can type as below HEX to MicroSoft IoT expansion accessory via USB.

```
0xF5 0x01 0x01
```

### GPIO IN

the gpio-in only has one mode is port read mode.

**Toggle digital port reporting by port** (second nibble of byte 0), eg 0xD1 is port 1 is pins 8 to 15
```
0  toggle digital port reporting (0xD0-0xDF) (MIDI Aftertouch)
1  disable(0) / enable(non-zero)
```

We can read data from port 0 GPIO(0 ~ 7) pin.use as below HEX.

```
0xD0 0x01
```

we can get feedback like this.that mean port 0 GPIO 1 is high(3.3v) and the other is low(0v).

```
0x90 0x02 0x00
```

**the reply protocol as below.**
Two byte digital data format, second nibble of byte 0 gives the port number.(eg 0x92 is the third port, port 2)

```
0  digital data, 0x90-0x9F, (MIDI NoteOn, bud different data format)
1  digital pins 0-6 bitmask
2  digital pin 7 bitmask
```
