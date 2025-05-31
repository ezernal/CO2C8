#include <SoftwareSerial.h>
#include "CO2C8.h"

// Настройки пинов UART
#define RX_PIN D5
#define TX_PIN D6

// Объекты
SoftwareSerial co2Serial(RX_PIN, TX_PIN);
CO2C8 co2(co2Serial);

void setup() {
    Serial.begin(9600);
    co2Serial.begin(9600);
    delay(500);
    Serial.println("Инициализация датчика CO2...");
}

void loop() {
    if (co2.requestCO2()) {
        Serial.print("CO2: ");
        Serial.print(co2.getCO2());
        Serial.println(" ppm");
    } else {
        Serial.println("Ошибка получения данных или CRC не совпадает");
    }

    delay(2000);
}
