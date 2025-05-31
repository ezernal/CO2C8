#ifndef CO2C8_H
#define CO2C8_H

#include <Arduino.h>
#include <Stream.h>

class CO2C8 {
public:
    // Конструкторы
    CO2C8(Stream& serial);
    
    // Методы UART
    bool requestCO2();
    uint16_t getCO2();
    bool isValid() const;

    // Методы PWM
    uint16_t readCO2_PWM(int pin);

private:
    Stream& _serial;
    uint8_t _buffer[16];
    uint16_t _co2 = 0;
    bool _valid = false;

    uint16_t calculateCRC(const uint8_t* data, uint16_t len);
};

#endif