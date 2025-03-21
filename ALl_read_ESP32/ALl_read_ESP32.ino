#include "PMS.h"
#include <HardwareSerial.h>
#include <Adafruit_SCD30.h>

Adafruit_SCD30 scd30;
PMS pms(Serial2);
PMS::DATA data;

String val;
int val1;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);

  while (!Serial) delay(10);
  Serial.println("Adafruit SCD30 test!");

  if (!scd30.begin()) {
    Serial.println("Failed to find SCD30 chip");
    while (1) delay(10);
  }
  Serial.println("SCD30 Found!");
  delay(2000);
}

void loop() {
  if (pms.read(data)) {
    val = data.PM_AE_UG_2_5;
    delay(2000);
  }
  if (scd30.dataReady() && scd30.read()) {
    Serial.print(scd30.temperature);
    Serial.print(",");
    Serial.print(scd30.relative_humidity);
    Serial.print(",");
    Serial.print(scd30.CO2);
    Serial.print(",");
    Serial.print(val);
    Serial.print(",");
    val1 = analogRead(32);
    val1 = map(val1, 0,4095,0,100);
    Serial.println(val1);
  }
}