#include <Arduino.h>
#include <Wire.h>

#ifndef ADC_ADS1115_H
#define ADC_ADS1115_H
#define USE_ADS1115_ANALOG_READ
#define ADS1115_GND_ADDRESS    (0x48)  // ADDR pin connect to GDN
#define ADS1115_VDD_ADDRESS    (0x49)  // ADDR pin connect to VDD
#define ADS1115_SDA_ADDRESS    (0x4A)  // ADDR pin connect to SDA
#define ADS1115_SCL_ADDRESS    (0x4B)  // ADDR pin connect to SCL

#define ADS1115_CONVERSIONDELAY     (300)  
#define ADS1115_POINTER_MASK        (0x03)
#define ADS1115_POINTER_CONVERT     (0x00)
#define ADS1115_POINTER_CONFIG      (0x01)
#define ADS1115_POINTER_LOWTHRESH   (0x02)
#define ADS1115_POINTER_HITHRESH    (0x03)

#define ADS1115_OS_MASK      (0x8000)    // Operational status/single-shot conversion start
#define ADS1115_OS_NOEFFECT  (0x0000)    // Write: Bit = 0
#define ADS1115_OS_SINGLE    (0x8000)    // Write: Bit = 1
#define ADS1115_OS_BUSY      (0x0000)    // Read: Bit = 0
#define ADS1115_OS_NOTBUSY   (0x8000)    // Read: Bit = 1

#define ADS1115_MUX_MASK     (0x7000)    // Input multiplexer configuration
#define ADS1115_MUX_0_1      (0x0000)    // P = AIN0, N = AIN1(default)
#define ADS1115_MUX_0_3      (0x1000)    // P = AIN0, N = AIN3
#define ADS1115_MUX_1_3      (0x2000)    // P = AIN1, N = AIN3
#define ADS1115_MUX_2_3      (0x3000)    // P = AIN2, N = AIN3
#define ADS1115_MUX_0_G      (0x4000)    // P = AIN0, N = GND
#define ADS1115_MUX_1_G      (0x5000)    // P = AIN1, N = GND
#define ADS1115_MUX_2_G      (0x6000)    // P = AIN2, N = GND
#define ADS1115_MUX_3_G      (0x7000)    // P = AIN3, N = GND

#define ADS1115_PGA_MASK     (0x0E00)    // Programmable gain amplifier configuration
#define ADS1115_PGA_6_144    (0x0000)    // 6.144V 
#define ADS1115_PGA_4_096    (0x0200)    // 4.096V 
#define ADS1115_PGA_2_048    (0x0400)    // 2.048V (default)
#define ADS1115_PGA_1_024    (0x0600)    // 1.024V 
#define ADS1115_PGA_0_512    (0x0800)    // 0.512V 
#define ADS1115_PGA_0_256    (0x0E00)    // 0.256V 

#define ADS1115_MODE_MASK    (0x0100)    // Device operating mode
#define ADS1115_MODE_CONTIN  (0x0000)    // Continuous conversion mode
#define ADS1115_MODE_SINGLE  (0x0100)    // Power-down single-shot mode (default)

#define ADS1115_DR_MASK      (0x00E0)    // Data rate(samples per second)
#define ADS1115_DR_8         (0x0000)    // 8 SPS
#define ADS1115_DR_16        (0x0020)    // 16 SPS
#define ADS1115_DR_32        (0x0040)    // 32 SPS
#define ADS1115_DR_64        (0x0060)    // 64 SPS
#define ADS1115_DR_128       (0x0080)    // 128 SPS (default)
#define ADS1115_DR_250       (0x00A0)    // 250 SPS
#define ADS1115_DR_475       (0x00C0)    // 475 SPS
#define ADS1115_DR_860       (0x00E0)    // 860 SPS

#define ADS1115_COMP_MODE_MASK   (0x0010)    // Comparator mode
#define ADS1115_COMP_MODE_TRAD   (0x0000)    // Traditional comparator with hysteresis (default)
#define ADS1115_COMP_MODE_WINDOW (0x0010)    // Window comparator

#define ADS1115_COMP_POL_MASK      (0x0008)    // Comparator polarity
#define ADS1115_COMP_POL_ACTVLOW   (0x0000)    // Active low(default)
#define ADS1115_COMP_POL_ACTVHIGH  (0x0008)    // Active high

#define ADS1115_COMP_LAT_MASK    (0x0004)    // Latching comparator
#define ADS1115_COMP_LAT_NONLAT  (0x0000)    // Non-latching comparator (default)
#define ADS1115_COMP_LAT_LATCH   (0x0004)    // Latching comparator

#define ADS1115_COMP_QUE_MASK    (0x0003)    // Comparator queue and disable
#define ADS1115_COMP_QUE_1CONV   (0x0000)    // After one conversions
#define ADS1115_COMP_QUE_2CONV   (0x0001)    // After two conversions
#define ADS1115_COMP_QUE_4CONV   (0x0002)    // After four conversions
#define ADS1115_COMP_QUE_NONE    (0x0003)    // Disable the comparator(default)

#define ADS1115_DEFAULT_CONFIG   (0x8583)    //Dfault content in config register

#define CHANNELNUMS 8   //channel number
typedef enum
{
    channel0 = 0,
    channel1 ,
    channel2 ,
    channel3 ,
    channel01 ,
    channel03 ,
    channel13 ,
    channel23
} channel_t;


template <class T>
class ADS1115{
private:
    static T* _pWire;
    static uint16_t _configRegister;
    static uint8_t _i2cAddress;
    static int16_t _channelToMUX[];
    static void writeRegister(uint8_t reg, uint16_t value);
    static int16_t readRegister(uint8_t reg);
public:
    ADS1115(T& myWire);
    bool begin(uint8_t i2cAddress = ADS1115_GND_ADDRESS);
    void setOperateMode(uint16_t operateMode);
    void setOperateStaus(uint16_t OperateStaus);
    void setPGAGain(uint16_t PGAGain);
    void setSampleRate(uint16_t SampleRate);
    void setInputMux(uint16_t InputMux);
    int16_t getConversionResults(channel_t channel);
    void setCompaMode(uint16_t CompaMode);
    void setCompaPolarity(uint16_t CompaPolarity);
    void setCompaLatching(uint16_t CompaLatching);
    void setCompaQueue(uint16_t CompaQueue);
    void setLowThreshold(uint16_t threshold);
    void setHighThreshold(uint16_t threshold);
};

#endif /*ADS1115_H*/