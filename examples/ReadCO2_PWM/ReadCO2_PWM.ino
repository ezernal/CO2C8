#include "CO2C8.h"
#include <HardwareSerial.h>

// Пин PWM на ESP8266
#define PWM_PIN D2

CO2C8 co2(Serial); // Используется только как объект, не используется UART

void setup() {
    Serial.begin(9600);
    pinMode(PWM_PIN, INPUT);
    delay(500);
    Serial.println("Работа в режиме PWM");
}

void loop() {
    uint16_t ppm = co2.readCO2_PWM(PWM_PIN);
    if (ppm > 0) {
        Serial.print("CO2: ");
        Serial.print(ppm);
        Serial.println(" ppm");
    } else {
        Serial.println("Ошибка чтения PWM");
    }

    delay(2000);
}
