#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>

MCUFRIEND_kbv tft;

#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define YELLOW 0xFFE0
#define ORANGE 0xFD20
#define MAGENTA 0xF81F

int pm25Value;  // Initial PM2.5 value
int co2Value;        // Initial CO2 value
int temperature;     // Initial temperature
int humidity;        // Initial humidity
int gasValue;   // Initial gas value

void setup() {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486;  // Adjust ID for compatibility
  tft.begin(ID);
  tft.setRotation(1);  // Landscape mode for 480x320
  drawUI();
}

void drawLine(int x, int y, int length, bool horizontal) {
  if (horizontal) tft.drawFastHLine(x, y, length, WHITE);
  else tft.drawFastVLine(x, y, length, WHITE);
}

void drawText(int x, int y, int size, uint16_t color, const char* text) {
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.setCursor(x, y);
  tft.print(text);
}

void drawUI() {
  tft.fillScreen(BLACK);

  // Draw dividing lines
  drawLine(10, 150, 310, true);
  drawLine(320, 85, 460, true);
  drawLine(320, 170, 460, true);
  drawLine(320, 10, 240, false);
  drawLine(10, 250, 480, true);

  drawText(50, 10, 2, MAGENTA, "Air Quality Monitor");

  // PM2.5 Value
  drawText(20, 40, 3, WHITE, "PM2.5");
  drawText(80, 70, 9, RED, String(pm25Value).c_str());
  drawText(240, 110, 2, WHITE, "ug/m3");

  // CO2 Value
  drawText(20, 160, 3, WHITE, "CO2");
  drawText(80, 190, 4, CYAN, String(co2Value).c_str());
  drawText(180, 200, 2, WHITE, "ppm");
  // Temperature and Humidity
  drawText(340, 20, 2, WHITE, "TEMP");
  drawText(340, 50, 3, WHITE, (String(temperature) + (char)247 + "C").c_str());
  drawText(340, 100, 2, WHITE, "HUMIDITY");
  drawText(340, 130, 3, WHITE, (String(humidity) + "%").c_str());
  drawText(340, 180, 2, WHITE, "G:Alc,Smoke");
  drawText(340, 210, 3, WHITE, (String(gasValue) + "%").c_str());

  // AQI Scale
  drawText(20, 260, 2, WHITE, "AQI");

  // AQI Color Bar
  int aqiColors[] = { GREEN, YELLOW, ORANGE, RED, MAGENTA };
  for (int i = 0; i < 5; i++) {
    tft.fillRect(80 + (i * 60), 260, 60, 20, aqiColors[i]);
  }

  // AQI Indicator
  updateAQIIndicator(pm25Value);

  // Battery Icon
  tft.drawRect(410, 10, 40, 20, WHITE);
  tft.fillRect(450, 14, 5, 12, WHITE);
  tft.fillRect(413, 13, 34, 14, GREEN);
}

void clearTextArea(int x, int y, int width, int height) {
  tft.fillRect(x, y, width, height, BLACK);
}

void updateValues() {
  clearTextArea(80, 70, 155, 70);                       // Clear larger PM2.5 value area
  drawText(80, 70, 9, RED, String(pm25Value).c_str());  // Redraw PM2.5 value

  clearTextArea(80, 190, 100, 30);  // Clear CO2 value area
  drawText(80, 190, 4, CYAN, String(co2Value).c_str());

  clearTextArea(340, 50, 60, 30);  // Clear Temperature area
  drawText(340, 50, 3, WHITE, (String(temperature) + (char)247 + "C").c_str());

  clearTextArea(340, 130, 40, 30);  // Clear Humidity area
  drawText(340, 130, 3, WHITE, (String(humidity) + "%").c_str());

  clearTextArea(340, 210, 40, 30);  // Clear Gas value area
  drawText(340, 210, 3, WHITE, (String(gasValue) + "%").c_str());

  updateAQIIndicator(pm25Value);  // Update the AQI indicator based on new PM2.5 value
}

void updateAQIIndicator(int pm25) {
  // Clear previous indicator position
  tft.fillRect(100, 285, 260, 30, BLACK);  // Clear the entire area where the triangle will be drawn

  // Set the new position based on the PM2.5 value
  if (pm25 >= 0 && pm25 <= 50) {
    tft.fillTriangle(110, 285, 100, 305, 120, 305, WHITE);  // Green area
  } else if (pm25 >= 51 && pm25 <= 100) {
    tft.fillTriangle(170, 285, 160, 305, 180, 305, WHITE);  // Yellow area
  } else if (pm25 >= 101 && pm25 <= 150) {
    tft.fillTriangle(230, 285, 220, 305, 240, 305, WHITE);  // Orange area
  } else if (pm25 >= 151 && pm25 <= 200) {
    tft.fillTriangle(290, 285, 280, 305, 300, 305, WHITE);  // Red area
  } else {
    tft.fillTriangle(350, 285, 340, 305, 360, 305, WHITE);  // Magenta area
  }
}

void loop() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 2000) {  // Update every 2 seconds
    lastUpdate = millis();

    // Simulate new sensor values
    // pm25Value = random(50, 250);  // Simulated range from 50 to 250 for testing
    // // co2Value = random(80, 150);
    // // temperature = random(20, 30);
    // // humidity = random(40, 60);
    // gasValue = random(40, 60);

    if (Serial.available()) {
      // Read the incoming data as a string until a newline character
      String incomingData = Serial.readStringUntil('\n');

      // Split the data into individual values
      int firstCommaIndex = incomingData.indexOf(',');
      int secondCommaIndex = incomingData.indexOf(',', firstCommaIndex + 1);
      int thirdCommaIndex = incomingData.indexOf(',', secondCommaIndex + 1);
      int fourthCommaIndex = incomingData.indexOf(',', thirdCommaIndex + 1);

      // Extract each value using substrings and convert to int
      temperature = incomingData.substring(0, firstCommaIndex).toInt();
      humidity = incomingData.substring(firstCommaIndex + 1, secondCommaIndex).toInt();
      co2Value = incomingData.substring(secondCommaIndex + 1, thirdCommaIndex).toInt();
      pm25Value = incomingData.substring(thirdCommaIndex + 1, fourthCommaIndex).toInt();
      gasValue = incomingData.substring(fourthCommaIndex + 1).toInt();

      // Print the extracted values as integers
      // Serial.print("Temperature: ");
      // Serial.println(temperature);
      // Serial.print("Humidity: ");
      // Serial.println(humidity);
      // Serial.print("CO2: ");
      // Serial.println(co2);
      // Serial.print("PM2.5: ");
      // Serial.println(pm25);
      // Serial.print("Gas: ");
      // Serial.println(gas);
    }

    updateValues();  // Update only the values that changed
  }
}
