#include"AdcAds1115.h"
// Initialize Class Variables //////////////////////////////////////////////////
template <class T>
int16_t ADS1115<T>::_channelToMUX[CHANNELNUMS] = {ADS1115_MUX_0_G,ADS1115_MUX_1_G,ADS1115_MUX_2_G,
    ADS1115_MUX_3_G,ADS1115_MUX_0_1,ADS1115_MUX_0_3,ADS1115_MUX_1_3,ADS1115_MUX_2_3};
template <class  T>
T* ADS1115<T>::_pWire = 0;
template<class T>
uint8_t ADS1115<T>::_i2cAddress = 0;
template<class T>
uint16_t ADS1115<T>::_configRegister = ADS1115_DEFAULT_CONFIG;
/**************************************************************************/
/*!
    @brief  Writes 16-bits to the specified destination register
    @param reg register address to write to
    @param value value to write to register
*/
/**************************************************************************/
template <class T>
ADS1115<T>::ADS1115(T& myWire){
    _pWire = &myWire;
}
/**************************************************************************/
/*!
    @brief  Writes 16-bits to the specified destination register
    @param reg register address to write to
    @param value value to write to register
*/
/**************************************************************************/
template <class T>
void ADS1115<T>::writeRegister(uint8_t reg, uint16_t value) {
    _pWire->beginTransmission(_i2cAddress);
    _pWire->write((uint8_t)reg);
    _pWire->write((uint8_t)(value >> 8));
    _pWire->write((uint8_t)(value & 0xFF));
    _pWire->endTransmission();
}
/**************************************************************************/
/*!
    @brief  Read 16-bits from the specified destination register
    @param reg register address to read from
    @return 16 bit register value read
*/
/**************************************************************************/
template <class T>
int16_t ADS1115<T>::readRegister(uint8_t reg) {
    _pWire->beginTransmission(_i2cAddress);
    _pWire->write(reg);
    _pWire->endTransmission();
    _pWire->requestFrom(_i2cAddress, (uint8_t)2);
    return ((_pWire->read() << 8) | _pWire->read());
}
/**************************************************************************/
/*!
    @brief Sets up the HW
    @param i2cAddress ads1115 address
                      0x48  ADS115_GND_ADDRESS
                      0x49  ADS115_VDD_ADDRESS
                      0x4A  ADS115_SDA_ADDRESS
                      0x4B  ADS115_SCL_ADDRESS
    @return status of wire connection
*/
/**************************************************************************/
template <class T>
bool ADS1115<T>::begin(uint8_t i2cAddress){
    _i2cAddress = i2cAddress;
    _pWire->begin();
    _pWire->beginTransmission(_i2cAddress);
    if(0 == _pWire->endTransmission()){
        return true;
    }else{
        return false;
    }
}
/**************************************************************************/
/*!
    @brief Set operate Mode
    @param operateMode ADS1115 operate mode 
                       Continuous-conversion mode ADS1115_MODE_CONTIN 
                       Single-shot mode  ADS1115_MODE_SINGLE
*/
/**************************************************************************/
template<class T>
void ADS1115<T>::setOperateMode(uint16_t operateMode){
    operateMode &= ADS1115_MODE_MASK;
    _configRegister &=~ADS1115_MODE_MASK;
    _configRegister |= operateMode;
}
/**************************************************************************/
/*!
    @brief Set operate staus
    @param operateStaus ADS1115 operate Staus
                        No effect  ADS1115_OS_NOEFFECT
                        Start a single conversion (when in power-down state) ADS1115_OS_SINGLE
*/              
/**************************************************************************/
template<class T>
void ADS1115<T>::setOperateStaus(uint16_t operateStaus){
    operateStaus &= ADS1115_OS_MASK;
    _configRegister &=~ADS1115_OS_MASK;  
    _configRegister |= operateStaus;
}
/**************************************************************************/
/*!
    @brief Set PGA gain
    @param PGAGain ADS1115 PGA gain
                   Full-Scale Range    param
                      ±0.256V       ADS1115_PGA_0_256
                      ±0.512V       ADS1115_PGA_0_512
                      ±1.024V       ADS1115_PGA_1_024
                      ±2.048V       ADS1115_PGA_2_048
                      ±4.096V       ADS1115_PGA_4_096
                      ±6.144V       ADS1115_PGA_6_144
*/
/**************************************************************************/
template<class T>
void ADS1115<T>::setPGAGain(uint16_t PGAGain){
    PGAGain &= ADS1115_PGA_MASK;
    _configRegister &=~ADS1115_PGA_MASK;    
    _configRegister |= PGAGain;
}
/**************************************************************************/
/*!
    @brief Set sample rate
    @param SampleRate ADS1115 sample rate
                      samples per second  param
                           8           ADS1115_DR_8
                           16          ADS1115_DR_16
                           32          ADS1115_DR_32
                           64          ADS1115_DR_64
                           128         ADS1115_DR_128
                           250         ADS1115_DR_250
                           475         ADS1115_DR_475
                           860         ADS1115_DR_860
*/
/**************************************************************************/
template<class T>
void ADS1115<T>::setSampleRate(uint16_t SampleRate){
    SampleRate &= ADS1115_DR_MASK;
    _configRegister &=~ADS1115_DR_MASK;
    _configRegister |= SampleRate;
}
/**************************************************************************/
/*!
    @brief Set Input Multiplexer
    @param InputMux ADS1115 sample rate
                    AIN P = AIN0 and AIN N = AIN1  ADS1115_MUX_0_1
                    AIN P = AIN0 and AIN N = AIN3  ADS1115_MUX_0_3
                    AIN P = AIN1 and AIN N = AIN3  ADS1115_MUX_1_3
                    AIN P = AIN2 and AIN N = AIN3  ADS1115_MUX_2_3
                    AIN P = AIN0 and AIN N = GND   ADS1115_MUX_0_G
                    AIN P = AIN1 and AIN N = GND   ADS1115_MUX_1_G
                    AIN P = AIN2 and AIN N = GND   ADS1115_MUX_2_G
                    AIN P = AIN3 and AIN N = GND   ADS1115_MUX_3_G
*/
/**************************************************************************/
template<class T>
void ADS1115<T>::setInputMux(uint16_t InputMux){
    InputMux &= ADS1115_MUX_MASK;
    _configRegister &=~ADS1115_MUX_MASK;
    _configRegister |= InputMux;
}
/**************************************************************************/
/*!
    @brief get Conversion Results
    @param channel ADC channel
                   AIN P = AIN0 and AIN N = AIN1  channel01
                   AIN P = AIN0 and AIN N = AIN3  channel03
                   AIN P = AIN1 and AIN N = AIN3  channel13
                   AIN P = AIN2 and AIN N = AIN3  channel23
                   AIN P = AIN0 and AIN N = GND   channel0
                   AIN P = AIN1 and AIN N = GND   channel1
                   AIN P = AIN2 and AIN N = GND   channel2
                   AIN P = AIN3 and AIN N = GND   channel3
    @return int16_t ADC vaule
*/
/**************************************************************************/
template <class T>
int16_t ADS1115<T>::getConversionResults(channel_t channel){
    setInputMux(_channelToMUX[channel]);
    writeRegister(ADS1115_POINTER_CONFIG, _configRegister);
    delay(ADS1115_CONVERSIONDELAY / (((_configRegister & ADS1115_DR_MASK)>>5) + 1));
    int16_t  ret_val =  readRegister(ADS1115_POINTER_CONVERT);
    if(channel < 4 && ret_val < 0) ret_val = 0;
    return (ret_val);
}
/**************************************************************************/
/*!
    @brief set Comparator mode
    @param CompeMode Comparator mode
                     Traditional comparator ADS1115_COMP_MODE_TRAD
                     Window comparator      ADS1115_COMP_MODE_WINDOW
*/
/**************************************************************************/
template <class T>
void ADS1115<T>::setCompaMode(uint16_t CompaMode){
    CompaMode &= ADS1115_COMP_MODE_MASK;
    _configRegister &=~ADS1115_COMP_MODE_MASK;
    _configRegister |= CompaMode;
}
/**************************************************************************/
/*!
    @brief set Comparator Polarity
    @param CompaPolarity Comparator Polarity
                         Active low   ADS1115_COMP_POL_ACTVLOW
                         Active high  ADS1115_COMP_POL_ACTVHIGH
*/
/**************************************************************************/
template <class T>
void ADS1115<T>::setCompaPolarity(uint16_t CompaPolarity){
    CompaPolarity &= ADS1115_COMP_POL_MASK;
    _configRegister &=~ADS1115_COMP_POL_MASK;
    _configRegister |= CompaPolarity;
}
/**************************************************************************/
/*!
    @brief set Comparator mode
    @param CompaLatching Comparator Latching
                         Nonlatching comparator   ADS1115_COMP_LAT_NONLAT
                         Latching comparator      ADS1115_COMP_LAT_LATCH
*/
/**************************************************************************/
template <class T>
void ADS1115<T>::setCompaLatching(uint16_t CompaLatching){
    CompaLatching &= ADS1115_COMP_LAT_MASK;
    _configRegister &=~ADS1115_COMP_LAT_MASK;
    _configRegister |= CompaLatching;
}
/**************************************************************************/
/*!
    @brief set Comparator queue
    @param CompaQueue Comparator Latching
                      Assert after one conversion     ADS1115_COMP_QUE_1CONV
                      Assert after two conversions    ADS1115_COMP_QUE_2CONV
                      Assert after four conversions   ADS1115_COMP_QUE_4CONV
                      Disable comparator              ADS1115_COMP_QUE_NONE
*/
/**************************************************************************/
template <class T>
void ADS1115<T>::setCompaQueue(uint16_t CompaQueue){
    CompaQueue &= ADS1115_COMP_QUE_MASK;
    _configRegister &=~ADS1115_COMP_QUE_MASK;
    _configRegister |= CompaQueue;
}
/**************************************************************************/
/*!
    @brief set low threshold value 
    @param threshold  low threshold value (0x0000 ~ 0x7fff)
    the Hi_thresh register value must always be greater than the Lo_thresh register value
*/
/**************************************************************************/
template <class T>
void ADS1115<T>::setLowThreshold(uint16_t threshold){
    writeRegister(ADS1115_POINTER_LOWTHRESH, threshold);
}
/**************************************************************************/
/*!
    @brief set high threshold value
    @param threshold  high threshold value (0x0000 ~ 0x7fff)
    the Hi_thresh register value must always be greater than the Lo_thresh register value
*/
/**************************************************************************/
template <class T>
void ADS1115<T>::setHighThreshold(uint16_t threshold){
    writeRegister(ADS1115_POINTER_HITHRESH, threshold);
}
#ifdef SOFTWAREWIRE
    template class ADS1115<SoftwareWire>;    
#else
    template class ADS1115<TwoWire>;
#endif