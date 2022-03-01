#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Gear.h"

Gear gear();

void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.println("Ciao Ricky");
    delay(1000);
}