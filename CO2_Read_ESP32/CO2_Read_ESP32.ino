#include <Adafruit_SCD30.h>

Adafruit_SCD30 scd30;

void setup() {
    Serial.begin(9600); // For debugging
    
    while (!Serial) delay(10);

    Serial.println("Adafruit SCD30 test!");

    if (!scd30.begin()) {
        Serial.println("Failed to find SCD30 chip");
        while (1) delay(10);
    }
    Serial.println("SCD30 Found!");
}

void loop() {
    if (scd30.dataReady() && scd30.read()) {
        Serial.print(scd30.temperature);
        Serial.print(","); // Use comma as a delimiter
        Serial.print(scd30.relative_humidity);
        Serial.print(",");
        Serial.println(scd30.CO2);
    }
    delay(2000); // Send data every 2 seconds
}
