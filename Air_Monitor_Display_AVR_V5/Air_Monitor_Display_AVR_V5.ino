#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>

MCUFRIEND_kbv tft;

#define BLACK  0x0000
#define WHITE  0xFFFF
#define GREEN  0x07E0
#define RED    0xF800
#define BLUE   0x001F
#define CYAN   0x07FF
#define YELLOW 0xFFE0
#define ORANGE 0xFD20
#define GRAY   0x8410

int aqi = 0;
int co2Value = 0;
int pm25Value = 0;
int gasValue = 0;
int temperature = 0;
int humidity = 0;

void setup() {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486;
  tft.begin(ID);
  tft.setRotation(0);  // Portrait mode
  drawUI();
}

void drawText(int x, int y, int size, uint16_t color, const char* text) {
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.setCursor(x, y);
  tft.print(text);
}

// void drawWiFiIcon(int x, int y) {
//   tft.fillCircle(x, y, 15, WHITE);
//   tft.fillCircle(x, y, 12, BLACK);
//   tft.fillRect(x-15, y - 6, 32, 22, BLACK);
//   tft.fillCircle(x, y+4, 12, WHITE);
//   tft.fillCircle(x, y+4, 9, BLACK);
//   tft.fillRect(x-15, y - 2, 32, 22, BLACK);
//   tft.fillCircle(x, y+2, 3, WHITE);
// }
void drawWiFiIcon(int x, int y) {
  tft.fillCircle(x, y, 15, WHITE);
  tft.fillCircle(x, y, 12, BLACK);
  tft.fillCircle(x, y, 9, WHITE);
  tft.fillCircle(x, y, 6, BLACK);
  tft.fillCircle(x, y, 3, WHITE);
  tft.fillRect(x-35, y-20, 32, 40, BLACK);
  tft.fillRect(x-15, y + 4, 32, 20, BLACK);
}

void drawBatteryIcon(int x, int y) {
  tft.drawRect(x, y, 40, 20, WHITE);
  tft.fillRect(x + 40, y + 5, 5, 10, WHITE);
  tft.fillRect(x + 3, y + 3, 34, 14, GREEN);
  // tft.fillRect(x + 3, y + 3, 18, 14, YELLOW);
  // tft.fillRect(x + 3, y + 3, 8, 14, RED);
}

void drawSensorBox(int x, int y, const char* label, int value, const char* unit, int maxValue) {
  uint16_t color = GREEN;

  // Determine box color based on sensor range
  float percent = (value / (float)maxValue) * 100;
  if (percent > 50) color = YELLOW;
  if (percent > 80) color = RED;

  // Draw the sensor box
  tft.fillRoundRect(x, y, 80, 40, 5, color);
  drawText(x + 10, y + 12, 2, BLACK, label);

  // Clear previous value area
  tft.fillRect(x + 100, y + 4, 180, 40, BLACK);
  drawText(x + 100, y + 4, 5, WHITE, String(value).c_str());
  drawText(x + 230, y + 30, 2, WHITE, unit);
}

void drawThickLine(int y) {
  tft.fillRect(0, y, 320, 2, WHITE);  // Thick line (4px)
}

// Map PM2.5 (0-500 ug/m3) to AQI (0-250)
int mapPM25toAQI(int pm25) {
  return map(pm25, 0, 500, 0, 250);
}

void drawAQIBar() {
  // Calculate AQI based on PM2.5
  aqi = mapPM25toAQI(pm25Value);

  uint16_t aqiColor = GREEN;
  const char* aqiText = "Healthy";

  if (aqi > 150) { aqiColor = YELLOW; aqiText = "Moderate"; }
  if (aqi > 200) { aqiColor = RED; aqiText = "Unhealthy"; }

  // Clear AQI area before updating
  tft.fillRect(20, 45, 300, 50, BLACK);

  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(25, 65);
  tft.print("AQI: ");
  
  tft.setTextSize(4);
  tft.setCursor(110, 60);
  tft.print(aqi);

  // AQI Indicator box
  tft.fillRoundRect(110, 10, 100, 30, 5, aqiColor);
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.setCursor(115, 17);
  tft.print(aqiText);
}
void drawThermometerIcon(int x, int y) {
  tft.fillCircle(x, y - 30, 5, RED);
  tft.fillCircle(x, y + 20, 15, RED);
  tft.fillRect(x - 5, y - 30, 12, 40, RED);
  tft.fillRect(x, y - 8, 8, 3, WHITE);
  tft.fillRect(x, y - 16, 8, 3, WHITE);
  tft.fillRect(x, y - 24, 8, 3, WHITE);
}

void drawHumidityIcon(int x, int y) {
  tft.fillCircle(x, y + 10, 15, CYAN);
  tft.fillTriangle(x - 16, y + 12, x + 16, y + 12, x, y - 28, CYAN);
  tft.fillCircle(x + 3, y + 10, 5, WHITE);
}

void drawUI() {
  tft.fillScreen(BLACK);
  
  // Top icons
  drawWiFiIcon(20, 25);
  drawBatteryIcon(260, 10);

  // AQI Bar
  drawAQIBar();
  drawThickLine(100);

  drawSensorBox(20, 120, "PM2.5", pm25Value, "ug/m3", 500);
  drawThickLine(180);
  
  drawSensorBox(20, 200, "CO2", co2Value, "ppm", 5000);
  drawThickLine(260);
  
  drawSensorBox(20, 280, "GAS", gasValue, "%", 100);
  drawThickLine(340);

  // Temperature & Humidity (Bottom Section)
  drawThermometerIcon(30, 390);
  drawText(60, 360, 2, WHITE, "TEMP");
  drawText(60, 390, 4, WHITE, (String(temperature) + "C").c_str());

  drawHumidityIcon(200, 400);
  drawText(230, 360, 2, WHITE, "HUMI");
  drawText(230, 390, 4, WHITE, (String(humidity) + "%").c_str());

  drawText(50,450,2,WHITE,"Air Quality Monitor");
}

void updateValues() {
  drawAQIBar();
  drawSensorBox(20, 120, "PM2.5", pm25Value, "ug/m3", 500);
  drawSensorBox(20, 200, "CO2", co2Value, "ppm", 5000);
  drawSensorBox(20, 280, "GAS", gasValue, "%", 100);

  // Clear previous temperature and humidity values
  tft.fillRect(60, 390, 50, 30, BLACK);  // Clear temp area
  tft.fillRect(230, 390, 50, 30, BLACK); // Clear humidity area

  // Update Temperature
  drawText(60, 390, 4, WHITE, (String(temperature) + "C").c_str());

  // Update Humidity
  drawText(230, 390, 4, WHITE, (String(humidity) + "%").c_str());
}


// Read Serial Data
void readSerialData() {
  if (Serial.available()) {
    String incomingData = Serial.readStringUntil('\n');

    int firstComma = incomingData.indexOf(',');
    int secondComma = incomingData.indexOf(',', firstComma + 1);
    int thirdComma = incomingData.indexOf(',', secondComma + 1);
    int fourthComma = incomingData.indexOf(',', thirdComma + 1);

    temperature = incomingData.substring(0, firstComma).toInt();
    humidity = incomingData.substring(firstComma + 1, secondComma).toInt();
    co2Value = incomingData.substring(secondComma + 1, thirdComma).toInt();
    pm25Value = incomingData.substring(thirdComma + 1, fourthComma).toInt();
    gasValue = incomingData.substring(fourthComma + 1).toInt();
  }
}

void loop() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 2000) {  
    lastUpdate = millis();
    readSerialData();
    updateValues();
  }
}
