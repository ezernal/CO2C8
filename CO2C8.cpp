#include "CO2C8.h"

CO2C8::CO2C8(Stream& serial) : _serial(serial) {}

bool CO2C8::requestCO2() {
    const uint8_t request[] = {0x64, 0x69, 0x03, 0x5E, 0x4E};

    while (_serial.available()) _serial.read(); // Очистка буфера
    _serial.write(request, sizeof(request));

    unsigned long start = millis();
    uint8_t idx = 0;
    while (millis() - start < 2000 && idx < 16) {
        if (_serial.available()) {
            _buffer[idx++] = _serial.read();
            if (idx >= 14) break;
        }
    }

    if (idx != 14) {
        _valid = false;
        return false;
    }

    uint16_t receivedCRC = (_buffer[13] << 8) | _buffer[12];
    uint16_t calculatedCRC = calculateCRC(_buffer, 12);

    if (receivedCRC != calculatedCRC) {
        _valid = false;
        return false;
    }

    _co2 = (_buffer[5] << 8) | _buffer[4]; // Байты 4 и 5
    _valid = true;
    return true;
}

uint16_t CO2C8::getCO2() {
    return _co2;
}

bool CO2C8::isValid() const {
    return _valid;
}

// === PWM режим ===
uint16_t CO2C8::readCO2_PWM(int pin) {
    int highTime = pulseIn(pin, HIGH, 20000); // макс. ожидание 20 мс
    if (highTime == 0) return 0;

    uint16_t co2ppm = static_cast<uint16_t>((highTime - 2000) / 100 * 5);
    _co2 = co2ppm;
    _valid = true;
    return _co2;
}

// === CRC-16/Modbus ===
uint16_t CO2C8::calculateCRC(const uint8_t* data, uint16_t len) {
    uint16_t crc = 0xFFFF;

    while (len--) {
        crc ^= *data++;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001; // Полином CRC-16/Modbus
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}