#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>

MCUFRIEND_kbv tft;

#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED 0xF800
#define GREEN 0x07E0
#define YELLOW 0xFFE0
#define CYAN 0x07FF

int pm25Value = 45;
int co2Value = 300;
int gasValue = 80;
int temperature = 23;
int humidity = 86;
int aqi = 125;

void setup() {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486;
  tft.begin(ID);
  tft.setRotation(1); 
  drawUI();
}

void drawText(int x, int y, int size, uint16_t color, const char* text) {
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.setCursor(x, y);
  tft.print(text);
}

void drawWiFiIcon(int x, int y) {
  tft.fillCircle(x, y, 2, WHITE);  // Small center dot
  tft.drawCircle(x, y, 6, WHITE);  // First ring
  tft.drawCircle(x, y, 10, WHITE); // Second ring
  tft.drawLine(x - 6, y - 6, x + 6, y - 6, WHITE); // Top arc
  tft.drawLine(x - 10, y - 10, x + 10, y - 10, WHITE); // Outer arc
}

void drawBatteryIcon(int x, int y) {
  tft.drawRect(x, y, 40, 20, WHITE);
  tft.fillRect(x + 40, y + 4, 5, 12, WHITE);
  tft.fillRect(x + 3, y + 3, 34, 14, GREEN);
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
}

void drawUI() {
  tft.fillScreen(BLACK);
  
  drawWiFiIcon(20, 20);
  drawBatteryIcon(410, 10);

  drawText(20, 50, 3, WHITE, "AQI ");
  drawText(90, 50, 3, WHITE, String(aqi).c_str());

  int aqiColor;
  const char* aqiStatus;
  if (aqi < 100) {
    aqiColor = GREEN;
    aqiStatus = "GOOD";
  } else if (aqi < 200) {
    aqiColor = YELLOW;
    aqiStatus = "MODERATE";
  } else {
    aqiColor = RED;
    aqiStatus = "POOR";
  }
  
  tft.fillRoundRect(180, 50, 120, 30, 5, aqiColor);
  drawText(195, 57, 2, BLACK, aqiStatus);

  tft.fillRoundRect(16, 108, 80, 30, 5, aqiColor);
  drawText(20, 115, 2, BLACK, "PM2.5"); 
  drawText(130, 115, 3, WHITE, String(pm25Value).c_str());
  drawText(215, 115, 3, WHITE, "ug/m3");
  

  drawText(20, 190, 3, WHITE, "CO2");
  drawText(130, 190, 3, WHITE, String(co2Value).c_str());
  drawText(215, 190, 3, WHITE, "ppm");

  drawText(20, 265, 3, WHITE, "GAS");
  drawText(130, 265, 3, WHITE, String(gasValue).c_str());
  drawText(215, 265, 3, WHITE, "%");

  drawThermometerIcon(335, 150);
  drawText(360, 110, 4, WHITE, "TEMP");
  drawText(360, 160, 4, WHITE, (String(temperature) + "C").c_str());

  drawHumidityIcon(335, 270);
  drawText(360, 230, 4, WHITE, "HUMI");
  drawText(360, 280, 4, WHITE, (String(humidity) + "%").c_str());

  tft.drawLine(0, 90, 480, 90, WHITE);
  tft.drawLine(0, 166, 310, 166, WHITE);
  tft.drawLine(0, 242, 310, 242, WHITE);
  tft.drawLine(310, 210, 480, 210, WHITE);
  tft.drawLine(310, 90, 310, 320, WHITE);
}

void updateValues() {
  tft.fillRect(90, 50, 60, 30, BLACK);
  drawText(90, 50, 3, WHITE, String(aqi).c_str());

  tft.fillRect(140, 115, 50, 30, BLACK);
  drawText(130, 115, 3, WHITE, String(pm25Value).c_str());

  tft.fillRect(140, 190, 70, 30, BLACK);
  drawText(130, 190, 3, WHITE, String(co2Value).c_str());

  tft.fillRect(130, 265, 90, 30, BLACK);
  drawText(130, 265, 3, WHITE, String(gasValue).c_str());

  tft.fillRect(360, 160, 60, 30, BLACK);
  drawText(360, 160, 4, WHITE, (String(temperature) + "C").c_str());

  tft.fillRect(360, 280, 60, 30, BLACK);
  drawText(360, 280, 4, WHITE, (String(humidity) + "%").c_str());

  int aqiColor;
  const char* aqiStatus;
  if (aqi < 100) {
    aqiColor = GREEN;
    aqiStatus = "GOOD";
  } else if (aqi < 200) {
    aqiColor = YELLOW;
    aqiStatus = "MODERATE";
  } else {
    aqiColor = RED;
    aqiStatus = "POOR";
  }

  tft.fillRoundRect(180, 50, 120, 30, 5, aqiColor);
  drawText(195, 57, 2, BLACK, aqiStatus);
}

void loop() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 2000) {
    lastUpdate = millis();

    if (Serial.available()) {
      String incomingData = Serial.readStringUntil('\n');

      int firstComma = incomingData.indexOf(',');
      int secondComma = incomingData.indexOf(',', firstComma + 1);
      int thirdComma = incomingData.indexOf(',', secondComma + 1);
      int fourthComma = incomingData.indexOf(',', thirdComma + 1);

      pm25Value = incomingData.substring(0, firstComma).toInt();
      co2Value = incomingData.substring(firstComma + 1, secondComma).toInt();
      gasValue = incomingData.substring(secondComma + 1, thirdComma).toInt();
      temperature = incomingData.substring(thirdComma + 1, fourthComma).toInt();
      humidity = incomingData.substring(fourthComma + 1).toInt();
    }

    updateValues();
  }
}
